build:
	rm -f tests.exe
	g++ -std=c++14 tests.cpp -o tests.exe -lgtest -lgtest_main -lpthread
	
run:
	./tests.exe

leaks:
	leaks --atExit -- ./tests.exe

tests:
	cmake --build build
	cd build && ctest