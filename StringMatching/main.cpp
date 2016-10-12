#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include "string_utils.h"

using namespace std;

int main()
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
	int m = pattern.size();
	vector<vector<int>> transition_table(m + 1, vector<int>(ALPHABET, 0));

	compute_transition_function(pattern, transition_table);
	std::vector<int> indices = finite_automaton_matcher(text, transition_table, m);

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
	m = pattern.size();
	transition_table = vector<vector<int>>(m + 1, vector<int>(ALPHABET, 0));

	compute_transition_function(pattern, transition_table);
	indices = finite_automaton_matcher(text, transition_table, m);
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

}
