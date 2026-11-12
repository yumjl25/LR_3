#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "queue.hpp"
#include <string>

using namespace std;

TEST_CASE("Тестирование класса Queue", "[queue]") {
    Queue q;
    string out;

    SECTION("Пустая очередь") {
        REQUIRE(q.isEmpty() == true);
        REQUIRE(q.getSize() == 0);
        REQUIRE(q.pop(out) == false);
        REQUIRE(q.get(out) == false);
        q.print(); // Проверка print для пустой
    }

    SECTION("Добавление и извлечение (FIFO)") {
        q.push("first");
        q.push("second");
        q.push("third");

        REQUIRE(q.getSize() == 3);
        q.print();

        REQUIRE(q.pop(out) == true);
        REQUIRE(out == "first"); 

        REQUIRE(q.pop(out) == true);
        REQUIRE(out == "second");
    }

    SECTION("Проверка get без удаления") {
        q.push("apple");
        REQUIRE(q.get(out) == true);
        REQUIRE(out == "apple");
        REQUIRE(q.getSize() == 1); // Размер не изменился
    }

    SECTION("Удаление последнего элемента (сброс tail)") {
        q.push("only_one");
        REQUIRE(q.pop(out) == true);
        REQUIRE(out == "only_one");
        REQUIRE(q.isEmpty() == true);
        
        // После опустошения очередь должна снова работать
        q.push("new");
        REQUIRE(q.get(out) == true);
        REQUIRE(out == "new");
    }
        SECTION("Сохранение и загрузка очереди (бинарный формат)") {
        q.push("first");
        q.push("second");
        q.push("third");
        q.saveToBinary("test_queue.bin");

        Queue q2;
        q2.loadFromBinary("test_queue.bin");

        REQUIRE(q2.getSize() == 3);
        string out;
        q2.pop(out);
        REQUIRE(out == "first");
        q2.pop(out);
        REQUIRE(out == "second");
    }
}