#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "list.hpp"
#include <string>

using namespace std;

TEST_CASE("Тестирование класса List (двусвязный)", "[list]") {
    List list;
    string out;

    SECTION("Пустой список") {
        REQUIRE(list.isEmpty() == true);
        REQUIRE(list.getSize() == 0);
        REQUIRE(list.delHead(out) == false);
        REQUIRE(list.delTail(out) == false);
        REQUIRE(list.delValue("none") == false);
        REQUIRE(list.getHead(out) == false);
        REQUIRE(list.getTail(out) == false);
        
        REQUIRE(list.search("apple") == false); 
        list.print(); 
    }

    SECTION("Добавление и проверка связей") {
        list.pushTail("B");
        list.pushHead("A");
        list.pushTail("D");
        list.pushBefore("D", "C"); // A, B, C, D
        list.pushAfter("D", "E");  // A, B, C, D, E

        REQUIRE(list.getSize() == 5);
        list.getAt(2, out);
        REQUIRE(out == "C");
        list.print();
    }

    SECTION("Вставка before/after для крайних элементов") {
        list.pushTail("middle");
        list.pushBefore("middle", "first"); // Сработает pushHead
        list.pushAfter("middle", "last");   // Сработает pushTail
        
        list.getHead(out);
        REQUIRE(out == "first");
        list.getTail(out);
        REQUIRE(out == "last");
    }

    SECTION("Удаление головы, хвоста и значения") {
        list.pushTail("1");
        list.pushTail("2");
        list.pushTail("3");

        REQUIRE(list.delHead(out) == true);
        REQUIRE(out == "1");
        REQUIRE(list.delTail(out) == true);
        REQUIRE(out == "3");
        
        REQUIRE(list.delValue("2") == true);
        REQUIRE(list.isEmpty() == true); // Проверка, что удалили всё и head/tail стали null
    }

    SECTION("Удаление единственного элемента (проверка веток else)") {
        list.pushHead("only");
        REQUIRE(list.delHead(out) == true); 
        
        list.pushHead("only2");
        REQUIRE(list.delTail(out) == true); 
        
        list.pushTail("only3");
        REQUIRE(list.delValue("only3") == true); 
    }

    SECTION("Удаление из середины") {
        list.pushTail("A");
        list.pushTail("B");
        list.pushTail("C");
        
        REQUIRE(list.delValue("B") == true);
        REQUIRE(list.getSize() == 2);
        list.getAt(1, out);
        REQUIRE(out == "C");
    }

    SECTION("Обработка неверных индексов и целей") {
        list.pushTail("test");
        REQUIRE(list.getAt(-1, out) == false);
        REQUIRE(list.getAt(5, out) == false);
        
        list.pushBefore("none", "val");
        list.pushAfter("none", "val");
    }
    SECTION("Сохранение и загрузка списка (бинарный формат)") {
        list.pushTail("A");
        list.pushTail("B");
        list.pushTail("C");
        list.saveToBinary("test_list.bin");

        List list2;
        list2.loadFromBinary("test_list.bin");

        REQUIRE(list2.getSize() == 3);
        string out;
        list2.getAt(0, out);
        REQUIRE(out == "A");
        list2.getAt(2, out);
        REQUIRE(out == "C");
    }
}