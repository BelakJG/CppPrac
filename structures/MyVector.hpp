#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP
#include <cstddef>

template <typename T>
class MyVector {
public:
    MyVector(size_t capacity = 64);
    ~MyVector();
    void reserve(size_t new_capacity);
    void push_back(T&& value);
    void clear();
    bool empty() const {return array_size == 0;}
    T& operator[](size_t index);
    const T&operator[](size_t index) const;
    size_t size() const {return array_size;}
    size_t capacity() const {return array_capacity;}

    MyVector(const MyVector&) = delete;
    MyVector& operator=(const MyVector&) = delete;
private:
    T* array = nullptr;
    size_t array_capacity;
    size_t array_size = 0;
};

#include "MyVector.tpp"
#endif