#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <string>
#include <fstream>
#include <iostream>

class AVLTree{
private:
    struct TNode{
        int data;
        int height;
        TNode* left;
        TNode* right;
        explicit TNode(int val) : data(val), height(1), left(nullptr), right(nullptr) {}
    };
    TNode* root;
    int size;
    int height(TNode* node) const;
    void updateHeight(TNode* node) const;
    int balanceFactor(TNode* node) const;
    TNode* rotateRight(TNode* y);
    TNode* rotateLeft(TNode* x);
    TNode* balance(TNode* node);
    TNode* insertNode(TNode* node, int value);
    TNode* deleteNode(TNode* node, int value, int& out, bool& deleted);
    TNode* findMin(TNode* node);
    void clear(TNode* node);
    void inorder(TNode* node) const;
    void preorder(TNode* node) const;
    void postorder(TNode* node) const;
    void bfs(TNode* node) const;
    void saveNode(TNode* node, std::ofstream& file) const;

public:
    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    void insert(int value);
    bool search(int value) const;
    
    bool del(int value, int& out); 

    void printInorder() const;
    void printPreorder() const;
    void printPostorder() const;
    void printBFS() const;

    bool isEmpty() const;
    int getSize() const;

    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename);
};

#endif