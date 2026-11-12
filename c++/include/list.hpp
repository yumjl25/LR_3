#ifndef LIST_HPP
#define LIST_HPP
#include <string>

class List{
private:
    struct LNode{
        std::string data;
        LNode* prev;
        LNode* next;
        explicit LNode(const std::string& val) : data(val), prev(nullptr), next(nullptr) {}
    };
    LNode* head;
    LNode* tail;
    int size;

public:
    List();
    ~List();
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    void pushHead(const std::string& value);
    void pushTail(const std::string& value);
    void pushBefore(const std::string& target, const std::string& value);
    void pushAfter(const std::string& target, const std::string& value);

    bool delHead(std::string& out);
    bool delTail(std::string& out);
    bool delValue(const std::string& value);

    bool search(const std::string& value) const;
    bool getHead(std::string& out) const;
    bool getTail(std::string& out) const;
    bool getAt(int index, std::string& out) const;

    bool isEmpty() const;
    int getSize() const;
    void print() const;

    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename);
};

#endif