#pragma once
#include <stdexcept>
#include <cmath>

template<typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(T data) : data(data), next(nullptr), prev(nullptr) {};
    };
    Node* head;
    Node* tail;
    Node* lastAccessedNode;
    int lastAccessedIndex;
    int size;

    Node* moveToIndex(Node* startNode, int startIndex, int endIndex) {
        Node* current = startNode;
        if (startIndex < endIndex) {
            for (int i = startIndex; i < endIndex; ++i) {
                current = current->next;
            }
        }
        else {
            for (int i = startIndex; i > endIndex; --i) {
                current = current->prev;
            }
        }
        return current;
    }

    Node* getIndex(int index) {
        if (index < 0 || index >= size) return nullptr;
        if (index == 0) return head;
        if (index == size - 1) return tail;

        Node* current = nullptr;
        int distanceFromStart = index;
        int distanceFromEnd = size - index - 1;
        int distanceFromLast = std::abs(lastAccessedIndex - index);

        if (lastAccessedNode && distanceFromLast <= distanceFromStart && distanceFromLast <= distanceFromEnd) {
            current = moveToIndex(lastAccessedNode, lastAccessedIndex, index);
        }
        else if (distanceFromStart <= distanceFromEnd) {
            current = moveToIndex(head, 0, index);
        }
        else {
            current = moveToIndex(tail, size - 1, index);
        }

        lastAccessedNode = current;
        lastAccessedIndex = index;
        return current;
    }

public:
    List() :head(nullptr), tail(nullptr), size(0), lastAccessedIndex(-1), lastAccessedNode(nullptr) {}
    ~List() {
        clear();
    }

    void add(T data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size++;
    }
    void insert(int index, T data) {
        if (index < 0 || index > size) return;

        Node* newNode = new Node(data);
        if (index == 0) {
            if (size == 0) {
                head = tail = newNode;
                size++;
                lastAccessedNode = newNode;
                lastAccessedIndex = 0;
                return;
            }
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        else if (index == size) {
            newNode->prev = tail;
            tail->next = newNode;
            tail = newNode;
        }
        else {
            Node* current = getIndex(index);
            newNode->next = current;
            newNode->prev = current->prev;
            current->prev->next = newNode;
            current->prev = newNode;
        }
        lastAccessedIndex = index;
        lastAccessedNode = newNode;
        size++;
    }
    void removeAt(int index) {
        if (index < 0 || index >= size) return;

        Node* toDelete;
        if (index == 0) {
            toDelete = head;
            head = head->next;
            if (head != nullptr) head->prev = nullptr;
            else tail = nullptr;
        }
        else if (index == size - 1) {
            toDelete = tail;
            tail = tail->prev;
            tail->next = nullptr;
        }
        else {
            toDelete = getIndex(index);
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }

        if (toDelete == lastAccessedNode) {
            lastAccessedNode = toDelete->prev;
            lastAccessedIndex = std::max(0, index - 1);
        }
        else if (index < lastAccessedIndex) {
            lastAccessedIndex--;
        }

        delete toDelete;
        size--;
        if (size == 0) {
            lastAccessedNode = nullptr;
            lastAccessedIndex = -1;
        }
    }
    T elementAt(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range");
        }
        // Если запрашиваемый индекс совпадает с последним обращенным, сразу возвращаем его данные
        if (lastAccessedIndex == index && lastAccessedNode != nullptr) {
            return lastAccessedNode->data;
        }
        return getIndex(index)->data;
    }
    T& elementAtRef(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index is out of range");
        }
        // Если запрашиваемый индекс совпадает с последним обращенным, сразу возвращаем его данные
        if (lastAccessedIndex == index && lastAccessedNode != nullptr) {
            return lastAccessedNode->data;
        }
        return getIndex(index)->data;
    }
    int count() {
        return size;
    }
    void clear() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        lastAccessedNode = nullptr;
        lastAccessedIndex = -1;
        size = 0;
    }
    void updateAt(int index, T newData) {
        Node* current = getIndex(index);
        if (current != nullptr) {
            current->data = newData;
            lastAccessedNode = current;
            lastAccessedIndex = index;
        }
    }

};