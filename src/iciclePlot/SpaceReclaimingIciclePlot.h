#pragma once

#include <chrono>   

#include "src/newick/NewickTree.h"
#include "src/newick/NewickTreeIndex.h"
#include "SRIParg.h"

using namespace std;

class SpaceReclaimingIciclePlot {

public:
	SpaceReclaimingIciclePlot(Newick& newickTree, SRIP1_arg arg, bool experimental, int multVector=1);
	//SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg, bool expirimental);
	SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg, bool experimental, int multVector=1);

	void drawQuadrangleHorizontal(float* vertexData, int& index, Point2& p1, Point2& p2, Point2& p3, Point2& p4, int multVector);

	/*
	* W: width of diagram
	* h: height of a layer in the diagram
	* gamma: horizontal gap between nodes
	* rho: space reclaiming parameter, rho \in [0,1]
	* r: root of a hierarchy
	*/

	/*
	* W: width diagram
	* h: height of a layer
	* epsilon [0,W]: max width of a node
	* gamma: max horizontal gap between nodes
	* rho [0,1]: lost space shrinking parameter
	* sigma [0,1]: sticky node shrinking factor
	* lambda: depth span of a sticky node
	*/

	float* getVertexDataArray();
	int getVertexDataArraySize();

	TreeNode* selectTreeNode(float x, float y);
	float cellHeight = 1.0f;

private:
	float* vertexDataArray;
	//number of vertex, not bytes
	int sizeVertexDataArray;

	//algorithms below are actually used in the code :
	//quadrangle Draw ----------------------------

	void SRIP1_initQ_h(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP1_rQ_h(int d, vector<TreeNode*> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, int multVector);

	void SRIP1Expirimental_initQ_h(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP1Expirimental_rQ_h(int d, vector<TreeNode*> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector);

	void displaceQuadranglesX(const float x, float* vertexData, int sizeVertexData);
	void displaceQuadranglesY(const float y, float* vertexData, int sizeVertexData);

	//SRIP2_ methods:
	/*void SRIP2_init(SRIP2_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP2_r(int d, vector<TreeNode> P, int m, float A, float w, float g, SRIP2_arg arg, float* vertexData, int& index, int multVector);*/

	void SRIP2_initQ_h(SRIP2_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP2_rQ_h(int d, vector<TreeNode*> P, int m, float A, float w, float g, SRIP2_arg arg, float* vertexData, int& index, int multVector);

	float Weight(const vector<TreeNode> C);
	float Weight(const vector<TreeNode*> C);
	float Weight(const TreeNode c);

	/* Drawing quadrangles multiple functions
	* Triangle generates 2 triangles (6 floats in vertexData)
	* Triangle with multVector generates 2 * multvector triangles
	*
	* Quadrangle generates 1 QUAD (4 float)
	* Quadrangle with multVecot generates multVector quads (from bottom to top base)
	* Quadrangle with multVecot Horizontal generates multVector quads where the quads sides are all vertical.
	*
	*/
	void drawQuadrangleByTriangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4);
	void drawQuadrangleByTriangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector);
	void drawQuadrangleByQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4);
	void drawQuadrangleByQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector);
	void drawQuadrangleByQuadrangleHorizontal(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector);
};