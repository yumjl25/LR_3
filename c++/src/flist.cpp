#include "flist.hpp"
#include <iostream>
#include <fstream>
using namespace std;

ForwardList::ForwardList() : head(nullptr), tail(nullptr), size(0) {}

ForwardList::~ForwardList(){
    while (head != nullptr){
        FNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void ForwardList::pushHead(const string& value){
    head = new FNode{value, head};
    if (tail == nullptr){
        tail = head;
    }
    size++;
}

void ForwardList::pushTail(const string& value){
    FNode* newNode = new FNode{value, nullptr};
    if (tail == nullptr){
        head = newNode;
        tail = newNode;
    }else{
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

void ForwardList::pushBefore(const string& target, const string& value){
    if (head == nullptr) return;
    if (head->data == target){
        pushHead(value);
    }
    FNode* cur = head;
    while(cur->next != nullptr && cur->next->data != target){
        cur = cur->next;
    }
    if (cur->next == nullptr){
        cout << "Target not found" << endl;
        return;
    }
    FNode* newNode = new FNode{value, cur->next};
    cur->next = newNode;
    size++;
}

void ForwardList::pushAfter(const string& target, const string& value){
    FNode* cur = head;
    while(cur != nullptr && cur->data != target){
        cur = cur->next;
    }
    if (cur == nullptr){
        cout << "Target not found" << endl;
        return;
    }
    FNode* newNode = new FNode{value, cur->next};
    cur->next = newNode;
    if (cur == tail){
        tail = newNode;
    }
    size++;
}

bool ForwardList::delHead(string& out){
    if (head == nullptr) return false;
    out = head->data;
    FNode* temp = head;
    head = head->next;
    if (head == nullptr){
        tail = nullptr;
    }
    delete temp;
    size--;
    return true;
}

bool ForwardList::delTail(string& out){
    if (head == nullptr) return false;
    if (head == tail){
        delHead(out);
    }
    FNode* cur = head;
    while (cur->next != tail){
        cur = cur->next;
    }
    out = tail->data;
    delete tail;
    tail = cur;
    tail->next = nullptr;
    size--;
    return true;
}

bool ForwardList::delValue(const string& value){
    if (head == nullptr) return false;
    if (head->data == value){
        string dummy;
        delHead(dummy);
        return true;
    }
    FNode* cur = head;
    while(cur->next != nullptr && cur->next->data != value){
        cur = cur->next;
    }
    if (cur->next == nullptr) return false;

    FNode* victim = cur->next;
    cur->next = victim->next;
    if (victim == tail){
        tail = cur;
    }
    delete victim;
    size--;
    return true;
}

bool ForwardList::search(const string& value) const{
    FNode* cur = head;
    while (cur != nullptr){
        if (cur->data == value){
            return true;
        }
        cur = cur->next;
    }
    return false;
}

bool ForwardList::getHead(string& out) const{
    if (head == nullptr) return false;
    out = head->data;
    return true;
}

bool ForwardList::getTail(string& out) const{
    if (tail == nullptr) return false;
    out = tail->data;
    return true;
}

bool ForwardList::getAt(int index, string& out) const{
    if (index < 0 || index >= size) return false;
    FNode* cur = head;
    for (int i = 0; i < index; i++){
        cur = cur->next;
    }
    out = cur->data;
    return true;
}

bool ForwardList::isEmpty() const {return head == nullptr;}

int ForwardList::getSize() const {return size;}

void ForwardList::print() const {
    FNode* cur = head;
    while (cur != nullptr) {
        cout << cur->data << " ";
        cur = cur->next;
    }
    cout << endl;
}

void ForwardList::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    FNode* cur = head;
    while (cur != nullptr) {
        int len = cur->data.length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(cur->data.c_str(), len);
        cur = cur->next;
    }
    file.close();
}

void ForwardList::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;
    while (head != nullptr) {
        FNode* temp = head;
        head = head->next;
        delete temp;
    }
    head = nullptr; tail = nullptr; size = 0;
    
    int new_size = 0;
    if (!file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size))) return;
    
    for (int i = 0; i < new_size; i++) {
        int len; file.read(reinterpret_cast<char*>(&len), sizeof(len));
        string s(len, '\0'); file.read(&s[0], len);
        pushTail(s);
    }
    file.close();
}