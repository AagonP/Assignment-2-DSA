#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class Node {
private:
	int key;
	Elem* data;
	Node* leftNode;
	Node* rightNode;
	int height;
public:
	Node(int key,Elem*data) {
		this->data = data;
		this->key = key;
		this->leftNode = NULL;
		this->rightNode = NULL;
		this->height = 1;
	}
	int getHeight() {
		return (this) ? height : 0;
	}
	void setHeight(int val) {
		this->height = val;
	}
	int getKey() {
		return this->key;
	}
	void setKey(int key) {
		this->key = key;
	}
	Node* getLeftNode() {
		return leftNode;
	}
	Node* getRightNode() {
		return rightNode;
	}
	void setLeft(Node* node) {
		this->leftNode = node;
	}
	void setRight(Node* node) {
		this->rightNode = node;
	}
	Data* getData() {
		return this->data->data;
	}
	Elem* getElem() {
		return this->data;
	}
};

class AVLtree {
	Node* root;
public:
	AVLtree() {
		root = NULL;
	}
	~AVLtree() {

	}
	Node* rotateRight(Node*);
	Node* rotateLeft(Node*);
	int getBalance(Node*);
	void updateNodeHeight(Node*);
	Node* insertNode(Node* root, int key,Elem*data);
	Node* deleteNode(Node* root, int key,Elem*delData);
	Node* getRoot() {
		return this->root;
	}
	void setRoot(Node*root) {
		this->root = root;
	}
	void print(const string& prefix, Node* node, bool isLeft)
	{	
		//Ulti
		cout << prefix;

		cout << (isLeft ? "|--" : "\\--");
		if (node != nullptr)
		{
			// print the value of the node
			cout << node->getKey() <<"("<< node->getData()->getValue()<<")" << endl;
			// enter the next tree level - left and right branch
			if (!node->getLeftNode() && !node->getRightNode()) return;
			print(prefix + (isLeft ? "|   " : "    "), node->getLeftNode(), true);
			print(prefix + (isLeft ? "|   " : "    "), node->getRightNode(), false);
		}
		else {
			cout << "N" << endl;
			return;
		}
	}
	void print()
	{	
		//Ulti
		print("", root, false);
	}
	Node* minValueNode(Node* node) {
		Node* curr = node;
		while (node->getLeftNode() != NULL) {
			curr = curr->getLeftNode();
		}
		return curr;
	}
	Elem* findData(int address,Node*root) {
		if (!root) return NULL;
		if (address > root->getKey()) {
			return findData(address, root->getRightNode());
		}
		else if (address < root->getKey()) {
			return findData(address, root->getLeftNode());
		}
		else {
			return root->getElem();
		}
	}
};

class Cache {
	queue<int> list;
	AVLtree* tree;
	int cacheSize = 0;
public:
	Cache(int s) {
		cacheSize = s;
		tree = new AVLtree();
	}
	~Cache() {
		delete tree;
	}
	Data* read(int addr);
	Elem* put(int addr, Data* cont);
	Elem* write(int addr, Data* cont);
	void print();
	void treePrint() {
		tree->print();
	}
	void preOrder();
	void inOrder();
	void queuePrint(queue<int> copyList) {
		while (!copyList.empty()) {
			Elem* data = tree->findData(copyList.front(),tree->getRoot());
			copyList.pop();
			cout << data->addr << " " << data->data->getValue() << " " << data->sync << endl;
		}
	}
	void prePrint(Node*node) {
		if (!node) return;
		Elem* data = node->getElem();
		cout << data->addr << " " << data->data->getValue() << " " << data->sync << endl;
		prePrint(node->getLeftNode());
		prePrint(node->getRightNode());
	}
	void inPrint(Node* node) {
		if (!node) return;
		inPrint(node->getLeftNode());
		Elem* data = node->getElem();
		cout << data->addr << " " << data->data->getValue() << " " << data->sync << endl;
		inPrint(node->getRightNode());
	}
};


#endif