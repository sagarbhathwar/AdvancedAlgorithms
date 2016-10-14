#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <set>
#include <algorithm>
#include <chrono>
#include "string_utils.h"
#include "suffix_tree.h"

using namespace std;

int main(int argc, char** argv)
{
	//read file
	ifstream file("AESOP_TALES.txt");
	string text;
	string line;
	while (getline(file, line))
	{
		text += line;
		text.push_back('\n');
	}

	//removing blank spaces
	string::iterator t = std::unique(text.begin(), text.end(), multiple_spaces);
	text.erase(t, text.end());

	//removing links for HTTP
	string pattern = "http";
	std::vector<int> indices = finite_automaton_matcher(text, pattern);

	/* Remove the links here */
	for (int i : indices)
	{
		int j = 0;
		do ++j; while (!isspace(text[i + j]));

		text.erase(i, j);

		/* update indices after erasing a part of text*/
		for (int& x : indices) x = x - j;
	}

	//remove links for FTP
	pattern = "ftp";
	indices = finite_automaton_matcher(text, pattern);
	for (int i : indices)
	{
		int j = 0;
		do ++j; while (!isspace(text[i + j]));

		text.erase(i, j);

		/* update indices after erasing a part of text*/
		for (int& x : indices) x = x - j;
	}

	ofstream op_file("AESOP_TALES_CLEANED.txt");
	op_file.write(text.c_str(), text.size());

	cout << find_length_of_text("AESOP_TALES.txt") << endl;

	indices = find_pattern(text, "night", 0, text.size(), kmp);

	for_each(indices.begin(), indices.end(), [](auto x) {cout << x << endl; });

	cout << indices.size() << endl;

	SuffixTree suffix_tree("abcababbababccbababbabcbbabbababbababababababbbabb");
	suffix_tree.print_tree();
}
