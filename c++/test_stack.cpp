#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "stack.hpp"

TEST_CASE("Тестирование класса Stack", "[stack]") {
    Stack s;

    SECTION("Изначально стек пуст") {
        REQUIRE(s.isEmpty() == true);
        REQUIRE(s.getSize() == 0);
    }

    SECTION("Добавление элементов (push)") {
        s.push("apple");
        s.push("banana");
        REQUIRE(s.isEmpty() == false);
        REQUIRE(s.getSize() == 2);
    }

    SECTION("Извлечение элементов (pop) и порядок LIFO") {
        s.push("first");
        s.push("second");
        
        std::string out;
        REQUIRE(s.pop(out) == true);
        REQUIRE(out == "second");
        
        REQUIRE(s.pop(out) == true);
        REQUIRE(out == "first");
        
        REQUIRE(s.pop(out) == false);
    }

    SECTION("Просмотр верхнего элемента без удаления (get)") {
        s.push("top");
        std::string out;
        REQUIRE(s.get(out) == true);
        REQUIRE(out == "top");
        REQUIRE(s.getSize() == 1);
    }

    SECTION("Сохранение и загрузка стека (бинарный формат)") {
        s.push("apple");
        s.push("banana");
        s.saveToBinary("test_stack.bin");

        Stack s2;
        s2.loadFromBinary("test_stack.bin");

        std::string out;
        REQUIRE(s2.getSize() == 2);
        s2.pop(out);
        REQUIRE(out == "banana");
        s2.pop(out);
        REQUIRE(out == "apple");
    }
    SECTION("Обработка ошибки при сохранении (неверный путь)") {
        Stack s;
        s.push("test");
        s.saveToBinary("/nonexistent_folder/error.bin"); 
    }

    SECTION("Загрузка в непустой стек (проверка очистки)") {
        Stack s;
        s.push("old_data"); // Делаем стек непустым
        
        s.saveToBinary("test_stack.bin"); 
        
        s.loadFromBinary("test_stack.bin"); 
        
        REQUIRE(s.getSize() == 1); 
    }
}