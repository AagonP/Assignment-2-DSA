#include "main.h"
#include "Cache.h"

void AVLtree::updateNodeHeight(Node* root) {
    if (!root) cout << "Update node height function is passed wrongly!";
    int leftNodeHeight = root->getLeftNode()->getHeight();
    int rightNodeHeight = root->getRightNode()->getHeight();
    root->setHeight((leftNodeHeight > rightNodeHeight) ? leftNodeHeight+1 : rightNodeHeight+1);
}

int AVLtree::getBalance(Node* root) {
    // Left - right
    return (root) ? root->getLeftNode()->getHeight() - root->getRightNode()->getHeight() : 0 ;
}

Node* AVLtree::rotateRight(Node* root) {
    Node* temp = root->getLeftNode()->getRightNode();
    root->getLeftNode()->setRight(root);
    root = root->getLeftNode();
    root->getRightNode()->setLeft(temp);
    updateNodeHeight(root->getRightNode());
    updateNodeHeight(root);
    return root;
}

Node* AVLtree::rotateLeft(Node* root) {
    Node* temp = root->getRightNode()->getLeftNode();
    root->getRightNode()->setLeft(root);
    root = root->getRightNode();
    root->getLeftNode()->setRight(temp);
    updateNodeHeight(root->getLeftNode());
    updateNodeHeight(root);
    return root;
}

Node* AVLtree::insertNode(Node*root,int key,Elem*data) {
    // First insert as a normal BST tree
    if (!root) {
       /* if (key == 3) {
            cout << "Insert!" << endl;
        }*/
        return new Node(key,data);
    }
    else {
        int rootKey = root->getKey();
        Node* rightNode = root->getRightNode();
        Node* leftNode = root->getLeftNode();
        if (key >= rootKey) {
            // Go right
            root->setRight(insertNode(rightNode, key,data));
        }
        else {
            // Go left
            root->setLeft(insertNode(leftNode, key,data));
        }
        // Update the height of the parents
        updateNodeHeight(root);
        // Check if the AVL property is stil hold
        int balanceFactor = getBalance(root);
        if (balanceFactor > 1 && key < leftNode->getKey()) {
            //Left-Left
            return rotateRight(root);
        }
        else if (balanceFactor > 1 && key >= leftNode->getKey()) {
            //Left-Right
            root->setLeft(rotateLeft(root->getLeftNode()));
            return rotateRight(root);
        }
        else if ( balanceFactor < -1 && key >= rightNode->getKey()) {
            //Right-Right
            return rotateLeft(root);
        }
        else if (balanceFactor < -1 && key < rightNode->getKey()) {
            //Right-Left
            root->setRight(rotateRight(root->getRightNode()));
            return rotateLeft(root);
        }
    }
    // As the recursion unwinds up the tree
    // perform rotation on any node that is unbalanced
    return root;
}

Node* AVLtree::deleteNode(Node* root, int key, Elem* delData) {
    if (!root) {
        return root;
    }
    if (key < root->getKey()) {
        root->setLeft(deleteNode(root->getLeftNode(), key, delData));
    }
    else if (key > root->getKey()) {
        root->setRight(deleteNode(root->getRightNode(), key, delData));
    }
    else {
        if (!root->getLeftNode() || !root->getRightNode()) {
            Node* temp = root->getLeftNode() ?  root->getLeftNode() : root->getRightNode() ;
            if (!temp) {              
                temp = root;
                root = NULL;
            }
            else {          
                *root = *temp;
            }
            delData->data = temp->getData();
            delete temp;
        }
        else {
            Node* temp = minValueNode(root->getRightNode());
            root->setKey(temp->getKey());
            root->setRight(deleteNode(root->getRightNode(), temp->getKey(),delData));
        }
    }
    if (!root) return root;
    root->setHeight(root->getLeftNode()->getHeight() > root->getRightNode()->getHeight() ? root->getLeftNode()->getHeight() + 1 : root->getRightNode()->getHeight() + 1);
    // Check if the AVL property is stil hold
    int balanceFactor = getBalance(root);
    Node* rightNode = root->getRightNode();
    Node* leftNode = root->getLeftNode();
    if (balanceFactor > 1 && getBalance(root->getLeftNode()) >= 0) {
        //Left-Left
        return rotateRight(root);
    }
    else if (balanceFactor > 1 && getBalance(root->getLeftNode()) < 0) {
        //Left-Right
        root->setLeft(rotateLeft(root->getLeftNode()));
        return rotateRight(root);
    }
    else if (balanceFactor < -1 && getBalance(root->getRightNode()) <= 0) {
        //Right-Right
        return rotateLeft(root);
    }
    else if (balanceFactor < -1 && getBalance(root->getRightNode()) > 0) {
        //Right-Left
        root->setRight(rotateRight(root->getRightNode()));
        return rotateLeft(root);
    }
    return root;
}

Data* Cache::read(int addr) {
    // returns the data stored at the addresss addr if
    // the addr is kept in the cache, otherwise, return NULL.
    Elem* data = tree->findData(addr,tree->getRoot());
    if (data) {
        return data->data;
    }
    else {
        return NULL;
    }
    return NULL;
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
        Elem* deleteElem = new Elem(addr,NULL,true);
        tree->setRoot(tree->deleteNode(tree->getRoot(), removeAddress,deleteElem));
        
        // Add new address into queue
        list.push(addr);
        // Add new address, Elem into AVL
        tree->setRoot(tree->insertNode(tree->getRoot(), addr, new Elem(addr, cont, true)));
        return deleteElem;
    }
    else {
       // Add new address into queue
       list.push(addr);
       // Add new address, Elem into AVL
       tree->setRoot(tree->insertNode(tree->getRoot(), addr, new Elem(addr, cont, true)));
       return NULL;
    }
}
Elem* Cache::write(int addr, Data* cont) {
    // searches if the addr is in the cache. If it is, this method 
    // replaces the existing value associated with the addr with 
    // the cont. If not, this method puts the addr and cont into the cache. 
    // This method returns the element removed out of the cache
    // if any, otherwise, returns NULL.
    Elem* data = tree->findData(addr, tree->getRoot());
    if (data) {
        data->data = cont;
        data->sync = false;
        return data;
    }
    else {
        if (list.size() == cacheSize) {
            // Remove the address from the queue
            int removeAddress = list.front();
            list.pop();
            // Remove it from the AVL tree;
            Elem* deleteElem = new Elem(addr, NULL, true);
            tree->setRoot(tree->deleteNode(tree->getRoot(), removeAddress, deleteElem));

            // Add new address into queue
            list.push(addr);
            // Add new address, Elem into AVL
            tree->setRoot(tree->insertNode(tree->getRoot(), addr, new Elem(addr, cont, false)));
            return deleteElem;
        }
        else {
            // Add new address into queue
            list.push(addr);
            // Add new address, Elem into AVL
            tree->setRoot(tree->insertNode(tree->getRoot(), addr, new Elem(addr, cont, false)));
            return NULL;
        }
    }
}
void Cache::print() {
    // prints the value of the elements in the cache in the descending 
    // order of the element’s living time.Each element must be printed 
    // by the method print of class Elem.
    queuePrint(list);
}
void Cache::preOrder() {
    // prints the value of the elements in the cache 
    // in the preorder of the AVL tree which must be 
    // used to search an address in the cache.    
    prePrint(tree->getRoot());
}
void Cache::inOrder() {
    // prints the value of the elements in the cache 
    // in the inorder of the AVL tree which must be 
    // used to search an address in the cache.
    inPrint(tree->getRoot());
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

