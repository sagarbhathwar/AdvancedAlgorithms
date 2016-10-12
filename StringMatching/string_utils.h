#pragma once
#include <string>
#include <vector>

const int ALPHABET = 256;

bool multiple_spaces(char a, char b);

std::vector<int> finite_automaton_matcher(const std::string& text, const std::string& pattern);

std::vector<int> rabin_karp(const std::string& text, const std::string& pattern, int d, int q);

std::vector<int> kmp(const std::string& text, const std::string& pattern);

int find_length_of_text(const std::string& filename);

std::vector<int> find_pattern(const std::string& text, const std::string& pattern, int start, int end, std::vector<int> (*algorithm)(const std::string&, const std::string&));
