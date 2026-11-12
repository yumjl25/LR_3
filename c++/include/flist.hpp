#ifndef FLIST_HPP
#define FLIST_HPP
#include <string>

class ForwardList{
private:
    struct FNode{
        std::string data;
        FNode* next;
        explicit FNode(const std::string& val, FNode* nxt) : data(val), next(nxt){}
    };

    FNode* head;
    FNode* tail;
    int size;

public:
    ForwardList();
    ~ForwardList();

    ForwardList(const ForwardList&) = delete;
    ForwardList& operator=(const ForwardList&) = delete;

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