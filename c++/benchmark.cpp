#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include "stack.hpp"
#include "array.hpp"
#include "flist.hpp"
#include "list.hpp"
#include "queue.hpp"
#include "hashtable.hpp"
#include "cuckoo.hpp"
#include "avltree.hpp"

using namespace std;
using namespace std::chrono;

const int N = 100000;

void printTime(const string& action, high_resolution_clock::time_point start, high_resolution_clock::time_point end) {
    auto ms = duration_cast<milliseconds>(end - start).count();
    cout << action << ": " << ms << " мс" << endl;
}

int main() {
    cout << " Бенчмарк структур данных (N = " << N << ") \n" << endl;

    cout << " Массив (Array) " << endl;
    Array arr;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) arr.push("val");
    auto end = high_resolution_clock::now();
    printTime("Вставка", start, end);
    cout << endl;

    cout << " Односвязный список (ForwardList) " << endl;
    ForwardList flist;
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) flist.pushTail("val");
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);
    cout << endl;

    cout << " Двусвязный список (List) " << endl;
    List list;
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) list.pushTail("val");
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);
    cout << endl;

    cout << " Стек (Stack) " << endl;
    Stack stack;
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) stack.push("val");
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);
    cout << endl;

    cout << " Очередь (Queue) " << endl;
    Queue queue;
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) queue.push("val");
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);
    cout << endl;

    cout << " Хеш-таблица (Цепочки) " << endl;
    HashTable ht(N);
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) ht.add(to_string(i), "val");
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);

    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        string out;
        ht.get(to_string(i), out);
    }
    end = high_resolution_clock::now();
    printTime("Поиск  ", start, end);
    cout << endl;

    cout << " Хеш-таблица (Кукушка) " << endl;
    CuckooTable cuckoo(N);
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) cuckoo.add(to_string(i), "val");
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);

    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        string out;
        cuckoo.get(to_string(i), out);
    }
    end = high_resolution_clock::now();
    printTime("Поиск  ", start, end);
    cout << endl;

    cout << " АВЛ-дерево (AVLTree) " << endl;
    AVLTree tree;
    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) tree.insert(i);
    end = high_resolution_clock::now();
    printTime("Вставка", start, end);

    start = high_resolution_clock::now();
    for (int i = 0; i < N; i++) tree.search(i);
    end = high_resolution_clock::now();
    printTime("Поиск  ", start, end);
    cout << endl;

    return 0;
}