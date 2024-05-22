#pragma once
#include <iostream>
using namespace std;

template <typename T>
struct Node {
    int key; // Используем номер группы как ключ
    T data; // Данные группы
    Node* left;
    Node* right;

    Node(int key, T data) : key(key), data(data), left(nullptr), right(nullptr) {}
};

template <typename T>
class Tree {
private:
    int size;
    Node<T>* root;

    int index = 0;
    Node<T>* deleteByMerging(Node<T>*& node) {
        Node<T>* tmp = node;
        if (node != nullptr) {
            if (!node->right) node = node->left;
            else if (node->left == nullptr) node = node->right;
            else {
                tmp = node->left;
                while (tmp->right != nullptr) tmp = tmp->right;
                tmp->right = node->right;
                tmp = node;
                node = node->left;
            }
            delete tmp;
            size--;
            return node;
        }
        return nullptr;
    }
    void clearNode(Node<T>* node) {
        if (node == nullptr) return;
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }
    void prefixTraversal(Node<T>* node, int* array) {
        if (node == nullptr) return;
        array[index++] = node->key;
        prefixTraversal(node->left, array);
        prefixTraversal(node->right, array);
    }

    void infixTraversal(Node<T>* node, int* array) {
        if (node == nullptr) return;
        infixTraversal(node->left, array);
        array[index++] = node->key;
        infixTraversal(node->right, array);
    }

    void postfixTraversal(Node<T>* node, int* array) {
        if (node == nullptr) return;
        postfixTraversal(node->left, array);
        postfixTraversal(node->right, array);
        array[index++] = node->key;
    }

public:
    Tree() : size(0), root(nullptr) {};
    ~Tree() {
        clear();
    }
    enum Order { Prefix, Infix, Postfix };
    int* ToArray(Order order) {
        int* array = new int[size];
        index = 0;
        switch (order) {
        case Prefix:
            prefixTraversal(root, array);
            break;
        case Infix:
            infixTraversal(root, array);
            break;
        case Postfix:
            postfixTraversal(root, array);
            break;
        }
        return array;
    }
    int* ToArray() {
        return ToArray(Infix);  // Перегрузка без параметров, использующая Infix
    }

    void add(int key, T value) {
        if (contains(key)) {
            return;
        }
        Node<T>* newNode = new Node<T>(key, value);

        if (root == nullptr) {
            root = newNode;
            size++;
            return;
        }

        Node<T>* current = root;

        while (true) {
            if (key < current->key) {
                if (current->left == nullptr) {
                    current->left = newNode;
                    size++;
                    break;
                }
                current = current->left;
            }
            else {
                if (current->right == nullptr) {
                    current->right = newNode;
                    size++;
                    break;
                }
                current = current->right;
            }
        }
    }

    void remove(int key) {
        Node<T>* node = root, * prev = 0;
        while (node != nullptr) {
            if (node->key == key)
                break;
            prev = node;
            if (key < node->key)
                node = node->left;
            else node = node->right;
        }
        if (node != nullptr && node->key == key) {
            if (node == root)
                root = deleteByMerging(root);
            else if (prev->left == node)
                prev->left = deleteByMerging(prev->left);
            else
                prev->right = deleteByMerging(prev->right);
        }
    }

    bool contains(int key) {
        Node<T>* current = root;

        while (current != nullptr) {
            if (key == current->key) {
                return true;
            }
            else if (key < current->key) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        return false;
    }

    int count() {
        return size;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
        size = 0;
    }

    Node<T>* getRoot() {
        return root;
    }
};