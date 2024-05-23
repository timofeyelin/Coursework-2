#pragma once
#include "LinkedList.h"
#include <string>

template<typename T>
class HashSet {
private:
    static const int size = 503;  // Размер хеш-таблицы
    List<T> table[size];    // Массив списков

    int hashFunction(std::string& key) {
        int sum = 0;
        for (int i = 0; i < key.length(); i++) {
            sum += key[i] * (i + 1);
        }
        return std::abs(sum % size);
    }

public:
    HashSet() {}
    ~HashSet() { clear(); }

    void add(T& value) {
        int index = hashFunction(value.FIO);
        if (!contains(value.FIO)) {
            table[index].add(value);
        }
    }

    void remove(std::string& key) {
        int index = hashFunction(key);
        for (int i = 0; i < table[index].count(); i++) {
            if (table[index].elementAt(i).FIO == key) {
                table[index].removeAt(i);
                return;
            }
        }
    }

    bool contains(std::string& key) {
        int index = hashFunction(key);
        for (int i = 0; i < table[index].count(); i++) {
            if (table[index].elementAt(i).FIO == key) {
                return true;
            }
        }
        return false;
    }

    void clear() {
        for (int i = 0; i < size; i++) {
            table[i].clear();
        }
    }

    int count() {
        int totalCount = 0;
        for (int i = 0; i < size; i++) {
            totalCount += table[i].count();
        }
        return totalCount;
    }

    T* ToArray() {
        int totalSize = count();
        T* array = new T[totalSize];
        int index = 0;
        for (int i = 0; i < size; ++i) {
            int listSize = table[i].count();
            for (int j = 0; j < listSize; ++j) {
                array[index++] = table[i].elementAt(j);
            }
        }
        return array;
    }
    T* findByEmail(const std::string& email) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < table[i].count(); ++j) {
                if (table[i].elementAt(j).email == email) {
                    return &table[i].elementAtRef(j);
                }
            }
        }
        return nullptr;
    }
};
