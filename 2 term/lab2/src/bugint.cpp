#include "../include/bugint.h"

void bugint::dispose()
{
    delete[] _other_digits;
    _other_digits = nullptr;
}

void bugint::copy_from(
    bugint const &other)
{
    _oldest_digit = other._oldest_digit;
    _other_digits = nullptr;

    if (other._other_digits != nullptr)
    {
        _other_digits = new int[*other._other_digits];
        memcpy(_other_digits, other._other_digits, *other._other_digits * sizeof(int));
    }
}

void bugint::move_from(
    bugint &&other)
{
    _oldest_digit = other._oldest_digit;
    other._oldest_digit = 0;

    _other_digits = other._other_digits;
    other._other_digits = nullptr;
}

bugint::~bugint() noexcept
{
    dispose();
}

bugint::bugint(
    bugint const &other):
        _other_digits(nullptr)
{
    copy_from(other);
}

bugint::bugint(
    bugint &&other) noexcept:
        _other_digits(nullptr)
{
    move_from(std::move(other));
}

bugint &bugint::operator=(
    bugint const &other)
{
    if (this != &other)
    {
        dispose();
        copy_from(other);
    }

    return *this;
}

bugint &bugint::operator=(
    bugint &&other) noexcept
{
    if (this != &other)
    {
        dispose();
        move_from(std::move(other));
    }

    return *this;
}

bugint &bugint::init_with(
    int const *digits_array,
    size_t digits_count)
{
    if (digits_array == nullptr)
    {
        throw std::invalid_argument("Pointer to digits array can't be EQ to nullptr");
    }

    if (digits_count == 0)
    {
        throw std::invalid_argument("Digits count can't be EQ to 0");
    }

    dispose();

    if (digits_count == 1)
    {
        _oldest_digit = *digits_array;

        return *this;
    }

    while (digits_count != 1 && ((digits_array[digits_count - 1] == 0 && digits_array[digits_count - 2] >= 0) || (digits_array[digits_count - 1] == -1 && digits_array[digits_count - 2] < 0)))
    {
        --digits_count;
    }

    *(_other_digits = new int[digits_count]) = static_cast<int>(digits_count);
    memcpy(_other_digits + 1, digits_array, (digits_count - 1) * sizeof(int));
    _oldest_digit = digits_array[digits_count - 1];

    return *this;
}

bugint::bugint(
    int const *digits,
    size_t digits_count):
        _other_digits(nullptr)
{
    init_with(digits, digits_count);
}

bugint::bugint(
    char const *string_representation,
    size_t base):
        _other_digits(nullptr)
{
    // TODO: this requires implemented operator+= and operator*=
}

int bugint::get_sign() const noexcept
{
    if (_oldest_digit == 0 && _other_digits == nullptr)
    {
        return 0;
    }

    // return _oldest_digit > 0
    return (_oldest_digit >> ((sizeof(int) << 3) - 1)) == 0
        ? 1
        : -1;
}

size_t bugint::get_digits_count() const noexcept
{
    return _other_digits == nullptr
        ? 1
        : *_other_digits;
}

unsigned int bugint::operator[](
    size_t index) const noexcept
{
    auto const digits_count = get_digits_count();

    if (index >= digits_count)
    {
        return 0;
    }

    return *reinterpret_cast<unsigned int const *>(index == digits_count - 1
        ? &_oldest_digit
        : _other_digits + 1 + index);
}

int &bugint::operator[](
    size_t index)
{
    auto const digits_count = get_digits_count();

    if (index >= digits_count)
    {
        throw std::out_of_range("out of range of digits array");
    }

    return index == digits_count - 1
        ? _oldest_digit
        : *(_other_digits + 1 + index);
}

bugint& bugint::invert() &
{
    _oldest_digit = ~_oldest_digit;
    if (_other_digits == nullptr)
    {
        return *this;
    }

    for (int i = 1; i < *_other_digits - 1; ++i)
    {
        _other_digits[i] = ~_other_digits[i];
    }

    return *this;
}

bugint &bugint::negate() &
{
    if (get_sign() == 0)
    {
        return *this;
    }

    int one_int = 1;
    bugint one(&one_int, 1);

    return get_sign() == 1
        ? invert() += one
        : (*this -= one).invert();
}

bugint bugint::operator-() const
{
    bugint negative(*this);

    return negative.negate();
}

bugint &bugint::operator+=(
    bugint const &summand) &
{
    unsigned int max_digits_count = get_max(get_digits_count(), summand.get_digits_count()) + 1;

    int *result_space = new int[max_digits_count];
    unsigned int extra_digit = 0;

    for (size_t i = 0; i < max_digits_count; ++i)
    {
        result_space[i] = 0;

        auto this_digit = static_cast<bugint const *>(this)->operator[](i);
        auto other_digit = summand[i];

        for (size_t j = 0; j < 2; ++j)
        {
            auto this_half_digit = j == 0
                ? get_loword(this_digit)
                : get_hiword(this_digit);

            auto other_half_digit = j == 0
                ? get_loword(other_digit)
                : get_hiword(other_digit);

            auto digits_sum = this_half_digit + other_half_digit + extra_digit;

            extra_digit = digits_sum >> SHIFT;

            result_space[i] += (digits_sum & MASK) << (j * SHIFT);
        }
    }

    init_with(result_space, max_digits_count);

    delete[] result_space;

    return *this;
}

bugint bugint::operator+(
    bugint const &summand) const
{
    bugint result(*this);

    return result += summand;
}

bugint &bugint::operator++()
{
    if (get_sign() == -1)
    {
        return (--negate()).negate();
    }

    size_t digits_count = get_digits_count();
    for (int i = 0; i < digits_count - 1; ++i)
    {
        if (++((*this)[i]) != 0)
        {
            return *this;
        }
    }

    if (++_oldest_digit != INT_MIN)
    {
        return *this;
    }

    if (_other_digits == nullptr)
    {
        _other_digits = new int[2];
        _other_digits[0] = 2;
        _other_digits[1] = _oldest_digit;
        _oldest_digit = 0;

        return *this;
    }

    int *new_array = new int[digits_count + 1];
    memcpy(new_array, _other_digits, sizeof(int) * digits_count);
    ++*new_array;

    delete [] _other_digits;
    _other_digits = new_array;

    (*this)[digits_count - 1] = _oldest_digit;
    _oldest_digit = 0;

    return *this;
}

bool bugint::is_negative() const {
    return negative_flag; // где negative_flag - это член класса, хранящий информацию о знаке
}

int bugint::compare(const bugint&first, const bugint&second){
    if (first.is_negative() && !second.is_negative()){
        return -1;
    }
    if (!first.is_negative() && second.is_negative()){
        return 1;
    }
    if (first.is_negative() && second.is_negative()){
        return compare(-second, -first);
    }
    if (first.get_digits_count()>second.get_digits_count()){
        return 1;
    }
    if (first.get_digits_count()<second.get_digits_count()){
        return -1;
    }
    for (size_t i=0; i<first.get_digits_count();++i){
        if (first[i]>second[i]) return 1;
        if (first[i]<second[i]) return -1;

    }
    return 0;
}


bugint const bugint::operator++(
    int)
{
    bugint curr(*this);
    ++*this;
    return curr;
}

bugint &bugint::operator-=(
    bugint const &minuend) &
{
    return *this += -minuend;
}

bugint bugint::operator-(
    bugint const &minuend) const
{
    bugint result(*this);

    return result -= minuend;
}

bugint &bugint::operator--()
{
    if (get_sign() == -1)
    {
        return (++negate()).negate();
    }

    if (get_sign() == 0)
    {
        _oldest_digit = -1;
        return *this;
    }

    size_t digits_count = get_digits_count();
    for (int i = 0; i < digits_count; ++i)
    {
        if (--((*this)[i]) != -1)
        {
            break;
        }
    }

    if (_oldest_digit != 0)
    {
        return *this;
    }

    if (get_sign() == 0)
    {
        return *this;
    }

    if (_other_digits[digits_count - 1] != INT_MAX)
    {
        return *this;
    }

    if (get_digits_count() == 2)
    {
        _oldest_digit = _other_digits[1];
        delete[] _other_digits;
        _other_digits = nullptr;

        return *this;
    }

    int *new_array = new int[digits_count - 1];
    memcpy(new_array, _other_digits, sizeof(int) * get_digits_count() - 1);
    --*new_array;

    _oldest_digit = _other_digits[digits_count - 1];

    delete[] _other_digits;
    _other_digits = new_array;

    return *this;
}

bugint const bugint::operator--(
    int)
{
    bugint x = *this;
    --*this;
    return x;
}

bugint &bugint::operator*=(
    bugint const &multiplier) &
{
    if (get_sign() == 0)
    {
        return *this;
    }

    if (multiplier.get_sign() == 0)
    {
        return *this = multiplier;
    }

    // TODO: German should handle multiplication of two negative values o_O

    if (multiplier.get_sign() == -1)
    {
        return (*this *= -multiplier).negate();
    }

    if (get_sign() == -1)
    {
        return (negate() *= multiplier).negate();
    }

    // TODO: Egor Letov should handle multiplying by 1/-1

    int words_multiplication_result_digits[2] = { 0 };
    int digit = 0;
    unsigned int words_multiplication_result_digit = 0;
    bugint const zero(&digit, 1);
    bugint result = zero;

    auto this_digits_count = get_digits_count();
    auto multiplier_digits_count = multiplier.get_digits_count();

    for (size_t i = 0; i < this_digits_count; ++i)
    {
        auto this_digit = (*this)[i];
        auto this_digit_loword = get_loword(this_digit);
        auto this_digit_hiword = get_hiword(this_digit);

        for (size_t j = 0; j < multiplier_digits_count; ++j)
        {
            auto multiplier_digit = multiplier[j];
            auto multiplier_digit_loword = get_loword(multiplier_digit);
            auto multiplier_digit_hiword = get_hiword(multiplier_digit);

            addition_for_multiplication(result, words_multiplication_result_digits, this_digit_loword, multiplier_digit_loword, (i + j) << 1);
            addition_for_multiplication(result, words_multiplication_result_digits, this_digit_loword, multiplier_digit_hiword, (i + j + 1) << 1);
            addition_for_multiplication(result, words_multiplication_result_digits, this_digit_hiword, multiplier_digit_loword, (i + j + 1) << 1);
            addition_for_multiplication(result, words_multiplication_result_digits, this_digit_hiword, multiplier_digit_hiword, (i + j + 2) << 1);
        }
    }

    delete[] _other_digits;
    _other_digits = result._other_digits;
    result._other_digits = nullptr;
    _oldest_digit = result._oldest_digit;

    return *this;
}

bugint bugint::operator*(
    bugint const &multiplier) const
{
    bugint result = *this;

    return result *= multiplier;
}

bugint &bugint::operator/=(
    bugint const &divisor) &
{

}

bugint bugint::operator/(
    bugint const &divisor) const
{

}

bugint &bugint::operator%=(
    bugint const &divisor) &
{

}

bugint bugint::operator%(
    bugint const &divisor) const
{

}

bugint::division_result bugint::division(
    bugint const &divisor) const
{

}

bool bugint::operator==(
    bugint const &other) const
{
    return bugint::compare(*this, other)==0;
}

bool bugint::operator!=(
    bugint const &other) const
{
    return !(*this==other);
}

bool bugint::operator<(
    bugint const &other) const
{
    return bugint::compare(*this,other) <0;
}

bool bugint::operator<=(
    bugint const &other) const
{
    return bugint::compare(*this,other)<=0;
}

bool bugint::operator>(
    bugint const &other) const
{
    return (*this<other);
}

bool bugint::operator>=(
    bugint const &other) const
{
    return *this<=other;
}

bugint bugint::operator~() const
{

}

bugint &bugint::operator&=(
    bugint const &other) &
{

}

bugint bugint::operator&(
    bugint const &other) const
{

}

bugint &bugint::operator|=(
    bugint const &other) &
{

}

bugint bugint::operator|(
    bugint const &other) const
{

}

bugint &bugint::operator^=(
    bugint const &other) &
{

}

bugint bugint::operator^(
    bugint const &other) const
{

}

bugint &bugint::operator<<=(
    size_t shift) &
{

}

bugint bugint::operator<<(
    size_t shift) const
{

}

bugint &bugint::operator>>=(
    size_t shift) &
{

}

bugint bugint::operator>>(
    size_t shift) const
{

}

std::ostream &operator<<(
    std::ostream &stream,
    bugint const &value)
{
    auto digits_count = value.get_digits_count();
    for (auto i = 0; i < digits_count; ++i)
    {
        stream << const_cast<bugint &>(value)[i] << ' ';
    }

    return stream;
}

std::istream &operator>>(
    std::istream &stream,
    bugint &value)
{


    return stream;
}