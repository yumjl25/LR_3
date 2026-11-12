#ifndef CUCKOO_HPP
#define CUCKOO_HPP

#include <string>

class CuckooTable {
private:
    struct CuckooEntry {
        std::string key;
        std::string value;
        bool isOccupied;
    };

    CuckooEntry* table1;
    CuckooEntry* table2;
    int capacity;
    int size;
    int kickLimit;

    unsigned int hash1(const std::string& key) const;
    unsigned int hash2(const std::string& key) const;
    
    bool insertHelper(const std::string& key, const std::string& value);
    
    void rehash();

public:
    explicit CuckooTable(int cap = 10);
    ~CuckooTable();
    CuckooTable(const CuckooTable&) = delete;
    CuckooTable& operator=(const CuckooTable&) = delete;

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