use std::time::Instant;
use rust::array::Array;
use rust::stack::Stack;
use rust::queue::Queue;
use rust::flist::ForwardList;
use rust::dlist::List;
use rust::hash::HashTable;
use rust::cuckoo::CuckooTable;
use rust::avltree::AVLTree;

const N: usize = 100000;

fn main() {
    println!(" Бенчмарк структур данных на Rust (N = {}) \n", N);

    let mut arr = Array::new();
    let start = Instant::now();
    for i in 0..N {
        arr.push(&i.to_string());
    }
    println!(" Массив (Array) ");
    println!("Вставка: {:?}\n", start.elapsed());

    let mut stack = Stack::new();
    let start = Instant::now();
    for i in 0..N {
        stack.push(&i.to_string());
    }
    println!(" Стек (Stack) ");
    println!("Вставка: {:?}\n", start.elapsed());

    let mut queue = Queue::new();
    let start = Instant::now();
    for i in 0..N {
        queue.push(&i.to_string());
    }
    println!(" Очередь (Queue) ");
    println!("Вставка: {:?}\n", start.elapsed());

    let mut flist = ForwardList::new();
    let start = Instant::now();
    for i in 0..N {
        flist.push_tail(&i.to_string());
    }
    println!(" Односвязный список (ForwardList) ");
    println!("Вставка: {:?}\n", start.elapsed());

    let mut dlist = List::new();
    let start = Instant::now();
    for i in 0..N {
        dlist.push_tail(&i.to_string());
    }
    println!(" Двусвязный список (List) ");
    println!("Вставка: {:?}\n", start.elapsed());

    let mut ht = HashTable::new();
    let start = Instant::now();
    for i in 0..N {
        ht.add(&i.to_string(), "val");
    }
    let insert_time = start.elapsed();

    let start = Instant::now();
    for i in 0..N {
        ht.get(&i.to_string());
    }
    let search_time = start.elapsed();
    println!(" Хеш-таблица (Цепочки) ");
    println!("Вставка: {:?}", insert_time);
    println!("Поиск:   {:?}\n", search_time);

    let mut cuckoo = CuckooTable::new();
    let start = Instant::now();
    for i in 0..N {
        cuckoo.add(&i.to_string(), "val");
    }
    let insert_time = start.elapsed();

    let start = Instant::now();
    for i in 0..N {
        cuckoo.get(&i.to_string());
    }
    let search_time = start.elapsed();
    println!(" Хеш-таблица (Кукушка) ");
    println!("Вставка: {:?}", insert_time);
    println!("Поиск:   {:?}\n", search_time);

    let mut tree = AVLTree::new();
    let start = Instant::now();
    for i in 0..N {
        tree.insert(i as i32);
    }
    let insert_time = start.elapsed();

    let start = Instant::now();
    for i in 0..N {
        tree.search(i as i32);
    }
    let search_time = start.elapsed();
    println!(" АВЛ-дерево (AVLTree) ");
    println!("Вставка: {:?}", insert_time);
    println!("Поиск:   {:?}\n", search_time);
}