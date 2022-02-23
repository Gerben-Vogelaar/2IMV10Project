#include <string>
#include <vector>

#include "src/misc/Point3.h"

using namespace std;

//TODO: change vector<TreeNode> to vector<&TreeNode> ??? -> is this faster?
struct TreeNode {
	string root_label;
	vector<TreeNode> descendant_list;
	string branch_length;

	Point3 position;

	void setPosition(Point3 newPos) {
		position = newPos;
	}
};