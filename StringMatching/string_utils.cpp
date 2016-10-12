#include "string_utils.h"

#include <iostream>
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

void compute_transition_function(const string& pattern, vector<vector<int>>& transition_table)
{
	int m = pattern.size();
	for (int q = 0; q < m; ++q)
	{
		for (int a = 0; a < ALPHABET; ++a)
		{
			int k = min(m + 1, q + 2);
			do --k; while (!is_suffix(pattern, k, q, (char)a));
			transition_table[q][a] = k;
		}
	}
}

std::vector<int> finite_automaton_matcher(const string& text, const vector<vector<int>>& transition_table, int m)
{
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

void rabin_karp(const string& text, const string& pattern, int d, int q)
{
	int n = text.size();
	int m = pattern.size();
	int h = (int)(pow(d, m - 1)) % q;
	int p = 0;
	int t = 0;
	for (int i = 0; i < m; ++i)
	{
		p = (d * p + (pattern[i] - '0')) % q;
		t = (d * t + (text[i] - '0')) % q;
	}
	for (int s = 0; s <= n - m; ++s)
	{
		if (p == t)
		{
			int j = 0;
			while (j < m && pattern[j] == text[s + j])
				++j;
			if (j == m)
				cout << "Pattern occurs with shift :" << s << "\n";
		}
		if (s < n - m)
		{
			t = (d * (t - (text[s] - '0') * h) + (text[s + m] - '0')) % q;
			if (t < 0)
				t += q;
		}
	}
}

vector<int> compute_prefix_function(const string& pattern)
{
	int m = pattern.size();
	vector<int> p(m, 0);
	int k = 0;
	for (int q = 1; q < m; ++q)
	{
		while (k > 0 && pattern[k] != pattern[q])
			k = p[k];
		if (pattern[k] == pattern[q])
			++k;
		p[q] = k;
	}
	return p;
}

void kmp(const string& text, const string& pattern)
{
	int n = text.size();
	int m = pattern.size();
	vector<int> p = compute_prefix_function(pattern);
	int q = 0;
	for (int i = 0; i < n; ++i)
	{
		while (q > 0 && pattern[q] != text[i])
			q = p[q];
		if (pattern[q] == text[i])
			++q;
		if (q == m)
		{
			cout << "Pattern occurs with shift : " << i - m + 1 << "\n";
			q = p[q];
		}
	}
}