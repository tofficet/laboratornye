// #include <iostream>

// class T
// {

// private:

//     void copy(
//         T const &from)
//     {
//         _array_size = from._array_size;
//         _array = (int *)malloc(sizeof(int) * _array_size);
//         if (_array == nullptr)
//         {
//             // TODO: throw an exception
//         }

//         memcpy(_array, from._array, sizeof(int) * _array_size);
//     }

//     void dispose()
//     {
//         std::cout << "Destructor called" << std::endl;
//         free(_array);
//         _array = nullptr;
//     }

// private:

//     int *_array;
//     size_t _array_size;

// public:

//     T(
//         T const &obj)
//     {
//         copy(obj);
//     }

//     T(
//         int const *values,
//         size_t values_count):
//             _array_size(values_count)
//     {
//         _array = (int *)malloc(sizeof(int) * values_count);
//         if (_array == nullptr)
//         {
//             // TODO: throw an exception
//         }

//         memcpy(_array, values, sizeof(int) * values_count);
//     }

//     ~T()
//     {
//         dispose();
//     }

//     // a = b -> a.operator=(&b) -> operator=(&a, &b);
//     // c = (a = b)

//     T &operator=(
//         T const &obj)
//     {
//         // 0. check equality of "this" pointer and param address
//         // 1. dispose current object old state
//         // 2. copy param state into current object
//         // 3. return calling instance ref as retval - return *this;

//         if (this == &obj)
//         {
//             return *this;
//         }

//         dispose();
//         copy(obj);

//         return *this;
//     }

//     T &foo()
//     {
//         std::cout << this << std::endl;
//         int arr[10] = { 0 };
//         T result(arr, 10);
//         return result;
//     }

// };

// class X
// {

// };

// class Y
// {
// public:
//     Y(X obj)
//     {

//     }
// };

// X foo(X obj)
// {
//     return obj;
// }

// class A
// {

// private:

//     int *_a;

// public:

//     A():
//         _a(new int[100])
//     {
//         std::cout << "A::A()" << std::endl;
//     }

//     virtual ~A()
//     {
//         std::cout << "A::~A()" << std::endl;
//         delete[] _a;
//         _a = nullptr;
//     }

//     A(
//         A const &other):
//             _a(new int[100])
//     {
//         std::cout << "A::A(A const &)" << std::endl;

//         memcpy(_a, other._a, sizeof(int) * 100);
//     }

//     A &operator=(
//         A const &other)
//     {
//         std::cout << "A &A::operator=(A const &)" << std::endl;

//         if (this == &other)
//         {
//             std::cout << "Self assignment detected!!1!1" << std::endl;

//             return *this;
//         }

//         memcpy(_a, other._a, sizeof(int) * 100);

//         return *this;
//     }

//     A(
//         A &&other) noexcept:
//             _a(other._a)
//     {
//         std::cout << "A::A(A &&)" << std::endl;

//         other._a = nullptr;
//     }

//     A &operator=(
//         A &&other) noexcept
//     {
//         std::cout << "A &A::operator=(A &&)" << std::endl;

//         if (this == &other)
//         {
//             std::cout << "Self assignment detected!!1!1" << std::endl;

//             return *this;
//         }

//         delete [] _a;
//         _a = other._a;
//         other._a = nullptr;

//         return *this;
//     }

// public:

//     virtual void foo()
//     {
//         std::cout << "A::foo()" << std::endl;
//     }

// };

// class B:
//     public A
// {
// private:
//     int *_b;
// public:
//     B(): _b(new int[100]) { std::cout << "B::ctor" << std::endl; }
//     ~B() { delete[] _b; _b = nullptr; std::cout << "B::dtor" << std::endl; }
//     B(B const &other) {  }
//     B &operator=(B const &other) { return *this; }
//     B(B &&other) noexcept {  }
//     B &operator=(B &&other) noexcept { return *this; }
// public:
//     void foo() override { std::cout << "B::foo()" << std::endl; }
// };

// class C:
//     public B
// {
// private:
//     int *_c;
// public:
//     C(): _c(new int[100]) { std::cout << "C::ctor" << std::endl; }
//     ~C() { delete[] _c; _c = nullptr; std::cout << "C::dtor" << std::endl; }
//     C(C const &other) {  }
//     C &operator=(C const &other) { return *this; }
//     C(C &&other) noexcept {  }
//     C &operator=(C &&other) noexcept { return *this; }
// public:
//     void foo() override { std::cout << "C::foo()" << std::endl; }
// };

// void foo(
//     A *ptr)
// {
//     ptr->foo();
// }

// void punjk()
// {
//     B obj;
//     foo(&obj);
// }

// class repeat
// {

//     ~repeat();

//     repeat(
//         repeat const &x);

//     repeat &operator=(
//         repeat const &puk);

// };

// void rule_of_5_demo()
// {
//     A obj;
//     A copy = obj;
//     A obj1 = std::move(copy);
// }

// int x = 10;

// int &bar()
// {
//     return x;
// }

// int main()
// {
//     // x = x;
//     // x = 10;
//     // 10 = x;
//     // bar() = 5;
//     // rule_of_5_demo();

//     // {
//     //     A *obj_ptr = new C();
// //
//     //     delete obj_ptr;
//     // }

//     return 0;

//     A* a = new A();
//     a->foo();
//     delete a;

//     a = new B();
//     a->foo();
//     delete a;

//     a = new C();
//     a->foo();
//     delete a;

//     return 0;

//     X obj;
//     Y yObj = obj;
//     X obj2 = obj;
//     obj2 = obj;
//     obj = obj2;

//     int arr[10] = { 1, 2, 3, 7, 14, 23, -5, 6, 14, 23 };

//     T x(arr + 4, 5);

//     {
//         T y = x;
//     }

//     x.foo();

//     std::cout << "Hello, World!" << std::endl;
//     return 0;
// }

#include "../derivative/include/int_wrapper.h"
#include <iostream>
#include <cassert>

void test_arithmetic_operators() {
    std::cout << "\n=== Testing Arithmetic Operators ===\n";
    
    int_wrapper a(10);
    int_wrapper b(3);
    
    // Test basic arithmetic
    std::cout << a << " + " << b << " = " << (a + b) << "\n";
    std::cout << a << " - " << b << " = " << (a - b) << "\n";
    std::cout << a << " * " << b << " = " << (a * b) << "\n";
    std::cout << a << " / " << b << " = " << (a / b) << "\n";
    std::cout << a << " % " << b << " = " << (a % b) << "\n";
    
    // Test compound assignment
    a += b;
    std::cout << "After a += b: " << a << "\n";
    a -= b;
    std::cout << "After a -= b: " << a << "\n";
    a *= b;
    std::cout << "After a *= b: " << a << "\n";
    a /= b;
    std::cout << "After a /= b: " << a << "\n";
    
    // Test increment/decrement
    std::cout << "a++: " << a++ << " (now: " << a << ")\n";
    std::cout << "++a: " << ++a << "\n";
    std::cout << "a--: " << a-- << " (now: " << a << ")\n";
    std::cout << "--a: " << --a << "\n";
}

void test_bitwise_operators() {
    std::cout << "\n=== Testing Bitwise Operators ===\n";
    
    int_wrapper a(0b1100);
    int_wrapper b(0b1010);
    
    std::cout << a << " & " << b << " = " << (a & b) << "\n";
    std::cout << a << " | " << b << " = " << (a | b) << "\n";    
    std::cout << a << " ^ " << b << " = " << (a ^ b) << "\n";
    std::cout << a << " << 1 = " << (a << int_wrapper(1)) << "\n";
    std::cout << a << " >> 1 = " << (a >> int_wrapper(1)) << "\n";
    
    // Test compound assignment
    a &= b;
    std::cout << "After a &= b: " << a << "\n";
    a |= b;
    std::cout << "After a |= b: " << a << "\n";
    a ^= b;
    std::cout << "After a ^= b: " << a << "\n";
    a <<= int_wrapper(1);
    std::cout << "After a <<= 1: " << a << "\n";
    a >>= int_wrapper(1);
    std::cout << "After a >>= 1: " << a << "\n";
}

void test_comparison_operators() {
    std::cout << "\n=== Testing Comparison Operators ===\n";
    
    int_wrapper a(5);
    int_wrapper b(5);
    int_wrapper c(10);
    
    std::cout << a << " == " << b << ": " << (a == b) << "\n";
    std::cout << a << " != " << c << ": " << (a != c) << "\n";
    std::cout << a << " < " << c << ": " << (a < c) << "\n";
    std::cout << a << " <= " << b << ": " << (a <= b) << "\n";
    std::cout << c << " > " << a << ": " << (c > a) << "\n";
    std::cout << b << " >= " << a << ": " << (b >= a) << "\n";
}

void test_copy_move_semantics() {
    std::cout << "\n=== Testing Copy/Move Semantics ===\n";
    
    int_wrapper a(42);
    int_wrapper b = a;  // copy constructor
    int_wrapper c;
    c = a;              // copy assignment
    
    std::cout << "Original: " << a << "\n";
    std::cout << "Copy constructed: " << b << "\n";
    std::cout << "Copy assigned: " << c << "\n";
    
    int_wrapper d = std::move(a);  // move constructor
    int_wrapper e;
    e = std::move(b);              // move assignment
    
    std::cout << "After move:\n";
    std::cout << "a: " << a << " (should be 0)\n";
    std::cout << "b: " << b << " (should be 0)\n";
    std::cout << "d (moved from a): " << d << "\n";
    std::cout << "e (moved from b): " << e << "\n";
}

void test_stream_operators() {
    std::cout << "\n=== Testing Stream Operators ===\n";
    
    int_wrapper a;
    std::cout << "Enter an integer: ";
    std::cin >> a;
    std::cout << "You entered: " << a << "\n";
}

int main() {
    std::cout << "=== Testing int_wrapper class ===\n";
    
    test_arithmetic_operators();
    test_bitwise_operators();
    test_comparison_operators();
    test_copy_move_semantics();
    test_stream_operators();
    
    std::cout << "\nAll tests completed!\n";
    return 0;
}