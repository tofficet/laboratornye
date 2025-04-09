#include <iostream>

class logical_values_array {
private:
    unsigned int _value;

public:
    unsigned int accessor() const;
    logical_values_array(unsigned int value = 0);

    logical_values_array and_op(const logical_values_array& other) const;
    logical_values_array or_op(const logical_values_array& other) const;
    logical_values_array invert() const;
    logical_values_array srav_op(const logical_values_array& other) const;
    logical_values_array unsrav_op(const logical_values_array& other) const;
    logical_values_array sumabs_op(const logical_values_array& other) const;
    logical_values_array ekva_op(const logical_values_array& other) const;
    logical_values_array pir_op(const logical_values_array& other) const;
    logical_values_array shiff_op(const logical_values_array& other) const;

    static void equals(const logical_values_array& a, const logical_values_array& b);
    int get_bit(size_t position) const; 
    int operator[](size_t position) const; 
    void bybite(char * other)const ;

};

logical_values_array::logical_values_array(unsigned int value) : _value(value) {}

unsigned int logical_values_array::accessor() const {
    return _value;
}

logical_values_array logical_values_array::and_op(const logical_values_array& other) const {
    return logical_values_array(_value & other._value);
}

logical_values_array logical_values_array::or_op(const logical_values_array& other) const {
    return logical_values_array(_value | other._value);
}

logical_values_array logical_values_array::invert() const {
    return logical_values_array(~_value);
}

logical_values_array logical_values_array::srav_op(const logical_values_array& other) const {
    return logical_values_array((~_value) | other._value);
}

logical_values_array logical_values_array::unsrav_op(const logical_values_array& other) const {
    return logical_values_array((~_value | other._value) & (~other._value | _value));
}

logical_values_array logical_values_array::sumabs_op(const logical_values_array& other) const {
    return logical_values_array(_value ^ other._value);
}

logical_values_array logical_values_array::ekva_op(const logical_values_array& other) const {
    return logical_values_array((_value & other._value) | (~_value & ~other._value));
}

logical_values_array logical_values_array::pir_op(const logical_values_array& other) const {
    return logical_values_array(~(_value | other._value)); 
}

logical_values_array logical_values_array::shiff_op(const logical_values_array& other) const {
    return logical_values_array(~(_value & other._value));
}

int logical_values_array::get_bit(size_t position) const {
    return (_value >> position) & 1;
}

int logical_values_array::operator[](size_t position) const{
    return get_bit(position);
}

void logical_values_array::bybite(char* other) const {
    const int bite = sizeof(_value)*8;
    for (int i=0; i<bite; ++i){
        other[i] = ((_value >> (bite - 1 - i)) & 1) ? '1' : '0';
    }
    other[bite]='\0';

}

void logical_values_array::equals(const logical_values_array& a, const logical_values_array& b) {
    if (a._value == b._value) {
        std::cout << "Numbers are equivalent." << std::endl;
    } else {
        std::cout << "Numbers are not equivalent." << std::endl;
    }
}

int main() {
    logical_values_array ob1(0b111);  
    logical_values_array ob2(0b11);

    logical_values_array result;

    result = ob1.invert();
    std::cout << "In: " << result.accessor() << std::endl;

    result = ob1.and_op(ob2);  
    std::cout << "kon: " << result.accessor() << std::endl;

    result = ob1.or_op(ob2);  
    std::cout << "diz: " << result.accessor() << std::endl;

    result = ob1.srav_op(ob2);  
    std::cout << "impl: " << result.accessor() << std::endl;

    result = ob1.unsrav_op(ob2);  
    std::cout << "koimpl: " << result.accessor() << std::endl;

    result = ob1.sumabs_op(ob2);  
    std::cout << "sum: " << result.accessor() << std::endl;

    result = ob1.ekva_op(ob2);  
    std::cout << "ekva: " << result.accessor() << std::endl;

    result = ob1.pir_op(ob2);  
    std::cout << "pir: " << result.accessor() << std::endl;

    result = ob1.shiff_op(ob2);  
    std::cout << "shiff: " << result.accessor() << std::endl;

    logical_values_array::equals(ob1, ob2);

    std::cout << "Bit ob1: " << ob1.get_bit(3) << std::endl;
    std::cout << "Bit ob2: " << ob2.get_bit(2) << std::endl;

    std::cout << "Bit ob1: " << ob1.operator[](3) << std::endl;
    std::cout << "Bit ob2: " << ob2.operator[](3) << std::endl;

    char ob3[33];
    ob1.bybite(ob3);
    std::cout << "Binary representation of ob1: " << ob3 << std::endl;

    ob2.bybite(ob3);
    std::cout << "Binary representation of ob2: " << ob3 << std::endl;



    return 0;
}
