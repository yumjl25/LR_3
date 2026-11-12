#include "list.hpp"
#include <iostream>
#include <fstream>
using namespace std;

List::List() : head(nullptr), tail(nullptr), size(0) {}

List::~List(){
    while(head != nullptr){
        LNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void List::pushHead(const string& value){
    LNode* node = new LNode(value);
    node->next = head;
    if (head != nullptr){
        head->prev = node;
    }else{
        tail = node;
    }
    head = node;
    size++;
}

void List::pushTail(const string& value){
    LNode* node = new LNode(value);
    node->prev = tail;
    if (tail != nullptr){
        tail->next = node;
    }else{
        head = node;
    }
    tail = node;
    size++;
}

void List::pushBefore(const string& target, const string& value){
    LNode* cur = head;
    while (cur != nullptr && cur->data != target){
        cur = cur->next;
    }
    if (cur == nullptr){
        cout << "Target not found" << endl;
        return;
    }

    if (cur == head){
        pushHead(value);
        return;
    }

    LNode* node = new LNode(value);
    node->prev = cur->prev;
    node->next = cur;
    cur->prev->next = node;
    cur->prev = node;
    size++;
}

void List::pushAfter(const string& target, const string& value){
    LNode* cur = head;
    while(cur != nullptr && cur->data != target){
        cur = cur->next;
    }
    if (cur == nullptr){
        cout << "Target not found" << endl;
        return;
    }
    if (cur == tail){
        pushTail(value);
        return;
    }

    LNode* node = new LNode(value);
    node->prev = cur;
    node->next = cur->next;
    cur->next->prev = node;
    cur->next = node;
    size++;
}

bool List::delHead(string& out){
    if (head == nullptr) return false;
    LNode* old = head;
    out = old->data;
    head = old->next;
    if (head != nullptr){
        head->prev = nullptr;
    }else{
        tail = nullptr;
    }
    delete old;
    size--;
    return true;
}

bool List::delTail(string& out){
    if (tail == nullptr) return false;
    LNode* old = tail;
    out = tail->data;
    tail = old->prev;
    if (tail != nullptr){
        tail->next = nullptr;
    }else{
        head = nullptr;
    }
    delete old;
    size--;
    return true;
}

bool List::delValue(const string& value){
    LNode* cur = head;
    while (cur != nullptr && cur->data != value){
        cur = cur->next;
    }
    if (cur == nullptr) return false;

    if (cur->prev != nullptr) {
        cur->prev->next = cur->next;
    } else {
        head = cur->next;
    }
    if (cur->next != nullptr) {
        cur->next->prev = cur->prev;
    } else {
        tail = cur->prev;
    }
    delete cur;
    size--;
    return true;
}

bool List::search(const string& value) const {
    LNode* cur = head;
    while (cur != nullptr) {
        if (cur->data == value) return true;
        cur = cur->next;
    }
    return false;
}

bool List::getHead(string& out) const {
    if (head == nullptr) return false;
    out = head->data;
    return true;
}

bool List::getTail(string& out) const {
    if (tail == nullptr) return false;
    out = tail->data;
    return true;
}

bool List::getAt(int index, string& out) const {
    if (index < 0 || index >= size) return false;
    LNode* cur = head;
    for (int i = 0; i < index; i++) {
        cur = cur->next;
    }
    out = cur->data;
    return true;
}

bool List::isEmpty() const { return head == nullptr; }
int List::getSize() const { return size; }

void List::print() const {
    LNode* cur = head;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void List::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;

    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    LNode* cur = head;
    while (cur != nullptr) {
        int len = cur->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(cur->data.c_str(), len);
        cur = cur->next;
    }
    file.close();
}

void List::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    while (head != nullptr) {
        LNode* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;

    int new_size = 0;
    if (!file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size))) {
        file.close();
        return;
    }

    for (int i = 0; i < new_size; i++) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        string s(len, '\0');
        file.read(&s[0], len);
        pushTail(s);
    }
    file.close();
}