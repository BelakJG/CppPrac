#ifndef MY_VECTOR_TPP
#define MY_VECTOR_TPP
#include "MyVector.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <new>

template<typename T>
MyVector<T>::MyVector(size_t capacity) : array_capacity(capacity) {
    array = static_cast<T*>(::operator new(sizeof(T) * capacity));
}

template<typename T>
MyVector<T>::~MyVector() {
    for (size_t i = 0; i < array_size; ++i) {
        array[i].~T();
    }

    ::operator delete(array);
}

template<typename T>
MyVector<T>::MyVector(const MyVector &other) : array_capacity(other.array_capacity), array_size(other.array_size) {
    array = static_cast<T*>(::operator new(sizeof(T) * array_capacity));

    size_t i = 0;
    try {
        for (; i < array_size; ++i) {
            new (&array[i]) T(other.array[i]);
        }
    } catch (...) {
        for (size_t j = 0; j < i; ++j) {
            array[j].~T();
        }
        ::operator delete(array);
        throw;
    }
}
template<typename T>
MyVector<T>& MyVector<T>::operator=(const MyVector &other) {
    if (this == &other) return *this;

    MyVector<T> temp(other);

    std::swap(array, temp.array);
    std::swap(array_capacity, temp.array_capacity);
    std::swap(array_size, temp.array_size);

    return *this;
}
template<typename T>
MyVector<T>::MyVector(MyVector&& other) noexcept : array(other.array), array_size(other.array_size), array_capacity(other.array_capacity) {
    other.array = nullptr;
    other.array_capacity = 0;
    other.array_size = 0;
}
template<typename T>
MyVector<T>& MyVector<T>::operator=(MyVector&& other) noexcept {
    if (this == &other) return *this;

    clear();
    ::operator delete(array);

    array = other.array;
    array_size = other.array_size;
    array_capacity = other.array_capacity;

    other.array = nullptr;
    other.array_capacity = 0;
    other.array_size = 0;

    return *this;
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
void MyVector<T>::push_back(const T& value) {
    if (array_size >= array_capacity) {
        reserve(array_capacity == 0 ? 1 : array_capacity * 2);
    }

    new (&array[array_size]) T(value);
    ++array_size;
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