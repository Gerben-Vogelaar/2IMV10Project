#include "IciclePlot.h"


//DONT forget to delete the new float[x] on object deletion
IciclePlot::IciclePlot(Newick& newickTree) {
	//8 vertex to define a square -> x,y for 4 points 
	this->sizeVertexDataArray = 12 * newickTree.getTreeSize();
	this->vertexDataArray = new float[sizeVertexDataArray];
	//we need 3 points to define a triangle, 6 for a square
	this->sizeVertexDataIndicesArray = 6 * newickTree.getTreeSize();
	this->vertexDataIndicesArray = new unsigned int[sizeVertexDataIndicesArray];

	calculateVertexData(newickTree, vertexDataArray);
	calculateVertexIndexData(newickTree);
}

void IciclePlot::calculateVertexData(Newick& newickTree, float* vertexData) {
	TreeNode currentNode = newickTree.getSourceNode();

	//1,1 is left upper corner
	int index = 0;

	calculateLayerVertex(currentNode, -1.0f, 1.0f, 2.0f, 0.1f, vertexDataArray, index);

	/*for (int i = 0; i < this->sizeVertexDataArray; i = i + 8) {
		cout <<
			"(" << vertexDataArray[i] << "," << vertexDataArray[i + 1] << "), " <<
			"(" << vertexDataArray[i + 2] << "," << vertexDataArray[i + 3] << "), " <<
			"(" << vertexDataArray[i + 4] << "," << vertexDataArray[i + 5] << "), " <<
			"(" << vertexDataArray[i + 6] << "," << float(vertexDataArray[i + 7]) << "), " <<
			endl;
	}*/

	for (int i = 0; i < sizeVertexDataArray; i = i + 2) {
		cout << vertexDataArray[i] << " " << vertexDataArray[i + 1] << endl;
	}
}

//TODO: work with fractions to remove the rounding errors which increase with each recursive call.
// e.g. return not new with as with/n.desc_list but the incremental n.desc_list
void IciclePlot::calculateLayerVertex(TreeNode n, float x, float y, float width, float height, float* vertexData, int& index)
{
	float cellWidth = width / float(n.descendant_list.size());

	//upper left corner of the drawing window is (-1, 1)

	//vertexData[index] = x;
	//vertexData[++index] = y;

	//vertexData[++index] = x + width;
	//vertexData[++index] = y;

	//vertexData[++index] = x;
	//vertexData[++index] = y - height;
	//
	//vertexData[++index] = x + width;
	//vertexData[++index] = y - height;

	float x2 = x + width;

	float y2 = y - height;
	
	vertexData[index++] = x;
	vertexData[index++] = y;

	vertexData[index++] = x2;
	vertexData[index++] = y;

	vertexData[index++] = x;
	vertexData[index++] = y2;
	
	vertexData[index++] = x2;
	vertexData[index++] = y;

	vertexData[index++] = x;
	vertexData[index++] = y2;

	vertexData[index++] = x2;
	vertexData[index++] = y2;

	y = y - height;

	//here we assume that the drawing order is the same as in the tree which is not guaranteed I think
	for (TreeNode desc : n.descendant_list) {
		calculateLayerVertex(desc, x, y, cellWidth, height, vertexData, index);
		x = x + cellWidth + 0.02f; //TODO: REMOVE 0.02f, is here for visual debugging
	}
}

void IciclePlot::calculateVertexIndexData(Newick& tree)
{
	for (int i = 0; i < tree.getTreeSize() * 6; i = i + 6) {
		vertexDataIndicesArray[i] = i;
		vertexDataIndicesArray[i+1] = i+1;
		vertexDataIndicesArray[i+2] = i+2;
		
		vertexDataIndicesArray[i+3] = i+1;
		vertexDataIndicesArray[i+4] = i+2;
		vertexDataIndicesArray[i+5] = i+3;
	}
}

float* IciclePlot::getVertexDataArray() {
	return this->vertexDataArray;
}
unsigned int* IciclePlot::getVertexDataIndicesArray() {
	return this->vertexDataIndicesArray;
}

int IciclePlot::getVertexDataArraySize() {
	return this->sizeVertexDataArray;
}
int IciclePlot::getVertexDataIndicesArraySize() {
	return this->sizeVertexDataIndicesArray;
}
