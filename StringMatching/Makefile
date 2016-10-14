main.exe: string_utils.o main.o suffix_tree.o
	clang++ string_utils.o main.o suffix_tree.o -o main.exe

string_utils.o: string_utils.cpp string_utils.h
	clang++ -std=c++14 -Wall -O3 -mtune=core-avx2 -c string_utils.cpp

suffix_tree.o: suffix_tree.cpp suffix_tree.h
	clang++ -std=c++14 -Wall -O3 -mtune=core-avx2 -c suffix_tree.cpp

main.o: main.cpp string_utils.h suffix_tree.h
	clang++ -std=c++14 -Wall -O3 -mtune=core-avx2 -c main.cpp
