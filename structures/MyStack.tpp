#ifndef MY_STACK_TPP
#define MY_STACK_TPP
#include "MyStack.hpp"
#include <new>
#include <cstddef>
#include <utility>
#include <stdexcept>

template<typename T>
MyStack<T>::MyStack(size_t capacity) : array_capacity(capacity) {
    array = static_cast<T*>(::operator new(sizeof(T) * array_capacity));
}
template<typename T>
MyStack<T>::~MyStack() noexcept {
    for (size_t i = 0; i < array_size; ++i) {
        array[i].~T();
    }
    ::operator delete(array);
}

template<typename T>
void MyStack<T>::resize(size_t capacity) {
    if (capacity < array_size) {
        throw std::runtime_error("Can't shrink below amount of stacked items");
    }
    if (capacity == array_capacity) return;

    T* new_array = static_cast<T*>(::operator new(sizeof(T) * capacity));
    size_t copied = 0;
    try {
        for (; copied < array_size; ++copied) {
            new (&new_array[copied]) T(std::move_if_noexcept(array[copied]));
        }
    } catch (...) {
        for (size_t i = 0; i < copied; ++i) {
            new_array[i].~T();
        }
        ::operator delete(new_array);
        throw;
    }

    for (size_t i = 0; i < array_size; ++i) {
        array[i].~T();
    }
    ::operator delete(array);

    array = new_array;
    array_capacity = capacity;
}

template<typename T>
void MyStack<T>::push(const T& entry) {
    if (array_size >= array_capacity) resize(array_capacity == 0 ? 1 : array_capacity * 2);
    new (&array[array_size]) T(entry);
    ++array_size;
}
template<typename T>
void MyStack<T>::push(T&& entry) {
    if (array_size >= array_capacity) resize(array_capacity == 0 ? 1 : array_capacity * 2);
    new (&array[array_size]) T(std::move_if_noexcept(entry));
    ++array_size;
}

template<typename T>
T& MyStack<T>::top() {
    if (array_size == 0) throw std::runtime_error("Stack is empty");
    return array[array_size - 1];
}
template <typename T>
const T& MyStack<T>::top() const {
    if (array_size == 0) throw std::runtime_error("Stack is empty");
    return array[array_size - 1];
}

template<typename T>
void MyStack<T>::pop() {
    if (array_size == 0) throw std::runtime_error("Stack is empty");
    --array_size;
    array[array_size].~T();
}

#endif