#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "array.hpp"
#include <string>
using namespace std;

TEST_CASE("Тестирование класса Array", "[array]") {
    Array a;
    string out;

    SECTION("Изначально пустой и capacity = 4") {
        REQUIRE(a.isEmpty() == true);
        REQUIRE(a.getSize() == 0);
        REQUIRE(a.getCapacity() == 4);
    }

    SECTION("Добавление в конец (push)") {
        a.push("apple");
        a.push("banana");
        REQUIRE(a.getSize() == 2);
        REQUIRE(a.isEmpty() == false);
        
        REQUIRE(a.get(0, out) == true);
        REQUIRE(out == "apple");
        REQUIRE(a.get(1, out) == true);
        REQUIRE(out == "banana");
    }

    SECTION("Расширение массива (resize)") {
        a.push("1");
        a.push("2");
        a.push("3");
        a.push("4");
        REQUIRE(a.getCapacity() == 4);
        
        a.push("5");
        REQUIRE(a.getCapacity() == 8);
        REQUIRE(a.getSize() == 5);
    }

    SECTION("Добавление по индексу (pushAt)") {
        a.push("a");
        a.push("c");
        a.pushAt("b", 1); 
        
        REQUIRE(a.getSize() == 3);
        a.get(1, out);
        REQUIRE(out == "b");
    }

    SECTION("Замена по индексу (set)") {
        a.push("hello");
        a.set(0, "world");
        a.get(0, out);
        REQUIRE(out == "world");
    }

    SECTION("Удаление по индексу (del)") {
        a.push("one");
        a.push("two");
        a.push("three");
        
        REQUIRE(a.del(1, out) == true);
        REQUIRE(out == "two");
        REQUIRE(a.getSize() == 2);
        
        a.get(1, out);
        REQUIRE(out == "three");
    }

    SECTION("Обработка выхода за границы") {
        Array empty_arr; 
        
        REQUIRE(empty_arr.get(0, out) == false); 
        REQUIRE(empty_arr.del(0, out) == false);
    }
    SECTION("Обработка ошибок при неверном индексе") {
        Array a;
        a.push("test");

        a.pushAt("bad", -1);  
        a.pushAt("bad", 10);  
        
        a.set(-1, "bad");
        a.set(10, "bad");
        
        REQUIRE(a.getSize() == 1);
    }
    SECTION("Сохранение и загрузка массива (бинарный формат)") {
        Array a;
        a.push("Moscow");
        a.push("London");
        a.push("Paris");
        a.saveToBinary("test_array.bin");

        Array a2;
        a2.loadFromBinary("test_array.bin");

        REQUIRE(a2.getSize() == 3);
        
        string out;
        REQUIRE(a2.get(0, out) == true);
        REQUIRE(out == "Moscow");
        
        REQUIRE(a2.get(2, out) == true);
        REQUIRE(out == "Paris");
        
        a2.push("Berlin");
        REQUIRE(a2.getSize() == 4);
    }
}