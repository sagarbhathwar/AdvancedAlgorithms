#pragma once
#include <string>
#include <vector>

const int ALPHABET = 256;

bool multiple_spaces(char a, char b);

void compute_transition_function(const std::string& pattern, std::vector<std::vector<int>>& transition_table);

std::vector<int> finite_automaton_matcher(const std::string& text, const std::vector<std::vector<int>>& transition_table, int m);

void rabin_karp(const std::string& text, const std::string& pattern, int d, int q);

std::vector<int> compute_prefix_function(const std::string& pattern);

void kmp(const std::string& text, const std::string& pattern);
