#include "stack.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

Stack::Stack() : head(nullptr), size(0) {}

Stack::~Stack(){
    while (head != nullptr){
        SNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void Stack::push(const std::string& value){
    head = new SNode(value, head);
    size++;
}

bool Stack::pop(std::string& out){
    if (head == nullptr) {return false;}
    out = head->data;
    SNode* temp = head;
    head = head->next;
    delete temp;
    size--;
    return true;
}

bool Stack::get(std::string& out) const{
    if (head == nullptr) {return false;}
    out = head->data;
    return true;
}

bool Stack::isEmpty() const{
    return head == nullptr;
}

int Stack::getSize() const{
    return size;
}

void Stack::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error: cannot open file for saving" << endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(&size), sizeof(size));

    SNode* cur = head;
    while (cur != nullptr) {
        int len = cur->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(cur->data.c_str(), len);
        cur = cur->next;
    }
    file.close();
}


void Stack::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    while (head != nullptr) {
        SNode* temp = head;
        head = head->next;
        delete temp;
    }
    size = 0;

    int new_size = 0;
    if (!file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size))) return;

    vector<string> temp;
    for (int i = 0; i < new_size; i++) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        string s(len, '\0');
        file.read(&s[0], len);
        temp.push_back(s);
    }
    file.close();

    for (int i = new_size - 1; i >= 0; i--) {
        push(temp[i]);
    }
}