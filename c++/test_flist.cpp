#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "flist.hpp"
#include <string>

using namespace std;

TEST_CASE("Тестирование класса ForwardList", "[flist]") {
    ForwardList list;
    string out;

    SECTION("Пустой список") {
        REQUIRE(list.isEmpty() == true);
        REQUIRE(list.getSize() == 0);
        REQUIRE(list.delHead(out) == false);
        REQUIRE(list.delTail(out) == false);
        REQUIRE(list.delValue("apple") == false);
        REQUIRE(list.getHead(out) == false);
        REQUIRE(list.getTail(out) == false);
        REQUIRE(list.search("apple") == false);
        list.print(); 
    }

    SECTION("Добавление в голову и хвост") {
        list.pushTail("middle");
        list.pushHead("first");
        list.pushTail("last");
        
        REQUIRE(list.getSize() == 3);
        list.getHead(out);
        REQUIRE(out == "first");
        list.getTail(out);
        REQUIRE(out == "last");
        list.print(); 
    }

    SECTION("Добавление до и после элемента") {
        list.pushTail("A");
        list.pushTail("C");
        list.pushBefore("C", "B"); // A, B, C
        list.pushAfter("C", "D");  // A, B, C, D
        
        REQUIRE(list.getSize() == 4);
        list.getAt(1, out);
        REQUIRE(out == "B");
        list.getAt(3, out);
        REQUIRE(out == "D");
    }

    SECTION("Удаление головы и хвоста") {
        list.pushTail("1");
        list.pushTail("2");
        list.pushTail("3");

        REQUIRE(list.delHead(out) == true);
        REQUIRE(out == "1");
        REQUIRE(list.delTail(out) == true);
        REQUIRE(out == "3");
        REQUIRE(list.getSize() == 1);
        
        REQUIRE(list.delHead(out) == true);
        REQUIRE(out == "2");
        REQUIRE(list.isEmpty() == true);
    }

    SECTION("Удаление по значению и поиск") {
        list.pushTail("apple");
        list.pushTail("banana");
        list.pushTail("cherry");
        
        REQUIRE(list.search("banana") == true);

        // Удаляем из середины
        REQUIRE(list.delValue("banana") == true);
        REQUIRE(list.getSize() == 2);
        
        // Удаляем голову через delValue
        REQUIRE(list.delValue("apple") == true);
        
        // Удаляем хвост через delValue
        REQUIRE(list.delValue("cherry") == true);
        REQUIRE(list.isEmpty() == true);
    }

    SECTION("Обработка неверных индексов и отсутствующих элементов") {
        list.pushTail("test");
        
        REQUIRE(list.getAt(-1, out) == false);
        REQUIRE(list.getAt(5, out) == false);
        
        list.pushBefore("nonexistent", "val"); // "Target not found"
        list.pushAfter("nonexistent", "val");  // "Target not found"
        
        list.pushBefore("test", "new_head"); 
    }
    SECTION("Сохранение и загрузка односвязного списка (бинарный формат)") {
        list.pushTail("X");
        list.pushTail("Y");
        list.saveToBinary("test_flist.bin");

        ForwardList list2;
        list2.loadFromBinary("test_flist.bin");

        REQUIRE(list2.getSize() == 2);
        string out;
        list2.getAt(0, out);
        REQUIRE(out == "X");
        list2.getAt(1, out);
        REQUIRE(out == "Y");
    }
}