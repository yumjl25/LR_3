#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "cuckoo.hpp"
#include <string>

using namespace std;

TEST_CASE("Тестирование класса CuckooTable", "[cuckoo]") {
    CuckooTable ct(5); // Маленькая емкость
    string out;

    SECTION("Пустая таблица") {
        REQUIRE(ct.isEmpty() == true);
        REQUIRE(ct.getSize() == 0);
        REQUIRE(ct.get("apple", out) == false);
        REQUIRE(ct.del("apple") == false);
        REQUIRE(ct.contains("apple") == false);
        ct.print();
    }

    SECTION("Добавление, поиск и обновление") {
        ct.add("apple", "10");
        ct.add("banana", "20");
        REQUIRE(ct.getSize() == 2);
        
        REQUIRE(ct.get("apple", out) == true);
        REQUIRE(out == "10");
        
        // Проверка ветки get из table2 (если ключ попал туда)
        REQUIRE(ct.get("banana", out) == true);
        REQUIRE(out == "20");
        
        // Обновление значения в table1 и table2
        ct.add("apple", "99");
        ct.get("apple", out);
        REQUIRE(out == "99");
    }

    SECTION("Удаление элементов и rehash") {
        // Добавляем элементы, чтобы спровоцировать коллизии
        ct.add("a", "1"); 
        ct.add("b", "2"); 
        ct.add("c", "3"); 
        
        REQUIRE(ct.del("b") == true);
        REQUIRE(ct.contains("b") == false);
        REQUIRE(ct.contains("a") == true);
        REQUIRE(ct.contains("c") == true);

        // Проверка удаления из table2
        ct.add("d", "4");
        ct.add("e", "5");
        ct.del("e");
        
        // Добавление большего числа элементов для rehash
        ct.add("1", "one");
        ct.add("2", "two");
        ct.add("3", "three");
        ct.add("4", "four");
        ct.add("5", "five");
        ct.add("6", "six"); 
        ct.add("7", "seven"); // Тут сработает rehash
        REQUIRE(ct.getSize() > 5);
        ct.get("7", out);
        REQUIRE(out == "seven");
    }
        
    SECTION("Сохранение и загрузка кукушки (бинарный формат)") {
        ct.add("test1", "1");
        ct.add("test2", "2");
        ct.saveToBinary("test_cuckoo.bin");

        CuckooTable ct2(5); 
        ct2.loadFromBinary("test_cuckoo.bin");

        REQUIRE(ct2.getSize() == 2);
        string out;
        ct2.get("test1", out);
        REQUIRE(out == "1");
    }
}