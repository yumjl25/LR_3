#include "hashtable.hpp"
#include <iostream>
#include <fstream>

using namespace std;

HashTable::HashTable(int cap) {
    capacity = cap;
    size = 0;
    buckets = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        buckets[i] = nullptr;
    }
}

HashTable::~HashTable() {
    for (int i = 0; i < capacity; i++) {
        HashNode* cur = buckets[i];
        while (cur != nullptr) {
            HashNode* next = cur->next;
            delete cur;
            cur = next;
        }
    }
    delete[] buckets;
}

unsigned int HashTable::hashFunc(const string& key) const {
    unsigned int hash = 0;
    for (char c : key) {
        hash = (hash + c) % capacity;
    }
    return hash;
}

void HashTable::add(const string& key, const string& value) {
    unsigned int idx = hashFunc(key);
    HashNode* cur = buckets[idx];

    while (cur != nullptr) {
        if (cur->key == key) {
            cur->value = value;
            return;
        }
        cur = cur->next;
    }

    HashNode* newNode = new HashNode(key, value);
    newNode->next = buckets[idx];
    buckets[idx] = newNode;
    size++;
}

bool HashTable::get(const string& key, string& out) const {
    unsigned int idx = hashFunc(key);
    HashNode* cur = buckets[idx];

    while (cur != nullptr) {
        if (cur->key == key) {
            out = cur->value;
            return true;
        }
        cur = cur->next;
    }
    return false;
}

bool HashTable::del(const string& key) {
    unsigned int idx = hashFunc(key);
    HashNode* cur = buckets[idx];
    HashNode* prev = nullptr;

    while (cur != nullptr) {
        if (cur->key == key) {
            if (prev == nullptr) {
                buckets[idx] = cur->next;
            } else {
                prev->next = cur->next;
            }
            delete cur;
            size--;
            return true;
        }
        prev = cur;
        cur = cur->next;
    }
    return false;
}

bool HashTable::contains(const string& key) const {
    string dummy;
    return get(key, dummy);
}

bool HashTable::isEmpty() const { return size == 0; }
int HashTable::getSize() const { return size; }

void HashTable::print() const {
    for (int i = 0; i < capacity; i++) {
        HashNode* cur = buckets[i];
        while (cur != nullptr) {
            cout << "[" << i << "] " << cur->key << " : " << cur->value << endl;
            cur = cur->next;
        }
    }
}

void HashTable::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    
    for (int i = 0; i < capacity; i++) {
        HashNode* cur = buckets[i];
        while (cur != nullptr) {
            int klen = cur->key.length();
            int vlen = cur->value.length();
            file.write(reinterpret_cast<const char*>(&klen), sizeof(klen));
            file.write(cur->key.c_str(), klen);
            file.write(reinterpret_cast<const char*>(&vlen), sizeof(vlen));
            file.write(cur->value.c_str(), vlen);
            cur = cur->next;
        }
    }
    file.close();
}

void HashTable::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;
    
    for (int i = 0; i < capacity; i++) {
        HashNode* cur = buckets[i];
        while (cur != nullptr) {
            HashNode* next = cur->next;
            delete cur;
            cur = next;
        }
        buckets[i] = nullptr;
    }
    size = 0;

    int new_size = 0;
    if (!file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size))) return;

    for (int i = 0; i < new_size; i++) {
        int klen; file.read(reinterpret_cast<char*>(&klen), sizeof(klen));
        string k(klen, '\0'); file.read(&k[0], klen);
        
        int vlen; file.read(reinterpret_cast<char*>(&vlen), sizeof(vlen));
        string v(vlen, '\0'); file.read(&v[0], vlen);
        
        add(k, v); // Используем наш метод add
    }
    file.close();
}