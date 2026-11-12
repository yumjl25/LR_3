#include "cuckoo.hpp"
#include <iostream>
#include <fstream>
using namespace std;

CuckooTable::CuckooTable(int cap) {
    capacity = cap;
    size = 0;
    kickLimit = 50;
    table1 = new CuckooEntry[capacity];
    table2 = new CuckooEntry[capacity];
    for (int i = 0; i < capacity; i++) {
        table1[i].isOccupied = false;
        table2[i].isOccupied = false;
    }
}

CuckooTable::~CuckooTable() {
    delete[] table1;
    delete[] table2;
}

unsigned int CuckooTable::hash1(const string& key) const {
    unsigned int h = 0;
    for (char c : key) h = (h * 31 + c) % capacity;
    return h;
}

unsigned int CuckooTable::hash2(const string& key) const {
    unsigned int h = 5381;
    for (char c : key) h = ((h << 5) + h) + c;
    return h % capacity;
}

bool CuckooTable::insertHelper(const string& key, const string& value) {
    string curKey = key;
    string curVal = value;
    bool useTable1 = true;

    for (int i = 0; i < kickLimit; i++) {
        if (useTable1) {
            int idx = hash1(curKey);
            if (!table1[idx].isOccupied) {
                table1[idx] = {curKey, curVal, true};
                return true;
            } else {
                swap(table1[idx].key, curKey);
                swap(table1[idx].value, curVal);
                useTable1 = false;
            }
        } else {
            int idx = hash2(curKey);
            if (!table2[idx].isOccupied) {
                table2[idx] = {curKey, curVal, true};
                return true;
            } else {
                swap(table2[idx].key, curKey);
                swap(table2[idx].value, curVal);
                useTable1 = true;
            }
        }
    }
    return false;
}

void CuckooTable::rehash() {
    int oldCap = capacity;
    CuckooEntry* oldT1 = table1;
    CuckooEntry* oldT2 = table2;

    capacity *= 2;
    table1 = new CuckooEntry[capacity];
    table2 = new CuckooEntry[capacity];
    for (int i = 0; i < capacity; i++) {
        table1[i].isOccupied = false;
        table2[i].isOccupied = false;
    }

    for (int i = 0; i < oldCap; i++) {
        if (oldT1[i].isOccupied) insertHelper(oldT1[i].key, oldT1[i].value);
        if (oldT2[i].isOccupied) insertHelper(oldT2[i].key, oldT2[i].value);
    }

    delete[] oldT1;
    delete[] oldT2;
}

void CuckooTable::add(const string& key, const string& value) {
    string dummy;
    if (get(key, dummy)) {
        int idx1 = hash1(key);
        if (table1[idx1].isOccupied && table1[idx1].key == key) {
            table1[idx1].value = value;
            return;
        }
        int idx2 = hash2(key);
        if (table2[idx2].isOccupied && table2[idx2].key == key) {
            table2[idx2].value = value;
            return;
        }
    }

    if (!insertHelper(key, value)) {
        rehash();
        insertHelper(key, value);
    }
    size++;
}

bool CuckooTable::get(const string& key, string& out) const {
    int idx1 = hash1(key);
    if (table1[idx1].isOccupied && table1[idx1].key == key) {
        out = table1[idx1].value;
        return true;
    }
    int idx2 = hash2(key);
    if (table2[idx2].isOccupied && table2[idx2].key == key) {
        out = table2[idx2].value;
        return true;
    }
    return false;
}

bool CuckooTable::del(const string& key) {
    int idx1 = hash1(key);
    if (table1[idx1].isOccupied && table1[idx1].key == key) {
        table1[idx1].isOccupied = false;
        size--;
        return true;
    }
    int idx2 = hash2(key);
    if (table2[idx2].isOccupied && table2[idx2].key == key) {
        table2[idx2].isOccupied = false;
        size--;
        return true;
    }
    return false;
}

bool CuckooTable::contains(const string& key) const {
    string dummy;
    return get(key, dummy);
}

bool CuckooTable::isEmpty() const { return size == 0; }
int CuckooTable::getSize() const { return size; }

void CuckooTable::print() const {
    cout << "Table 1:" << endl;
    for (int i = 0; i < capacity; i++) {
        if (table1[i].isOccupied) cout << "[" << i << "] " << table1[i].key << " : " << table1[i].value << endl;
    }
    cout << "Table 2:" << endl;
    for (int i = 0; i < capacity; i++) {
        if (table2[i].isOccupied) cout << "[" << i << "] " << table2[i].key << " : " << table2[i].value << endl;
    }
}

void CuckooTable::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;
    
    file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    
    for (int i = 0; i < capacity; i++) {
        if (table1[i].isOccupied) {
            int klen = table1[i].key.length();
            int vlen = table1[i].value.length();
            file.write(reinterpret_cast<const char*>(&klen), sizeof(klen));
            file.write(table1[i].key.c_str(), klen);
            file.write(reinterpret_cast<const char*>(&vlen), sizeof(vlen));
            file.write(table1[i].value.c_str(), vlen);
        }
    }
    
    for (int i = 0; i < capacity; i++) {
        if (table2[i].isOccupied) {
            int klen = table2[i].key.length();
            int vlen = table2[i].value.length();
            file.write(reinterpret_cast<const char*>(&klen), sizeof(klen));
            file.write(table2[i].key.c_str(), klen);
            file.write(reinterpret_cast<const char*>(&vlen), sizeof(vlen));
            file.write(table2[i].value.c_str(), vlen);
        }
    }
    file.close();
}

void CuckooTable::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    delete[] table1; 
    delete[] table2;
    
    int new_capacity = 0, new_size = 0;
    file.read(reinterpret_cast<char*>(&new_capacity), sizeof(new_capacity));
    file.read(reinterpret_cast<char*>(&new_size), sizeof(new_size));
    
    capacity = new_capacity;
    size = 0;

    table1 = new CuckooEntry[capacity];
    table2 = new CuckooEntry[capacity];
    for (int i = 0; i < capacity; i++) {
        table1[i].isOccupied = false;
        table2[i].isOccupied = false;
    }


    for (int i = 0; i < new_size; i++) {
        int klen, vlen;
        
        file.read(reinterpret_cast<char*>(&klen), sizeof(klen));
        string k(klen, '\0'); 
        file.read(&k[0], klen);
        
        file.read(reinterpret_cast<char*>(&vlen), sizeof(vlen));
        string v(vlen, '\0'); 
        file.read(&v[0], vlen);
        
        add(k, v);
    }
    file.close();
}