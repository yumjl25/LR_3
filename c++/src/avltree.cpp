#include "avltree.hpp"
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

AVLTree::AVLTree() : root(nullptr), size(0) {}

AVLTree::~AVLTree(){
    clear(root);
}

void AVLTree::clear(TNode* node){
    if (node == nullptr) return;

    clear(node->left);
    clear(node->right);
    delete node;
}

int AVLTree::height(TNode* node) const {
    return (node == nullptr) ? 0 : node->height;
}

void AVLTree::updateHeight(TNode* node) const {
    if (node != nullptr){
        node->height = 1 + max(height(node->left), height(node->right));
    }
}

int AVLTree::balanceFactor(TNode* node) const {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

AVLTree::TNode* AVLTree::rotateRight(TNode* y){
    AVLTree::TNode* x = y->left;
    AVLTree::TNode* B = x->right;
    x->right = y;
    y->left = B;
    updateHeight(y);
    updateHeight(x);
    return x;
}

AVLTree::TNode* AVLTree::rotateLeft(TNode* x){
    AVLTree::TNode* y = x->right;
    AVLTree::TNode* B = y->left;
    y->left = x;
    x->right = B;
    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLTree::TNode* AVLTree::balance(TNode* node){
    if (node == nullptr) return nullptr;
    updateHeight(node);
    int bf = balanceFactor(node);

    if (bf > 1 && balanceFactor(node->left) >= 0) return rotateRight(node);
    if (bf > 1 && balanceFactor(node->left) < 0){
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1 && balanceFactor(node->right) <= 0) return rotateLeft(node);
    if (bf < -1 && balanceFactor(node->right) > 0){
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

AVLTree::TNode* AVLTree::insertNode(TNode* node, int value){
    if (node == nullptr) return new TNode(value);
    if (value < node->data){
        node->left = insertNode(node->left, value);
    }else if(value > node->data){
        node->right = insertNode(node->right, value);
    }else{
        return node;
    }
    return balance(node);
}

void AVLTree::insert(int value) {
    if (search(value)) {
        return; 
    }
    root = insertNode(root, value);
    size++;
}

bool AVLTree::search(int value) const {
    TNode* cur = root;
    while(cur != nullptr){
        if (value == cur->data) return true;
        if (value < cur->data){
            cur = cur->left;
        }else{
            cur = cur->right;
        }
    }
    return false;
}

AVLTree::TNode* AVLTree::findMin(TNode* node){
    while (node->left != nullptr){
        node = node->left;
    }
    return node;
}

AVLTree::TNode* AVLTree::deleteNode(TNode* node, int value, int& out, bool& deleted){
    if (node == nullptr) return nullptr;
    if (value < node->data){
        node->left = deleteNode(node->left, value, out, deleted);
    }else if(value > node->data){
        node->right = deleteNode(node->right, value, out, deleted);
    }else{
        if (!deleted) {
            out = node->data;
            deleted = true;
        }

        if (node->left == nullptr || node->right == nullptr){
            TNode* child = (node->left != nullptr) ? node->left : node->right;
            if (child == nullptr){
                delete node;
                return nullptr;
            }
            delete node;
            return child;
        }else{
            TNode* successor = findMin(node->right);
            node->data = successor->data;
            node->right = deleteNode(node->right, successor->data, out, deleted);
        }
    }
    return balance(node);
}

bool AVLTree::del(int value, int& out) {
    bool deleted = false;
    root = deleteNode(root, value, out, deleted);
    if (deleted) {
        size--;
    }
    return deleted;
}

void AVLTree::bfs(TNode* node) const {
    if (node == nullptr) return;

    TNode** queue = new TNode*[size * 2];
    int head = 0;
    int tail = 0;

    queue[tail++] = node;

    while (head < tail) {
        TNode* cur = queue[head++];
        cout << cur->data << " ";

        if (cur->left != nullptr) queue[tail++] = cur->left;
        if (cur->right != nullptr) queue[tail++] = cur->right;
    }
    
    delete[] queue;
}

void AVLTree::printBFS() const {
    if (root == nullptr) {
        cout << "Tree is empty" << endl;
        return;
    }
    bfs(root);
    cout << endl;
}

void AVLTree::inorder(TNode* node) const {
    if (node == nullptr) return;
    inorder(node->left);
    cout << node->data << " ";
    inorder(node->right);
}

void AVLTree::printInorder() const {inorder(root); cout << endl;}

void AVLTree::preorder(TNode* node) const {
    if (node == nullptr) return;
    cout << node->data << " ";
    preorder(node->left);
    preorder(node->right);
}

void AVLTree::printPreorder() const {preorder(root); cout << endl;}

void AVLTree::postorder(TNode* node) const {
    if (node == nullptr) return;
    postorder(node->left);
    postorder(node->right);
    cout << node->data << " ";
}

void AVLTree::saveNode(AVLTree::TNode* node, ofstream& file) const {
    if (node == nullptr) return;
    file.write(reinterpret_cast<const char*>(&node->data), sizeof(node->data));
    saveNode(node->left, file);
    saveNode(node->right, file);
}

void AVLTree::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    saveNode(root, file);
    file.close();
}

void AVLTree::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;
    clear(root); 
    root = nullptr; size = 0;

    int new_size = 0;
    if (!file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size))) return;

    for (int i = 0; i < new_size; i++) {
        int val;
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        insert(val);
    }
    file.close();
}

void AVLTree::printPostorder() const {postorder(root); cout << endl;}

bool AVLTree::isEmpty() const {return root == nullptr;}
int AVLTree::getSize() const {return size;}