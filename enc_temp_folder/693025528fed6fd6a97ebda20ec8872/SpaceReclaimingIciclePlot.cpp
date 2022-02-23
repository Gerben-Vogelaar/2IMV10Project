#include "SpaceReclaimingIciclePlot.h"

SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(float W, float h, float gamma, float rho, Newick& newickTree) : W(W), h(h), gamma(gamma), rho(rho)
{
	this->sizeVertexDataArray = 12 * newickTree.getTreeSize();
	this->vertexDataArray = new float[sizeVertexDataArray];

	SRIP1_init(newickTree);

	for (int i = 0; i < this->sizeVertexDataArray; i = i + 12) {
		cout <<
			"(" << vertexDataArray[i] << "," << vertexDataArray[i + 1] << "), " <<
			"(" << vertexDataArray[i + 2] << "," << vertexDataArray[i + 3] << "), " <<
			"(" << vertexDataArray[i + 4] << "," << vertexDataArray[i + 5] << "), " <<
			"(" << vertexDataArray[i + 6] << "," << float(vertexDataArray[i + 7]) << "), " <<
			"(" << vertexDataArray[i + 8] << "," << float(vertexDataArray[i + 9]) << "), " <<
			"(" << vertexDataArray[i + 10] << "," << float(vertexDataArray[i + 11]) << "), " <<
			endl;
	}

}

void SpaceReclaimingIciclePlot::SRIP1_init(Newick & newickTree)
{
	int index = 0;

	//Point3 p = Point3(0.0f, 0.0f, 0.0f);

	drawQuadrangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(W, 0.0f), Point2(W, h), Point2(0.0f, h));

	TreeNode source = newickTree.getSourceNode();

	source.position = Point3(0.0f, h, W);

	if (source.descendant_list.size() > 0) {
		SRIP1_r(1, vector<TreeNode>{source}, source.descendant_list.size(), W, vertexDataArray, index);
	}

};

void SpaceReclaimingIciclePlot::SRIP1_r(int d, vector<TreeNode> P, int m, float w, float* vertexData, int& index) {

	float U = w - (m - 1.0f) * gamma;
	float x = (W - w) / 2.0f;
	float y = (d + 1.0f) * h;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	cout << "(" << d << "),(" << "(" << m << "),(" << "(" << w << "),(" << U << "),(" << x << "),(" << y << "),(" << wp << "),(" << mp << ")" << endl;

	for (TreeNode p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode c : *(&p.descendant_list)) {
			Point2 p1 = p0.add(Point2(p.position.z, 0).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			drawQuadrangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y));
			c.setPosition(Point3(x, y, delta));
			x += delta + gamma;
			p0 = p1;
			int n = c.descendant_list.size();
			if (n > 0) {
				Pp.push_back(c);
				wp = wp + delta;
				mp = mp + n;
			}
		}
	}

	vector<TreeNode> C;

	for (TreeNode p : P) {

	C.insert(C.end(), p.descendant_list.begin(), p.descendant_list.end());
	}

	if (mp > 0) {
		SRIP1_r(d + 1, C, mp, wp + rho*(W - wp), vertexData, index);
	}
}

void SpaceReclaimingIciclePlot::drawQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4) {
	vertexData[index++] = p1.x;
	vertexData[index++] = p1.y;

	vertexData[index++] = p2.x;
	vertexData[index++] = p2.y;

	vertexData[index++] = p3.x;
	vertexData[index++] = p3.y;

	vertexData[index++] = p1.x;
	vertexData[index++] = p1.y;

	vertexData[index++] = p3.x;
	vertexData[index++] = p3.y;

	vertexData[index++] = p4.x;
	vertexData[index++] = p4.y;
}

float* SpaceReclaimingIciclePlot::getVertexDataArray() {
	return this->vertexDataArray;
}

int SpaceReclaimingIciclePlot::getVertexDataArraySize() {
	return this->sizeVertexDataArray;
}