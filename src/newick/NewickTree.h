//@author Gerben Vogelaar 1016370 TU/e

//Using Netwick's Tree Format Standart: https://evolution.genetics.washington.edu/phylip/newick_doc.html

#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "TreeNode.h"

using namespace std;

class Newick
{
private: 
	TreeNode tree;
	int treeSize; 

	//possible other things required for easy drawing later on

	void newickParseTree(char* newickTree, int& index, TreeNode& node);
	void newickParseDescendantList(char* newickTree, int& index, TreeNode& node);
	void newickParseSubTree(char* newickTree, int& index, TreeNode& node);
	void Parse(string newickTree);

public:	
	Newick(string newickTree);
	TreeNode* getSourceNodePointer();
	TreeNode& getSourceNodeRef();
	TreeNode getSourceNode();

	int getTreeSize();
	void printStatistics();
};



