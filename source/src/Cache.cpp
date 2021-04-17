#include "main.h"
#include "Cache.h"

bool isExistInCache();

Data* Cache::read(int addr) {
    // returns the data stored at the addresss addr if
    // the addr is kept in the cache, otherwise, return NULL.
    if (isExistInCache()) {
        Elem* returnElem;
        return returnElem->data;
    }
    else {
        return NULL;
    }
}
Elem* Cache::put(int addr, Data* cont) {
    // put addr and data into the cache
    // and return the element which is remove out of the
    // cache if any, or NULL otherwise.
    if (list.size() == cacheSize) {
        // Remove the address from the queue
        int removeAddress = list.front();
        list.pop();
        // Remove it from the AVL tree;
        // avl.delete(addr);
        Elem *returnElem;
        // Add new address into queue
        list.push(addr);
        // Add new address, Elem into AVL
        // avl.insert(addr,elem);
        return returnElem;
    }
    else {
        // Add new address into queue
        list.push(addr);
        // Add new address, Elem into AVL
        // avl.insert(addr,elem);
        return NULL;
    }
}
Elem* Cache::write(int addr, Data* cont) {
    // searches if the addr is in the cache. If it is, this method 
    // replaces the existing value associated with the addr with 
    // the cont. If not, this method puts the addr and cont into the cache. 
    // This method returns the element removed out of the cache
    // if any, otherwise, returns NULL.
    if (isExistInCache()) {
        Elem* returnElem;
        returnElem->data = cont;
    }
    else {
        if (list.size() == cacheSize) {
            // Remove the address from the queue
            int removeAddress = list.front();
            list.pop();
            // Remove it from the AVL tree;
            // avl.delete(addr);
            Elem* returnElem;
            // Add new address into queue
            list.push(addr);
            // Add new address, Elem into AVL
            // avl.insert(addr,new Elem(addr,cont,false));
            return returnElem;
        }
        else {
            // Add new address into queue
            list.push(addr);
            // Add new address, Elem into AVL
            // avl.insert(addr,new Elem(addr,cont,false));
            return NULL;
        }
    }
}
void Cache::print() {
    // prints the value of the elements in the cache in the descending 
    // order of the element’s living time.Each element must be printed 
    // by the method print of class Elem.
}
void Cache::preOrder() {
    // prints the value of the elements in the cache 
    // in the preorder of the AVL tree which must be 
    // used to search an address in the cache.    
}
void Cache::inOrder() {
    // prints the value of the elements in the cache 
    // in the inorder of the AVL tree which must be 
    // used to search an address in the cache. 
}




// A queue will store the addresses put into the cache
// So when we remove an address it will be the first one to enter
// Then we search it in the AVL tree and remove that node
// AVL tree will hold the Elem(), addr

// Can we just use the AVL tree to store Elem(), addr and have some kind of variable that store the live time ?
// Then when we want to remove an address that lives the longest we have to search a whole tree ( since AVL is a BST, nodes
// do not stored with the key as livetime but address), so in the worst case  we have
// to search the whole tree since the livetime is randomly stored ? -> a tree with N nodes will need O(n) ( a queue.pop() would only take O(1))
// if we stored it as queue then we search the address it only takes O(logn)