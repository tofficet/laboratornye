#include "../include/int_wrapper.h"

void int_wrapper::dispose()
{
    delete[] _value_ptr;
    _value_ptr= nullptr;
}
