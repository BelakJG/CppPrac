#ifndef MY_STACK_HPP
#define MY_STACK_HPP
#include <cstddef>

template<typename T>
class MyStack {
public:
    MyStack(size_t capacity = 64);
    ~MyStack() noexcept;

    void resize(size_t capacity);
    void push(const T& entry);
    void push(T&& entry);
    T& top();
    const T& top() const;
    void pop();

    size_t size() const noexcept {return array_size;}
    bool empty() const noexcept {return array_size == 0;}
private:
    T* array = nullptr;
    size_t array_size = 0;
    size_t array_capacity;
};

#include "MyStack.tpp"
#endif