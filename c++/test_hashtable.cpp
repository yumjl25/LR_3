#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "hashtable.hpp"
#include <string>

using namespace std;

TEST_CASE("Тестирование класса HashTable", "[hashtable]") {
    HashTable ht(10);
    string out;

    SECTION("Пустая таблица") {
        REQUIRE(ht.isEmpty() == true);
        REQUIRE(ht.getSize() == 0);
        REQUIRE(ht.get("apple", out) == false);
        REQUIRE(ht.del("apple") == false);
        REQUIRE(ht.contains("apple") == false);
        ht.print();
    }

    SECTION("Добавление и поиск") {
        ht.add("apple", "10");
        ht.add("banana", "20");

        REQUIRE(ht.getSize() == 2);
        REQUIRE(ht.get("apple", out) == true);
        REQUIRE(out == "10");
        REQUIRE(ht.contains("banana") == true);
        REQUIRE(ht.contains("orange") == false);
    }

    SECTION("Обновление существующего ключа") {
        ht.add("apple", "10");
        ht.add("apple", "99"); // Обновление

        REQUIRE(ht.getSize() == 1); // Размер не изменился
        ht.get("apple", out);
        REQUIRE(out == "99");
    }

    SECTION("Удаление элементов") {
        ht.add("apple", "10");
        ht.add("banana", "20");

        REQUIRE(ht.del("apple") == true);
        REQUIRE(ht.getSize() == 1);
        REQUIRE(ht.contains("apple") == false);

        REQUIRE(ht.del("banana") == true);
        REQUIRE(ht.isEmpty() == true);
    }

    SECTION("Коллизии (добавление в одну корзину)") {
        // Подбираем ключи, которые дадут одинаковый хеш (при capacity=10)
        ht.add("ab", "first");
        ht.add("ba", "second");

        REQUIRE(ht.getSize() == 2);
        REQUIRE(ht.get("ab", out) == true);
        REQUIRE(out == "first");
        REQUIRE(ht.get("ba", out) == true);
        REQUIRE(out == "second");
        
        // Удаление из середины цепочки
        REQUIRE(ht.del("ab") == true);
        REQUIRE(ht.contains("ba") == true);
    }
    SECTION("Сохранение и загрузка хеш-таблицы (бинарный формат)") {
        ht.add("key1", "val1");
        ht.add("key2", "val2");
        ht.saveToBinary("test_ht.bin");

        HashTable ht2(10); 
        ht2.loadFromBinary("test_ht.bin");

        REQUIRE(ht2.getSize() == 2);
        string out;
        ht2.get("key1", out);
        REQUIRE(out == "val1");
    }
}