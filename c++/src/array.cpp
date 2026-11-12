#include "array.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Array::Array(){
    capacity = 4;
    size = 0;
    data = new string[capacity];
}

Array::~Array(){
    delete[] data;
}

void Array::resize(){
    int new_capacity = capacity * 2;
    string* new_data = new string[new_capacity];
    for (int i = 0; i < size; i++){
        new_data[i] = data[i];
    }
    delete[] data;
    data = new_data;
    capacity = new_capacity;
}

void Array::push(const string& value){
    if (size == capacity){resize();}
    data[size] = value;
    size++;
}

void Array::pushAt(const string& value, int index){
    if (index < 0 || index > size){
        cout << "Index out of range" << endl;
        return;
    }

    if (size == capacity){resize();}

    for (int i = size; i > index; i--){
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

bool Array::get(int index, string& out) const {
    if (index < 0 || index >= size){
        return false;
    }
    out = data[index];
    return true;
}

void Array::set(int index, const string& value){
    if (index < 0 || index >= size){
        cout << "Index out of range" << endl;
        return;
    }
    data[index] = value;
}

bool Array::del(int index, string& out){
    if (index < 0 || index >= size){
        return false;
    }
    out = data[index];

    for (int i = index; i < size - 1; i++){
        data[i] = data[i + 1];
    }
    size--;
    return true;
}

void Array::saveToBinary(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) return;

    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(&capacity), sizeof(capacity));

    for (int i = 0; i < size; i++) {
        int len = data[i].length();
        file.write(reinterpret_cast<const char*>(&len), sizeof(len));
        file.write(data[i].c_str(), len);
    }
    file.close();
}

void Array::loadFromBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) return;

    delete[] data;
    size = 0;
    
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    file.read(reinterpret_cast<char*>(&capacity), sizeof(capacity));
    
    data = new string[capacity];

    for (int i = 0; i < size; i++) {
        int len;
        file.read(reinterpret_cast<char*>(&len), sizeof(len));
        
        string s(len, '\0');
        file.read(&s[0], len);
        data[i] = s;
    }
    file.close();
}

bool Array::isEmpty() const {return size == 0;}
int Array::getSize() const {return size;}
int Array::getCapacity() const {return capacity;}