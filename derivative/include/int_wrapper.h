#ifndef PUKSRENJK_INT_WRAPPER_H
#define PUKSRENJK_INT_WRAPPER_H

class int_wrapper final
{

private:

    int *_value_ptr;

public:

    explicit int_wrapper(
        int value = 0): _value_ptr(new int(value)){};

    ~int_wrapper(){
        delete _value_ptr;
    }

    int_wrapper(
        int_wrapper const &other): _value_ptr(new int (*other._value_ptr)){}

    int_wrapper &operator=(
        int_wrapper const &other){
            if (this != &other){
                delete _value_ptr;
                _value_ptr=new int(*other._value_ptr);
            }
            return *this;
        }
    int_wrapper(
        int_wrapper &&other) noexcept:
         _value_ptr(other._value_ptr){
            other._value_ptr=nullptr;
        }
    
    int_wrapper &operator=(
        int_wrapper  &&other) noexcept{
            if (this != &other){
                delete _value_ptr;
                _value_ptr-other._value_ptr;
                other._value_ptr=nullptr;
            }
            return *this;
        }


public:

    // TODO: arithmetic operators
    // +=, +, -=, -, *=, *, /=, /, %=, %, ++v, v++, --v, v--

    // TODO: bitwise operators
    // <<=, <<, >>=, >>, &=, &, |=, |=, ^=, ^

    // TODO: stream operator
    // <<, >>

};





#endif //PUKSRENJK_INT_WRAPPER_H

