#include "src/newick/NewickTreeIndex.h"

NewickTreeIndex::NewickTreeIndex() : heightNodes(0) {};

NewickTreeIndex::NewickTreeIndex(Newick& tree)
{
	int tree_depth = tree.getSourceNode().calculateDepth(1, tree.getSourceNode());

	std::vector<std::vector<TreeNode*>> matrix(tree_depth, std::vector<TreeNode*>());
	index = matrix;
	depthTree = tree_depth;

	populateIndexing(tree.getSourceNodePointer(), 0);
}

void NewickTreeIndex::populateIndexing(TreeNode* n, int height)
{
	index.at(height).push_back(n);

	for (TreeNode& node : n->descendant_list) {
		populateIndexing(&node, height + 1);
	}
}

TreeNode* NewickTreeIndex::selectTreeNode(float xpos, float ypos, float cellHeight)
{
	if (cellHeight == 0.0f) {
		return NULL;
	}

	int layerIndex = std::floor(ypos / cellHeight);

	if (layerIndex  >= depthTree || layerIndex < 0) {
		return NULL;
	}

	for (TreeNode* n : index.at(layerIndex)) {
		if (checkValueInQuad(xpos, ypos, n)) {
			return n;
		}
	}

	return NULL;
}

bool NewickTreeIndex::checkValueInQuad(float x, float y, TreeNode* treeNode)
{
	bool rightP1P4 = !isLeft(treeNode->point1, treeNode->point4, Point2(x, y));;
	bool leftP2P3 = isLeft(treeNode->point2, treeNode->point3, Point2(x, y));

	return rightP1P4 && leftP2P3;
}

bool NewickTreeIndex::isLeft(Point2 a, Point2 b, Point2 c) {
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) > 0;
}
