#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"
#include "avltree.hpp"

TEST_CASE("Тестирование класса AVLTree", "[avltree]") {
    AVLTree tree;
    int out;

    SECTION("Пустое дерево") {
        REQUIRE(tree.isEmpty() == true);
        REQUIRE(tree.getSize() == 0);
        REQUIRE(tree.search(10) == false);
        REQUIRE(tree.del(10, out) == false);
        
        tree.printInorder();
        tree.printPreorder();
        tree.printPostorder();
        tree.printBFS(); // Проверка BFS на пустом дереве
    }

    SECTION("Вставка и обходы") {
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.insert(2);
        tree.insert(7);
        
        REQUIRE(tree.getSize() == 5);
        REQUIRE(tree.search(10) == true);
        
        tree.printInorder();   // 2 5 7 10 15
        tree.printPreorder();  // 10 5 2 7 15
        tree.printPostorder(); // 2 7 5 15 10
        tree.printBFS();       // 10 5 15 2 7
    }

    SECTION("Проверка поворотов (Балансировка)") {
        // LL (Right Rotation)
        tree.insert(30); tree.insert(20); tree.insert(10);
        
        // RR (Left Rotation)
        AVLTree tree2;
        tree2.insert(10); tree2.insert(20); tree2.insert(30);

        // LR (Left-Right Rotation)
        AVLTree tree3;
        tree3.insert(30); tree3.insert(10); tree3.insert(20);

        // RL (Right-Left Rotation)
        AVLTree tree4;
        tree4.insert(10); tree4.insert(30); tree4.insert(20);
    }

    SECTION("Удаление элементов (с проверкой возврата значения)") {
        tree.insert(10); tree.insert(5); tree.insert(15); tree.insert(2); tree.insert(7);
        
        REQUIRE(tree.del(2, out) == true);
        REQUIRE(out == 2);
        
        REQUIRE(tree.del(5, out) == true);
        REQUIRE(out == 5);
        
        REQUIRE(tree.del(10, out) == true);
        REQUIRE(out == 10);
        
        REQUIRE(tree.del(99, out) == false);
        
        tree.del(7, out);
        tree.del(15, out);
        REQUIRE(tree.isEmpty() == true);
    }

    SECTION("Дубликаты") {
        tree.insert(10);
        tree.insert(10);
        REQUIRE(tree.getSize() == 1); 
    }

    SECTION("Сохранение и загрузка АВЛ-дерева (бинарный формат)") {
        tree.insert(10);
        tree.insert(5);
        tree.insert(15);
        tree.saveToBinary("test_avl.bin");

        AVLTree tree2;
        tree2.loadFromBinary("test_avl.bin");

        REQUIRE(tree2.getSize() == 3);
        REQUIRE(tree2.search(10) == true);
        REQUIRE(tree2.search(5) == true);
        REQUIRE(tree2.search(99) == false);
    }
}