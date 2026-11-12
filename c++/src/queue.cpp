#include "queue.hpp"
#include <iostream>
#include <iostream>
#include <fstream>

using namespace std;

Queue::Queue() : head(nullptr), tail(nullptr), size(0) {}

Queue::~Queue(){
    while (head != nullptr){
        QNode* temp = head;
        head = temp->next;
        delete temp;
    }
}

void Queue::push(const string& value){
    QNode* node = new QNode(value);
    if (tail != nullptr){
        tail->next = node;
    }else{
        head = node;
    }
    tail = node;
    size++;
}

bool Queue::pop(string& out){
    if (head == nullptr) return false;
    QNode* old = head;
    out = old->data;
    head = old->next;
    if (head == nullptr){
        tail = nullptr;
    }
    delete old;
    size--;
    return true;
}

bool Queue::get(string& out) const{
    if (head == nullptr) return false;
    out = head->data;
    return true;
}

bool Queue::isEmpty() const { return head == nullptr; }
int Queue::getSize() const { return size; }

void Queue::print() const {
    QNode* cur = head;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void Queue::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;

    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    QNode* cur = head;
    while (cur != nullptr) {
        int len = cur->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(cur->data.c_str(), len);
        cur = cur->next;
    }
    file.close();
}

void Queue::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    while (head != nullptr) {
        QNode* temp = head;
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
        push(s);
    }
    file.close();
}