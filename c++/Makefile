CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude --coverage
LDFLAGS = --coverage

all: test_all

stack.o: src/stack.cpp
	$(CXX) $(CXXFLAGS) -c src/stack.cpp -o stack.o

array.o: src/array.cpp
	$(CXX) $(CXXFLAGS) -c src/array.cpp -o array.o

catch.o: src/catch_amalgamated.cpp
	$(CXX) $(CXXFLAGS) -c src/catch_amalgamated.cpp -o catch.o

test_stack.o: test_stack.cpp
	$(CXX) $(CXXFLAGS) -c test_stack.cpp -o test_stack.o

test_array.o: test_array.cpp
	$(CXX) $(CXXFLAGS) -c test_array.cpp -o test_array.o

flist.o: src/flist.cpp
	$(CXX) $(CXXFLAGS) -c src/flist.cpp -o flist.o

test_flist.o: test_flist.cpp
	$(CXX) $(CXXFLAGS) -c test_flist.cpp -o test_flist.o

list.o: src/list.cpp
	$(CXX) $(CXXFLAGS) -c src/list.cpp -o list.o

test_list.o: test_list.cpp
	$(CXX) $(CXXFLAGS) -c test_list.cpp -o test_list.o

queue.o: src/queue.cpp
	$(CXX) $(CXXFLAGS) -c src/queue.cpp -o queue.o

test_queue.o: test_queue.cpp
	$(CXX) $(CXXFLAGS) -c test_queue.cpp -o test_queue.o

hashtable.o: src/hashtable.cpp
	$(CXX) $(CXXFLAGS) -c src/hashtable.cpp -o hashtable.o

test_hashtable.o: test_hashtable.cpp
	$(CXX) $(CXXFLAGS) -c test_hashtable.cpp -o test_hashtable.o

avltree.o: src/avltree.cpp
	$(CXX) $(CXXFLAGS) -c src/avltree.cpp -o avltree.o

test_avltree.o: test_avltree.cpp
	$(CXX) $(CXXFLAGS) -c test_avltree.cpp -o test_avltree.o

cuckoo.o: src/cuckoo.cpp
	$(CXX) $(CXXFLAGS) -c src/cuckoo.cpp -o cuckoo.o

test_cuckoo.o: test_cuckoo.cpp
	$(CXX) $(CXXFLAGS) -c test_cuckoo.cpp -o test_cuckoo.o

test_all: stack.o array.o flist.o list.o queue.o hashtable.o avltree.o cuckoo.o catch.o test_stack.o test_array.o test_flist.o test_list.o test_queue.o test_hashtable.o test_avltree.o test_cuckoo.o
	$(CXX) $(LDFLAGS) $^ -o test_all


run: test_all
	./test_all

coverage: run
	gcov -b -c stack.o array.o flist.o list.o queue.o hashtable.o avltree.o cuckoo.o
	lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch,source
	lcov --remove coverage.info '/usr/*' '*catch_amalgamated*' '*include*' --output-file coverage_filtered.info --ignore-errors mismatch,source
	genhtml coverage_filtered.info --output-directory coverage_report --ignore-errors mismatch,source

lint:
	cppcheck --enable=all --inconclusive --quiet -Iinclude \
	--suppress=missingIncludeSystem \
	--suppress=unusedFunction \
	--suppress=useStlAlgorithm \
	--suppress=functionStatic \
	--suppress=constVariablePointer \
	src/stack.cpp src/array.cpp src/flist.cpp src/list.cpp src/queue.cpp src/hashtable.cpp src/avltree.cpp src/cuckoo.cpp
	@echo "Линтер завершил проверку. Ошибок не найдено"

clean:
	rm -f test_all *.o *.gcda *.gcno *.gcov *.info
	rm -rf coverage_report

benchmark: src/stack.cpp src/array.cpp src/flist.cpp src/list.cpp src/queue.cpp src/hashtable.cpp src/cuckoo.cpp src/avltree.cpp benchmark.cpp
	$(CXX) -std=c++17 -O2 -Iinclude $^ -o benchmark

run_benchmark: benchmark
	./benchmark

.PHONY: all run coverage lint clean benchmark run_benchmark