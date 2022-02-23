#include "src/newick/NewickTree.h"
#include "src/misc/Point2.h"

class SpaceReclaimingIciclePlot {

public:
	SpaceReclaimingIciclePlot(float W, float h, float gamma, float rho, Newick& newickTree);

	/*
	* W: width of diagram 
	* h: height of a layer in the diagram
	* gamma: horizontal gap between nodes
	* rho: space reclaiming parameter, rho \in [0,1]
	* r: root of a hierarchy
	*/
	void SRIP1_init(Newick& r);

	float* getVertexDataArray();

	int getVertexDataArraySize();

private:
	float* vertexDataArray;
	int sizeVertexDataArray;

	float W; //width diagram
	float h; //height of a layer in the diagram
	float gamma; // horizontal gap beween nodes
	float rho; // space reclaiming parameter; rho \in [0,1]
	 
	/*
	* P = List of all parent nodes at depth d-1 with attributes (x,y,w)
	* w = available horizontal width at depth d
	* m = number of child nodes at depth d, m > 0
	* d = current depth of the tree
	*/
	void SRIP1_r(int d, vector<TreeNode> P, int m, float w, float* vertexData, int& index);
	void drawQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4);
};