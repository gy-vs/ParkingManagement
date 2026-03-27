#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <stdexcept>
#include <vector>

template <typename T>
class Queue {
private:
    std::queue<T> data;

public:
    Queue() {}

    bool isEmpty() const {
        return data.empty();
    }

    int size() const {
        return data.size();
    }

    void enqueue(const T& item) {
        data.push(item);
    }

    T dequeue() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        T item = data.front();
        data.pop();
        return item;
    }

    T& front() {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return data.front();
    }

    const T& front() const {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return data.front();
    }

    void clear() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    std::vector<T> toVector() const {
        std::queue<T> temp = data;
        std::vector<T> result;
        while (!temp.empty()) {
            result.push_back(temp.front());
            temp.pop();
        }
        return result;
    }
};

#endif
