#ifndef MY_VECTOR_TPP
#define MY_VECTOR_TPP
#include "MyVector.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <new>

template <typename T>
MyVector<T>::MyVector(size_t capacity) : array_capacity(capacity) {
    array = static_cast<T*>(::operator new(sizeof(T) * capacity));
}

template <typename T>
MyVector<T>::~MyVector() {
    for (size_t i = 0; i < array_size; ++i) {
        array[i].~T();
    }

    ::operator delete(array);
}

template<typename T>
void MyVector<T>::reserve(size_t new_capacity) {
    if (new_capacity <= array_capacity) {
        return;
    }

    T* new_array = static_cast<T*>(::operator new(sizeof(T) * new_capacity));

    for (size_t i = 0; i < array_size; ++i) {
        new (&new_array[i]) T(std::move_if_noexcept(array[i]));
    }

    for (size_t i = 0; i < array_size; ++i) {
        array[i].~T();
    }
    ::operator delete(array);

    array = new_array;
    array_capacity = new_capacity;
}

template<typename T>
void MyVector<T>::push_back(T&& value) {
    if (array_size >= array_capacity) {
        reserve(array_capacity == 0 ? 1 : array_capacity * 2);
    }

    new (&array[array_size]) T(std::move_if_noexcept(value));
    ++array_size;
}

template<typename T>
void MyVector<T>::clear() {
    for (size_t i = 0; i < array_size; ++i) {
        array[i].~T();
    }

    array_size = 0;
}

template <typename T>
T& MyVector<T>::operator[](size_t index) {
    return array[index];
}
template <typename T>
const T& MyVector<T>::operator[](size_t index) const {
    return array[index];
}

#endif