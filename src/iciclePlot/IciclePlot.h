#include "src/newick/NewickTree.h"

class IciclePlot {

private:
	float* vertexDataArray;
	int sizeVertexDataArray;
	
	unsigned int* vertexDataIndicesArray;
	int sizeVertexDataIndicesArray;

	void calculateVertexIndexData(Newick& tree);

	void calculateVertexData(Newick& newickTree, float* vertexData);
	void calculateLayerVertex(TreeNode n, float x, float y, float width, float height, float* vertexData, int& index);

public:
	IciclePlot(Newick& newickTree);

	float* getVertexDataArray();
	unsigned int* getVertexDataIndicesArray();

	int getVertexDataArraySize();
	int getVertexDataIndicesArraySize();
};