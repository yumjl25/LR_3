#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <string>

class Array{
private:
    std::string* data;
    int size;
    int capacity;

    void resize();

public:
    Array();
    ~Array();

    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;

    void push(const std::string& value);
    void pushAt(const std::string& value, int index);
    bool get(int index, std::string& out) const;
    void set(int index, const std::string& value);
    bool del(int index, std::string& out);

    bool isEmpty() const;
    int getSize() const;
    int getCapacity() const;
    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename);
};

#endif