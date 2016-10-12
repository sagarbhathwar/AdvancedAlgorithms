main: string_utils.o main.o suffix_tree.o
	g++ string_utils.o main.o suffix_tree.o -o main

string_utils.o: string_utils.cpp string_utils.h
	g++ -std=c++14 -Wall -O3 -c string_utils.cpp

suffix_tree.o: suffix_tree.cpp suffix_tree.h
	g++ -std=c++14 -Wall -O3 -c suffix_tree.cpp

main.o: main.cpp string_utils.h suffix_tree.h
	g++ -std=c++14 -Wall -O3 -c main.cpp
