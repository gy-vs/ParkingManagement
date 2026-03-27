#ifndef STACK_H
#define STACK_H

#include <vector>
#include <stdexcept>
#include <iostream>

template <typename T>
class Stack {
private:
    std::vector<T> data;
    int capacity;

public:
    Stack(int cap = 10) : capacity(cap) {}

    bool isEmpty() const {
        return data.empty();
    }

    bool isFull() const {
        return data.size() >= static_cast<size_t>(capacity);
    }

    int size() const {
        return data.size();
    }

    void push(const T& item) {
        if (isFull()) {
            throw std::runtime_error("Stack is full");
        }
        data.push_back(item);
    }

    T pop() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        T item = data.back();
        data.pop_back();
        return item;
    }

    T& top() {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }

    const T& top() const {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return data.back();
    }

    void clear() {
        data.clear();
    }

    T& at(int index) {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& at(int index) const {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }
};

#endif
