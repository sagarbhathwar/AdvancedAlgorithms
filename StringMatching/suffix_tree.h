#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "string_utils.h"

class SuffixTreeNode
{
	friend class SuffixTree;
private:
	std::vector<SuffixTreeNode*> children_;
	SuffixTreeNode* suffix_link_;
	int start_;
	int* end_;
	int suffix_index_;
	SuffixTreeNode(int start, int* end)
	{
		children_ = std::vector<SuffixTreeNode*>(ALPHABET, nullptr);
		start_ = start;
		end_ = end;
		suffix_index_ = -1;
	}
};

typedef SuffixTreeNode Node;

class SuffixTree
{
private:
	std::string text_;
	Node* root_ = nullptr;
	Node* last_new_node_ = nullptr;
	Node* active_node_ = nullptr;

	int active_edge_ = -1;
	int active_length_ = 0;

	int remaining_suffix_count_ = 0;
	int leaf_end_ = -1;
	int* root_end_ = nullptr;
	int* split_end_ = nullptr;
	int size_;

	Node* get_new_node(int start, int* end);

	int edge_length(Node* node);

	bool walk_down(Node* curr_node);

	void extend_suffix_tree(int pos);

	void set_suffix_index(Node* node, int label_height);

	void print_tree_(Node* node, int height);

public:
	SuffixTree(const std::string& text);

	void print_tree();
};