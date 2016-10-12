#include "string_utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

bool multiple_spaces(char a, char b)
{
	return (a == b) && (a == ' ');
}

inline bool is_suffix(const string& pattern, int k, int q, char a)
{
	string next = pattern.substr(0, q);
	next.push_back(a);
	string curr = pattern.substr(0, k);
	if (curr.size() > next.size())
		return false;
	else
		return next.compare(next.size() - curr.size(), curr.size(), curr) == 0;
}

//copy elision optimization
vector<vector<int>> compute_transition_function(const string& pattern)
{
	int m = pattern.size();
	vector<vector<int>> transition_table(m + 1, vector<int>(ALPHABET, 0));
	for (int q = 0; q < m; ++q)
	{
		for (int a = 0; a < ALPHABET; ++a)
		{
			int k = min(m + 1, q + 2);
			do --k; while (!is_suffix(pattern, k, q, (char)a));
			transition_table[q][a] = k;
		}
	}

	return transition_table;
}

std::vector<int> finite_automaton_matcher(const std::string& text, const std::string& pattern)
{
	int m = pattern.size();
	vector<vector<int>> transition_table = compute_transition_function(pattern); 
	
	int q = 0;
	std::vector<int> res;
	for (int i = 0; i < text.size(); ++i)
	{
		q = transition_table[q][int(text[i])];
		if (q == m)
			res.push_back(i - m + 1);
	}

	return res;
}

vector<int> rabin_karp(const string& text, const string& pattern, int d, int q)
{
	int n = text.size();
	int m = pattern.size();
	int h = int(pow(d, m - 1)) % q;
	int p = 0;
	int t = 0;
	vector<int> indices;
	for (int i = 0; i < m; ++i)
	{
		p = (d * p + (pattern[i] - '0')) % q;
		t = (d * t + (text[i] - '0')) % q;
	}
	for (int s = 0; s < n - m; ++s)
	{
		if (p == t)
		{
			int j = 0;
			while (j < m && pattern[j] == text[s + j]) 
				++j;

			if (j == m) 
				indices.push_back(s);
		}
		if (s < n - m - 1)
			t = (d * (t - (text[s] - '0') * h) + (text[s + m] - '0')) % q;
	}

	return indices;
}

vector<int> compute_prefix_function(const string& pattern)
{
	int m = pattern.size();
	vector<int> prefix_function(m, -1);
	prefix_function[0] = 0;
	int k = 0;
	for (int q = 1; q < m; ++q)
	{
		while (k > 0 && pattern[k] != pattern[q])
			k = prefix_function[k];
		if (pattern[k] == pattern[q])
			++k;
		prefix_function[q] = k;
	}
	return prefix_function;
}

vector<int> kmp(const string& text, const string& pattern)
{
	int n = text.size();
	int m = pattern.size();
	vector<int> prefix_function = compute_prefix_function(pattern);
	vector<int> indices;
	int q = 0;
	for (int i = 0; i < n; ++i)
	{
		while (q > 0 && pattern[q] != text[i])
			q = prefix_function[q];
		if (pattern[q] == text[i])
			++q;
		if (q == m)
		{
			indices.push_back(i - m + 1);
			q = prefix_function[q - 1];
		}
	}

	return indices;
}

int find_length_of_text(const std::string& filename)
{
	//read file
	ifstream file(filename);
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

	//store urls in a vector of strings
	vector<string> urls;

	/* Remove the links here */
	for (int i : indices)
	{
		int j = 0;
		do ++j; while (!isspace(text[i + j]));
		urls.push_back(text.substr(i, j - 1));
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
		urls.push_back(text.substr(i, j - 1));
		text.erase(i, j);

		/* update indices after erasing a part of text*/
		for (int& x : indices) x = x - j;
	}

	ofstream op_file("AESOP_TALES_CLEANED.txt");
	op_file.write(text.c_str(), text.size());

	for_each(urls.begin(), urls.end(), [](auto s) {cout << s << endl; });

	return urls.size();
}

vector<int> find_pattern(const std::string& text, const std::string& pattern, int start, int end, std::vector<int>(*algorithm)(const std::string&, const std::string&))
{
	string substring = text.substr(start, end - start);
	return algorithm(substring, pattern);
}