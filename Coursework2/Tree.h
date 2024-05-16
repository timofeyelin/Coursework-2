#pragma once
#include <iostream>
using namespace std;

struct Node {
	int data;
	Node* left;
	Node* right;

	Node(int data) : data(data), left(nullptr), right(nullptr) {};
};

class Tree {
private:
	int size;
	Node* root;

	int index = 0;
	Node* deleteByMerging(Node*& node) {
		Node* tmp = node;
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
	}
	void clearNode(Node* node) {
		if (node == nullptr) return;
		clearNode(node->left);
		clearNode(node->right);
		delete node;
	}
	void prefixTraversal(Node* node, int* array) {
		if (node == nullptr) return;
		array[index++] = node->data;
		prefixTraversal(node->left, array);
		prefixTraversal(node->right, array);
	}

	void infixTraversal(Node* node, int* array) {
		if (node == nullptr) return;
		infixTraversal(node->left, array);
		array[index++] = node->data;
		infixTraversal(node->right, array);
	}

	void postfixTraversal(Node* node, int* array) {
		if (node == nullptr) return;
		postfixTraversal(node->left, array);
		postfixTraversal(node->right, array);
		array[index++] = node->data;
	}

	// Поворот влево относительно узла
	void rotateLeft(Node*& node) {
		if (!node || !node->right) return;
		Node* rightChild = node->right;
		node->right = rightChild->left;
		rightChild->left = node;
		node = rightChild;
	}

	// Поворот вправо относительно узла
	void rotateRight(Node*& node) {
		if (!node || !node->left) return;
		Node* leftChild = node->left;
		node->left = leftChild->right;
		leftChild->right = node;
		node = leftChild;
	}
	// Вспомогательная функция для балансировки
	Node* buildBalancedTree(int arr[], int start, int end) {
		if (start > end) {
			return nullptr;
		}
		int mid = (start + end) / 2;
		Node* node = new Node(arr[mid]);
		node->left = buildBalancedTree(arr, start, mid - 1);
		node->right = buildBalancedTree(arr, mid + 1, end);
		return node;
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

	// Поворот влево дерева вокруг узла с заданным значением
	void ToLeft(int value) {
		Node** node = &root;
		while (*node != nullptr && (*node)->data != value) {
			if (value < (*node)->data) {
				node = &((*node)->left);
			}
			else {
				node = &((*node)->right);
			}
		}
		if (*node != nullptr) {
			rotateLeft(*node);
		}
	}

	// Поворот вправо дерева вокруг узла с заданным значением
	void ToRight(int value) {
		Node** node = &root;
		while (*node != nullptr && (*node)->data != value) {
			if (value < (*node)->data) {
				node = &((*node)->left);
			}
			else {
				node = &((*node)->right);
			}
		}
		if (*node != nullptr) {
			rotateRight(*node);
		}
	}
	void add(int value) {
		if (contains(value)) {
			return;
		}
		Node* newNode = new Node(value);

		if (root == nullptr) {
			root = newNode;
			size++;
			return;
		}

		Node* current = root;

		while (true) {
			if (value < current->data) {
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

	void remove(int value) {
		Node* node = root, * prev = 0;
		while (node != nullptr) {
			if (node->data == value)
				break;
			prev = node;
			if (value < node->data)
				node = node->left;
			else node = node->right;
		}
		if (node != nullptr && node->data == value) {
			if (node == root)
				root = deleteByMerging(root);
			else if (prev->left == node)
				prev->left = deleteByMerging(prev->left);
			else
				prev->right = deleteByMerging(prev->right);
		}
	}
	bool contains(int value) {
		Node* current = root;

		while (current != nullptr) {
			if (value == current->data) {
				return true;
			}
			else if (value < current->data) {
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
	void Balance() {
		if (size == 0) return;
		int* elements = ToArray(Infix);
		clear();  // Очистка текущего дерева
		root = buildBalancedTree(elements, 0, size - 1);  // Перестраиваем дерево
		delete[] elements;  // Освобождаем память массива
	}


};