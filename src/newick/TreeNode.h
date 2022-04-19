#include <string>
#include <vector>

#include "src/misc/Point2.h"
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

	//Point 1 - upper left point, other points rotate clockwise 
	Point2 point1;
	Point2 point2;
	Point2 point3;
	Point2 point4;

	void setPosition(Point3 newPos) {
		position = newPos;
	}

	void setPoints(Point2 p1, Point2 p2, Point2 p3, Point2 p4) {
		point1 = p1;
		point2 = p2;
		point3 = p3;
		point4 = p4;
	}

	/*
	* Initialize with c_depth = 1 -> first element has depth + 1;
	*/
	int calculateDepth(int c_depth, TreeNode node) {
		
		int maxDepth = c_depth;

		for (TreeNode node : node.descendant_list) {
			int nodeDepth = calculateDepth(c_depth + 1, node);
			
			if (nodeDepth > maxDepth) {
				maxDepth = nodeDepth;
			}
		}

		return maxDepth;
	}

	void print() {
		cout << point1.x << " " << point1.y << " " << point2.x << " " << point2.y << " " << point3.x << " " << point3.y << " " << point4.x << " " << point4.y << endl;
		cout << root_label << " " << descendant_list.size() << " " << endl;
	}
};