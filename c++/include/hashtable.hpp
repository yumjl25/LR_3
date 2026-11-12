#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string>

class HashTable {
private:
    struct HashNode {
        std::string key;
        std::string value;
        HashNode* next;
        explicit HashNode(const std::string& k, const std::string& v) : key(k), value(v), next(nullptr) {}
    };

    HashNode** buckets;
    int capacity;
    int size;

    unsigned int hashFunc(const std::string& key) const;

public:
    explicit HashTable(int cap = 100);
    ~HashTable();
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    void add(const std::string& key, const std::string& value);
    bool get(const std::string& key, std::string& out) const;
    bool del(const std::string& key);
    bool contains(const std::string& key) const;

    bool isEmpty() const;
    int getSize() const;
    void print() const;
    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename);
};

#endif