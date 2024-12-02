#pragma once
#include <vector>
#include "Errors/CustomExceptions.h"

// Класс-итератор
template <typename T>
class MyIterator {
private:
    T* ptr;

public:
    explicit MyIterator(T* ptr) : ptr(ptr) {}

    // Доступ к элементу
    T& operator*() { return *ptr; }
    T* operator->() { return ptr; }

    // Префиксный инкремент
    MyIterator& operator++() {
        ++ptr;
        return *this;
    }

    // Постфиксный инкремент
    MyIterator operator++(int) {
        MyIterator temp = *this;
        ++ptr;
        return temp;
    }

    // Операторы сравнения
    bool operator==(const MyIterator& other) const { return ptr == other.ptr; }
    bool operator!=(const MyIterator& other) const { return ptr != other.ptr; }
};

// Класс-контейнер
template <typename T>
class list {
private:
    std::vector<T> data;  // Используем вектор для хранения элементов

public:
    list() = default;

    // Добавление элемента
    void add(T&& value) {
        data.push_back(std::move(value));
    }


    // Удаление элемента по индексу
    void remove(size_t index) {
        if (index >= data.size()) {
            throw ListException("Index out of range");
        }
        data.erase(data.begin() + index);
    }

    // Получение размера
    size_t size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }
    // Оператор доступа по индексу
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

    // Тип итератора
    using Iterator = MyIterator<T>;

    // Методы для получения итераторов
    Iterator begin() { return Iterator(data.data()); }
    Iterator end() { return Iterator(data.data() + data.size()); }
};
