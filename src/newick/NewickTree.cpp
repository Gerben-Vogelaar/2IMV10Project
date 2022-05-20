//@author Gerben Vogelaar TU/e 1016370

//TODO: implement the comment part of the standart, now only non commented versions are accepted.

//strcpy is deprecated, this removes the warming
#define _CRT_SECURE_NO_WARNINGS 

#include "NewickTree.h"

Newick::Newick(string newickTree): treeSize(1) {
	Parse(newickTree);

	computeDepth();
}

void Newick::Parse(string newickTree)
{
	char* newWickTreeC = new char[newickTree.size() + 1];

	strcpy(newWickTreeC, newickTree.c_str());
	
	TreeNode source;
	int index = 0;

	newickParseTree(newWickTreeC, index, source);

	this->tree = source;
	cout << "Parsed tree successfull" << endl;
}

void Newick::computeDepth()
{
	depth = tree.computeDepth(0);
}

TreeNode Newick::getSourceNode()
{
	return tree;
}

TreeNode& Newick::getSourceNodeRef()
{
	return tree;
}

TreeNode* Newick::getSourceNodePointer()
{
	return &tree;
}

int Newick::getTreeSize()
{
	return this->treeSize;
}

void Newick::printStatistics()
{
	cout << "Tree size: " << getTreeSize() << " - Tree depth: " << getSourceNode().calculateDepth(1, getSourceNode()) << endl;
}

//tree ==> descendant_list [ root_label ] [ : branch_length ] ;
void Newick::newickParseTree(char* newickTree, int& index, TreeNode& node) {
	//1. parse descendant_list
	if (newickTree[index] != '(') {
		cout << "Faulty tree" << endl;
		return;
	}
	newickParseDescendantList(newickTree, index, node);
}

//descendant_list ==> ( subtree { , subtree } )
void Newick::newickParseDescendantList(char* newickTree, int& index, TreeNode& node) {
	while (newickTree[++index] != ')') {

		if (newickTree[index] == ',') {
			index++;
		}
		newickParseSubTree(newickTree, index, node);
	}

	//!SEMI-CODE DUPLICATION!!!! from newickParseSubTree!
	//parse the Descendantlist label and branch_length
	bool branch_length = false;
	while (newickTree[++index] != ',' && newickTree[index] != ')') {
		if (newickTree[index] == ';') {
			return;
		}
		else if (newickTree[index] == ':') {
			branch_length = true;
		}
		else if (branch_length) {
			node.branch_length += newickTree[index];
		}
		else {
			node.root_label += newickTree[index];
		}
	}
	index--;
}
// subtree	==> descendant_list[internal_node_label][: branch_length]
//			==> leaf_label[: branch_length]
void Newick::newickParseSubTree(char* newickTree, int& index, TreeNode& node) {

	// descendant_list
	if (newickTree[index] == '(') {
		TreeNode newNode = TreeNode();
		newickParseDescendantList(newickTree, index, newNode);
		node.descendant_list.push_back(newNode);
		this->treeSize++;
	}
	// leaf_label
	else {

		TreeNode newNode = TreeNode();

		bool branch_length = false;
		while (newickTree[index] != ',' && newickTree[index] != ')') {
			if (newickTree[index] == ':') {
				branch_length = true;
			}			
			else if (branch_length) {
				newNode.branch_length += newickTree[index];
			}
			else {
				newNode.root_label += newickTree[index];
			}
			index++;
		}
		index--;

		node.descendant_list.push_back(newNode);
		this->treeSize++;
	}
}