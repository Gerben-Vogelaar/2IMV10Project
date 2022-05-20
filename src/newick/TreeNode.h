#include <string>
#include <vector>

#include "src/misc/Point3.h"

using namespace std;

//TODO: change vector<TreeNode> to vector<&TreeNode> ??? -> is this faster?
struct TreeNode {
	string root_label;
	vector<TreeNode> descendant_list;
	string branch_length;


	bool sticky = false;
	int span;

	Point3 position; //upperleft corner rectangle (used in spaceReclaimingIciclePlot.cpp)

	void setPosition(Point3 newPos) {
		position = newPos;
	}

	int computeDepth(int depth) {
		int currentMax = depth;
		for (TreeNode n : descendant_list) {
			int dp = n.computeDepth(depth + 1);
			if (dp > currentMax) {
				currentMax = dp;
			}
		}

		return currentMax;
	}
};