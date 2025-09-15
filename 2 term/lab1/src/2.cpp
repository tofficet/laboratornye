#include <iostream>

class logical_values_array final {
   private:
    unsigned int _value;

   public:
    unsigned int get_value() const;
    logical_values_array(unsigned int value = 0);

    logical_values_array logical_and(const logical_values_array& other) const;
    logical_values_array logical_or(const logical_values_array& other) const;
    logical_values_array logical_not() const;
    logical_values_array implies(const logical_values_array& other) const;
    logical_values_array equivalence(const logical_values_array& other) const;
    logical_values_array xor_op(const logical_values_array& other) const;
    logical_values_array nand_op(const logical_values_array& other) const;
    logical_values_array nor_op(const logical_values_array& other) const;

    static bool equals(const logical_values_array& a, const logical_values_array& b);
    int get_bit(size_t position) const;
    int operator[](size_t position) const;
    void to_binary_string(char* buffer) const;
};

logical_values_array::logical_values_array(unsigned int value) : _value(value) {}

unsigned int logical_values_array::get_value() const { return _value; }

logical_values_array logical_values_array::logical_and(const logical_values_array& other) const {
    return logical_values_array(_value & other._value);
}

logical_values_array logical_values_array::logical_or(const logical_values_array& other) const {
    return logical_values_array(_value | other._value);
}

logical_values_array logical_values_array::logical_not() const { return logical_values_array(~_value); }

logical_values_array logical_values_array::implies(const logical_values_array& other) const {
    return logical_values_array((~_value) | other._value);
}

logical_values_array logical_values_array::equivalence(const logical_values_array& other) const {
    return logical_values_array((_value & other._value) | (~_value & ~other._value));
}

logical_values_array logical_values_array::xor_op(const logical_values_array& other) const {
    return logical_values_array(_value ^ other._value);
}

logical_values_array logical_values_array::nand_op(const logical_values_array& other) const {
    return logical_values_array(~(_value & other._value));
}

logical_values_array logical_values_array::nor_op(const logical_values_array& other) const {
    return logical_values_array(~(_value | other._value));
}

int logical_values_array::get_bit(size_t position) const { return (_value >> position) & 1; }

int logical_values_array::operator[](size_t position) const { return get_bit(position); }

void logical_values_array::to_binary_string(char* buffer) const {
    const int bits = sizeof(_value) * 8;
    for (int i = 0; i < bits; ++i) {
        buffer[i] = ((_value >> (bits - 1 - i)) & 1) ? '1' : '0';
    }
    buffer[bits] = '\0';
}

bool logical_values_array::equals(const logical_values_array& a, const logical_values_array& b) {
    return a._value == b._value;
}

int main() {
    logical_values_array ob1(0b111);
    logical_values_array ob2(0b11);

    logical_values_array result;

    result = ob1.logical_not();
    std::cout << "NOT: " << result.get_value() << std::endl;

    result = ob1.logical_and(ob2);
    std::cout << "AND: " << result.get_value() << std::endl;

    result = ob1.logical_or(ob2);
    std::cout << "OR: " << result.get_value() << std::endl;

    result = ob1.implies(ob2);
    std::cout << "IMPLIES: " << result.get_value() << std::endl;

    result = ob1.equivalence(ob2);
    std::cout << "EQUIVALENCE: " << result.get_value() << std::endl;

    result = ob1.xor_op(ob2);
    std::cout << "XOR: " << result.get_value() << std::endl;

    result = ob1.nor_op(ob2);
    std::cout << "NOR: " << result.get_value() << std::endl;

    result = ob1.nand_op(ob2);
    std::cout << "NAND: " << result.get_value() << std::endl;

    std::cout << "EQUALS: " << std::boolalpha << logical_values_array::equals(ob1, ob2) << std::endl;

    std::cout << "Bit ob1[3]: " << ob1[3] << std::endl;
    std::cout << "Bit ob2[2]: " << ob2[2] << std::endl;

    char binary_str[33];
    ob1.to_binary_string(binary_str);
    std::cout << "Binary ob1: " << binary_str << std::endl;

    ob2.to_binary_string(binary_str);
    std::cout << "Binary ob2: " << binary_str << std::endl;

    return 0;
}