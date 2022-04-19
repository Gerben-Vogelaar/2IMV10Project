#pragma once

//#include "src/newick/TreeNode.h"
#include "NewickTree.h"

#include <Vector>


class NewickTreeIndex {

public:


	NewickTreeIndex();
	NewickTreeIndex(Newick& tree);

	TreeNode* selectTreeNode(float xpos, float ypos, float cellHeight);

private:

	std::vector<std::vector<TreeNode*>> index;
	float heightNodes;
	float depthTree;

	/*
	* Returns a node from index iff a node was present for the given coords, NULL otherwise
	* runs in O(n) time
	*/
	

	void populateIndexing(TreeNode* n, int height);

	bool checkValueInQuad(float x, float y, TreeNode* treeNode);
	bool isLeft(Point2 a, Point2 b, Point2 c);

};