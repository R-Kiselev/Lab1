#pragma once
#include <vector>
#include "Errors/CustomExceptions.h"

template <typename T>
class Iter {
private:
    T* ptr;

public:
    explicit Iter(T* ptr) : ptr(ptr) {}

    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }

    Iter& operator++() {
        ++ptr;
        return *this;
    }

    Iter operator++(int) {
        Iter temp = *this;
        ++ptr;
        return temp;
    }

    bool operator==(const Iter& other) const { return ptr == other.ptr; }
    bool operator!=(const Iter& other) const { return ptr != other.ptr; }
};

template <typename T>
class list {
private:
    std::vector<T> data;

public:
    list() = default;

    void add(T&& value) {
        data.push_back(std::move(value));
    }

    void remove(size_t index) {
        if (index >= data.size()) {
            throw ListException("Index out of range");
        }
        data.erase(data.begin() + index);
    }

    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    T& operator[](size_t index) {
        if (index >= data.size()) {
            throw ListException("Index out of range");
        }
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= data.size()) {
            throw ListException("Index out of range");
        }
        return data[index];
    }

    using Iterator = Iter<T>;

    Iterator begin() { return Iterator(data.data()); }
    Iterator end() { return Iterator(data.data() + data.size()); }
};
