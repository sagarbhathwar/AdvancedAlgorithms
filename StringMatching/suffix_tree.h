#pragma once
#include <iostream>
#include <string>

class Edge
{
public:
	friend class SuffixTree;
private:
	int start_;
	int end_;
	Edge(int start, int end) : start_(start), end_(end) {}
	std::pair<int, int> get_index_range() { return{ start_, end_ }; }
};

class SuffixTree
{
private:
	std::string text_;
private:
	SuffixTree(const std::string& text);
};