#include "../include/binary_queue.h"
#include <cstring>
#include <stdexcept>
#include <utility>

binary_queue::binary_queue(int n=16) : 
    _data_arr(new pq_node[n]),
    _size(0),
    _capacity(n)
{}


binary_queue::binary_queue(const binary_queue& other) :
    _data_arr(new pq_node[other._capacity]),
    _size(other._size),
    _capacity(other._capacity)
{
    for (int i = 0; i < _size; ++i) {
        _data_arr[i].key = other._data_arr[i].key;
        
        if (other._data_arr[i].data) {
            _data_arr[i].data = new char[std::strlen(other._data_arr[i].data) + 1];
            std::strcpy(_data_arr[i].data, other._data_arr[i].data);
        } else {
            _data_arr[i].data = nullptr;
        }
    }
}

binary_queue::~binary_queue() {
    for (int i = 0; i < _size; ++i) {
        delete[] _data_arr[i].data;
    }
    delete[] _data_arr;
}


void binary_queue::swap(pq_node& a, pq_node& b) {
    std::swap(a.data, b.data);
    std::swap(a.key, b.key);
}

binary_queue& binary_queue::operator=(const binary_queue& other) {
    if (this != &other) {
        binary_queue temp(other);
        std::swap(*this, temp);
    }
    return *this;
}


void binary_queue::insert(const char* data, int key) {
    if (_size >= _capacity) {
        resize();
    }
    _data_arr[_size].data = new char[std::strlen(data) + 1];
    std::strcpy(_data_arr[_size].data, data);
    _data_arr[_size].key = key;
    sift_up(_size);
    _size++;
}

char* binary_queue::find_max() {
    if (_size == 0) {
        throw std::runtime_error("Queue is empty");
    }
    return _data_arr[0].data;
}

char* binary_queue::remove_max() {
    if (_size == 0) {
        throw std::runtime_error("Queue is empty");
    }

    char* max_data = _data_arr[0].data;
    char* data_copy = new char[std::strlen(max_data) + 1];
    std::strcpy(data_copy, max_data);
    delete[] max_data;

    _data_arr[0] = _data_arr[--_size];
    sift_down(0);

    return data_copy;
}

priority_queue* binary_queue::merge(priority_queue* q) {
    binary_queue* other = dynamic_cast<binary_queue*>(q);
    if (!other) {
        throw std::invalid_argument("Invalid queue type for merge");
    }

    for (int i = 0; i < other->_size; ++i) {
        char* data = new char[std::strlen(other->_data_arr[i].data) + 1];
        std::strcpy(data, other->_data_arr[i].data);
        insert(data, other->_data_arr[i].key);
        delete[] data;
    }
    return this;
}

// Private utility methods
void binary_queue::sift_up(int index) {
    while (index > 0 && _data_arr[(index - 1) / 2].key < _data_arr[index].key) {
        swap(_data_arr[(index - 1) / 2], _data_arr[index]);
        index = (index - 1) / 2;
    }
}

void binary_queue::sift_down(int index) {
    while (2 * index + 1 < _size) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int largest = index;

        if (left < _size && _data_arr[left].key > _data_arr[largest].key) {
            largest = left;
        }
        if (right < _size && _data_arr[right].key > _data_arr[largest].key) {
            largest = right;
        }
        if (largest == index) {
            break;
        }
        swap(_data_arr[index], _data_arr[largest]);
        index = largest;
    }
}

void binary_queue::resize() {
    _capacity *= 2;
    pq_node* new_arr = new pq_node[_capacity];
    
    for (int i = 0; i < _size; ++i) {
        new_arr[i].key = _data_arr[i].key;
        if (_data_arr[i].data) {
            new_arr[i].data = new char[std::strlen(_data_arr[i].data) + 1];
            std::strcpy(new_arr[i].data, _data_arr[i].data);
            delete[] _data_arr[i].data;
        } else {
            new_arr[i].data = nullptr;
        }
    }
    
    delete[] _data_arr;
    _data_arr = new_arr;
}

int binary_queue::get_max_priority() {
    if (_size == 0) {
        throw std::runtime_error("Queue is empty");
    }
    return _data_arr[0].key;
}