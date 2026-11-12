#ifndef STACK_HPP
#define STACK_HPP
#include <string>

class Stack{
private:
    struct SNode{
        std::string data;
        SNode* next;
        explicit SNode(const std::string& val, SNode* nxt) : data(val), next(nxt) {}
    };

    SNode* head;
    int size;

public:
    Stack();
    ~Stack();

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    void push(const std::string& value);
    bool pop(std::string& out);
    bool get(std::string& out) const;
    bool isEmpty() const;
    int getSize() const; 
    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename); 
};

#endif