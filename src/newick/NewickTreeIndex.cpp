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
	std::vector<Point2> points = { treeNode->point1, treeNode->point2, treeNode->point3, treeNode->point4 };

	std::vector<Point2> pointsUp;
	std::vector<Point2> pointsDown;

	for (Point2 pn : points) {
		if (pn.y > y) {
			pointsUp.push_back(pn);
		}
		else {
			pointsDown.push_back(pn);
		}
	}

	if (pointsUp.size() != 2 || pointsDown.size() != 2) {
		cout << "something went wrong calculating points in quad" << endl;
		return false;
	}

	Point2 p1;
	Point2 p2;
	Point2 p3;
	Point2 p4;

	if (pointsDown.at(0).x < pointsDown.at(1).x) {
		p1 = pointsDown.at(0);
		p2 = pointsDown.at(1);
	} else {
		p1 = pointsDown.at(1);
		p2 = pointsDown.at(0);
	}

	if (pointsUp.at(0).x > pointsUp.at(1).x) {
		p3 = pointsUp.at(0);
		p4 = pointsUp.at(1);
	} else {
		p4 = pointsUp.at(1);
		p3 = pointsUp.at(0);
	}

	bool rightP1P4 = !isLeft(p1, p4, Point2(x, y));;
	bool leftP2P3 = isLeft(p2, p3, Point2(x, y));

	//sort points in treeNode since the order is unknown
	
	/*bool rightP1P4 = !isLeft(treeNode->point1, treeNode->point4, Point2(x, y));;
	bool leftP2P3 = isLeft(treeNode->point2, treeNode->point3, Point2(x, y));*/

	return rightP1P4 && leftP2P3;
}

bool NewickTreeIndex::isLeft(Point2 a, Point2 b, Point2 c) {
	return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x)) > 0;
}
