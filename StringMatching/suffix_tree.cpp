#include "suffix_tree.h"
#include "string_utils.h"
#include <iostream>
#include <cassert>

Node* SuffixTree::get_new_node(int start, int* end)
{
	Node* node = new Node(start, end);
	node->suffix_link_ = root_;
	return node;
}

int SuffixTree::edge_length(Node* node)
{
	return *(node->end_) - (node->start_) + 1;
}

bool SuffixTree::walk_down(Node* curr_node)
{
	if (active_length_ >= edge_length(curr_node))
	{
		active_edge_ += edge_length(curr_node);
		active_length_ -= edge_length(curr_node);
		active_node_ = curr_node;
		return true;
	}
	return false;
}

void SuffixTree::extend_suffix_tree(int pos)
{
	leaf_end_ = pos;
	++remaining_suffix_count_;
	last_new_node_ = nullptr;

	while (remaining_suffix_count_ > 0)
	{
		if (active_length_ == 0) 
			active_edge_ = pos;

		if (active_node_->children_[text_[active_edge_]] == nullptr)
		{
			active_node_->children_[text_[active_edge_]] = get_new_node(pos, &leaf_end_);

			if (last_new_node_ != nullptr)
			{
				last_new_node_->suffix_link_ = active_node_;
				last_new_node_ = nullptr;
			}
		}

		else
		{
			Node* next = active_node_->children_[text_[active_edge_]];
			if (walk_down(next)) 
				continue;

			if (text_[next->start_ + active_length_] == text_[pos])
			{
				if (last_new_node_ != nullptr && active_node_ != root_)
				{
					last_new_node_->suffix_link_ = active_node_;
					last_new_node_ = nullptr;
				}
				++active_length_;
				break;
			}

			split_end_ = new int[1];
			*split_end_ = next->start_ + active_length_ - 1;

			Node* split = get_new_node(next->start_, split_end_);
			active_node_->children_[text_[active_edge_]] = split;

			split->children_[text_[pos]] = get_new_node(pos, &leaf_end_);
			next->start_ += active_length_;
			split->children_[text_[next->start_]] = next;

			if (last_new_node_ != nullptr) 
				last_new_node_->suffix_link_ = split;

			last_new_node_ = split;
		}

		--remaining_suffix_count_;

		if (active_node_ == root_ && active_length_ > 0)
		{
			--active_length_;
			active_edge_ = pos - remaining_suffix_count_ + 1;
		}

		else if (active_node_ != root_)
			active_node_ = active_node_->suffix_link_;
	}
}

void SuffixTree::set_suffix_index(Node* node, int label_height)
{
	if (node == nullptr) return;

	bool leaf = true;
	for (int i = 0; i < ALPHABET; ++i)
	{
		if (node->children_[i] != nullptr)
		{

			leaf = false;
			set_suffix_index(node->children_[i], label_height + edge_length(node->children_[i]));
		}
	}

	if (leaf)
		node->suffix_index_ = size_ - label_height;
}

SuffixTree::SuffixTree(const std::string& text)
{
	text_ = text;
	size_ = text.size();
	root_end_ = new int[1];
	*root_end_ = -1;
	root_ = get_new_node(-1, root_end_);
	active_node_ = root_;
	for (int i = 0; i < size_; ++i)
		extend_suffix_tree(i);
	set_suffix_index(root_, 0);
}

void SuffixTree::print_tree_(Node* node, int height)
{
	if (node == nullptr) return;

	if (node->start_ != -1) 
		std::cout << text_.substr(node->start_, *(node->end_) - node->start_ + 1);

	bool leaf = true;
	for (int i = 0; i < ALPHABET; ++i)
	{
		if (node->children_[i] != nullptr)
		{
			if(leaf && (node->start_ != -1))
				std::cout << "[" << node->suffix_index_ << "]" << std::endl;

			leaf = false;
			print_tree_(node->children_[i], height + edge_length(node->children_[i]));
		}
	}

	if (leaf)
		std::cout << "[" << node->suffix_index_ << "]" << std::endl;
}

void SuffixTree::print_tree()
{
	print_tree_(root_, 0);
}