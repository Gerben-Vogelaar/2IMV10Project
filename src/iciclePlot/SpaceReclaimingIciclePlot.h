#include "src/newick/NewickTree.h"
#include "src/misc/Point2.h"
#include "SRIParg.h"
	
class SpaceReclaimingIciclePlot {

public:
	SpaceReclaimingIciclePlot(Newick& newickTree, SRIP1_arg arg, bool expirimental);
	SpaceReclaimingIciclePlot(Newick& newickTree, SRIP1_arg arg, bool expirimental, int multVector);
	SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg, bool expirimental);
	SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg, bool expirimental, int multVector);

	/*
	* W: width of diagram 
	* h: height of a layer in the diagram
	* gamma: horizontal gap between nodes
	* rho: space reclaiming parameter, rho \in [0,1]
	* r: root of a hierarchy
	*/
	float* getVertexDataArray();

	int getVertexDataArraySize();

private:
	float* vertexDataArray;
	int sizeVertexDataArray;

	//TODO: make the algorithms more modular, now most algorithms do the same with the exception of an small added code segment or a different vertex generation call.

	void SRIP1_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index);
	void SRIP1_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index);

	void SRIP1_s_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP1_s_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector);
	
	void SRIP1Expirimental_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index);
	void SRIP1Expirimental_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset);

	//quadrangle Draw
	void SRIP1Expirimental_initQ(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP1Expirimental_rQ(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector);
	
	//quadrangle - horizontal Draw
	void SRIP1Expirimental_initQ_h(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP1Expirimental_rQ_h(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector);


	void SRIP1_s_expirimental_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP1_s_expirimental_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, int multVector);

	void displaceQuadranglesX(const float x, float* vertexData, int sizeVertexData);
	void displaceQuadranglesY(const float y, float* vertexData, int sizeVertexData);

	//SRIP2_ methods:
	void SRIP2_init(SRIP2_arg arg, Newick& tree, float* vertexData, int& index, int multVector);
	void SRIP2_r(int d, vector<TreeNode> P, int m, float A, float w, float g, SRIP2_arg arg, float* vertexData, int& index, int multVector);

	float Weight(const vector<TreeNode> C);
	float Weight(const TreeNode c);

	//Util:
	void drawQuadrangleByTriangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4);
	void drawQuadrangleByTriangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector);
	void drawQuadrangleByQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4);
	void drawQuadrangleByQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector);
	void drawQuadrangleByQuadrangleHorizontal(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector);
};