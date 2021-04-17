#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class Cache {
	queue<int> list;
	int cacheSize = 0;
public:
	Cache(int s) {
		cacheSize = s;
	}
	~Cache() {
	}
	Data* read(int addr);
	Elem* put(int addr, Data* cont);
	Elem* write(int addr, Data* cont);
	void print();
	void preOrder();
	void inOrder();
};


#endif