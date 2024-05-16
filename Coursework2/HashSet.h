#pragma once
#include <string>
#include "LinkedList.h"

class HashSet {
private:
    static const int size = 503;  // Размер хеш-таблицы
    List<std::string> table[size];  // Массив списков
    int hashFunction(const std::string& value);  // Функция хеширования

public:
    HashSet();
    ~HashSet();
    void add(const std::string& value);
    void remove(const std::string& value);
    bool contains(const std::string& value);
    void clear();
    int count();
    std::string* ToArray();  // Возвращает массив всех элементов хеш-множества
};
HashSet::HashSet() {

}

HashSet::~HashSet() {
    clear();
}

int HashSet::hashFunction(const std::string& value) {
    int sum = 0;
    for (int i = 0; i < value.length(); i++) {
        sum += value[i] * (i + 1);
    }
    return abs(sum % size);
}


void HashSet::add(const std::string& value) {
    int index = hashFunction(value);
    if (!contains(value)) {
        table[index].add(value);
    }
}

void HashSet::remove(const std::string& value) {
    int index = hashFunction(value);
    for (int i = 0; i < table[index].count(); i++) {
        if (table[index].elementAt(i) == value) {
            table[index].removeAt(i);
            return;
        }
    }
}

bool HashSet::contains(const std::string& value) {
    int index = hashFunction(value);
    for (int i = 0; i < table[index].count(); i++) {
        if (table[index].elementAt(i) == value) {
            return true;
        }
    }
    return false;
}

void HashSet::clear() {
    for (int i = 0; i < size; i++) {
        table[i].clear();
    }
}

int HashSet::count() {
    int totalCount = 0;
    for (int i = 0; i < size; i++) {
        totalCount += table[i].count();
    }
    return totalCount;
}

std::string* HashSet::ToArray() {
    int totalSize = count();
    std::string* array = new std::string[totalSize];
    int index = 0;
    for (int i = 0; i < size; ++i) {
        int listSize = table[i].count();
        for (int j = 0; j < listSize; ++j) {
            array[index++] = table[i].elementAt(j);
        }
    }
    return array;
}