#ifndef QUEUE_H
#define QUEUE_H
#include <string>

class Queue{
private:
    struct QNode{
        std::string data;
        QNode* next;
        explicit QNode(const std::string& val) : data(val), next(nullptr) {}
    };

    QNode* head;
    QNode* tail;
    int size;

public:
    Queue();
    ~Queue();
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    void push(const std::string& value);
    bool pop(std::string& out);
    
    bool get(std::string& out) const;
    bool isEmpty() const;
    int getSize() const;
    void print() const;
    void saveToBinary(const std::string& filename) const;
    void loadFromBinary(const std::string& filename);
};

#endif