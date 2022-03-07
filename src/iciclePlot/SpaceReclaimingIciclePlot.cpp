#include "SpaceReclaimingIciclePlot.h"


SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(Newick& newickTree, SRIP1_arg arg) {
	this->sizeVertexDataArray = 12 * newickTree.getTreeSize();
	this->vertexDataArray = new float[sizeVertexDataArray];

	int index = 0;

	//SRIP1_init(arg, newickTree, vertexDataArray, index);
	SRIP1Expirimental_init(arg, newickTree, vertexDataArray, index);

	displaceQuadranglesX(-1.0f, vertexDataArray, sizeVertexDataArray);

	displaceQuadranglesY(-1.0f, vertexDataArray, sizeVertexDataArray);
}

SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg) {
	this->sizeVertexDataArray = 12 * newickTree.getTreeSize();
	this->vertexDataArray = new float[sizeVertexDataArray];

	int index = 0;

	SRIP2_init(arg, newickTree, vertexDataArray, index);

	displaceQuadranglesX(-1.0f, vertexDataArray, sizeVertexDataArray);

	displaceQuadranglesY(-1.0f, vertexDataArray, sizeVertexDataArray);
}

//shouldnt be a reference! changes made in a copy suffices
void SpaceReclaimingIciclePlot::SRIP1_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index)
{
	//Point3 p = Point3(0.0f, 0.0f, 0.0f);

	drawQuadrangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h));

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1_r(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index);
	}

};

//P should be reference???
void SpaceReclaimingIciclePlot::SRIP1_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index) {

	float U = w - (m - 1) * arg.gamma; //total width - number of white spaces size gamma (= (m - 1) * gamma)
	float x = (arg.W - w) / 2.0f; //
	float y = (d + 1.0f) * arg.h;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode& c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			drawQuadrangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y));
			c.setPosition(Point3(x, y, delta));
			x += delta + arg.gamma;
			p0 = p1;
			int n = c.descendant_list.size();
			if (n > 0) {
				Pp.push_back(c);
				wp += delta;
				mp += n;
			}
		}
	}

	if (mp > 0) {
		SRIP1_r(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index);
	}
}

void SpaceReclaimingIciclePlot::SRIP1Expirimental_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index)
{
	//Point3 p = Point3(0.0f, 0.0f, 0.0f);

	drawQuadrangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h));

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1Expirimental_r(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index);
	}
}

void SpaceReclaimingIciclePlot::SRIP1Expirimental_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index)
{
	float U = w - (m - 1) * arg.gamma; //total width - number of white spaces size gamma (= (m - 1) * gamma)
	float x = (arg.W - w) / 2.0f; //
	float y = (d + 1.0f) * arg.h;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	int indexFirst = index;

	float xOffset = 0.0f;
	float xOffset2 = 0.0f;

	float wpp = 0.0f;
	float mpp = 0.0f;

	float xp = (arg.W - w) / 2.0f; //
	float yp = (d + 1.0f) * arg.h;

	for (TreeNode p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			//drawQuadrangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y));
			xOffset += p0.x - xp;
			xOffset2 += p1.x - (xp + delta);
			c.setPosition(Point3(xp, yp, delta));
			xp += delta + arg.gamma;
			p0 = p1;
			int n = c.descendant_list.size();
			if (n > 0) {
				wpp += delta;
				mpp += n;
			}
		}
	}

	float finalOffset = (xOffset2 - xOffset) / mpp;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode& c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			drawQuadrangle(vertexData, index, p0, p1, Point2(x + delta + finalOffset, y), Point2(x + finalOffset, y));
			//xOffset += p0.x - x;
			//xOffset2 += p1.x - (x + delta);
			c.setPosition(Point3(x + finalOffset, y, delta));
			x += delta + arg.gamma;
			p0 = p1;
			int n = c.descendant_list.size();
			if (n > 0) {
				Pp.push_back(c);
				wp += delta;
				mp += n;
			}
		}
	}


	

	cout << "offsetLeft, offsetRight, difference: " << xOffset << " " << xOffset2 << " " << xOffset + xOffset2 << endl;

	if (mp > 0) {
		SRIP1Expirimental_r(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index);
	}
}

/*
* W: width diagram
* h: height of a layer
* epsilon [0,W]: max width of a node
* gamma: max horizontal gap between nodes
* rho [0,1]: lost space shrinking parameter
* sigma [0,1]: sticky node shrinking factor
* lambda: depth span of a sticky node
*/
void SpaceReclaimingIciclePlot::SRIP2_init(SRIP2_arg arg, Newick& tree, float* vertexData, int& index) {
	//compute weights
	//... TODO

	//offset
	float o = (arg.W - arg.epsilon) / 2;

	drawQuadrangle(vertexData, index, Point2(o, 0), Point2(arg.W - o, 0), Point2(arg.W - o, arg.h), Point2(o, arg.h));

	TreeNode r = tree.getSourceNode();

	r.setPosition(Point3(o, arg.h, arg.epsilon));

	if (r.descendant_list.size() > 0) {
		SRIP2_r(1, vector<TreeNode>{r}, r.descendant_list.size(), Weight(r.descendant_list), arg.W, 0, arg, vertexData, index);
	}
}

void SpaceReclaimingIciclePlot::SRIP2_r(int d, vector<TreeNode> P, int m, float A, float w, float g, SRIP2_arg arg, float* vertexData, int& index) {
	float gammap = 0;

	if (m > 1) {
		float q = min(arg.gamma, floor((w - g - m) / (m - 1)));
		gammap = 0 > q ? 0 : q;
	}

	//might go wrong due to rounding errors~!!!!!
	if (gammap == 0 && g > w) {
		w = g;
	}

	float U = w - g - (m - 1) * gammap;
	float x = (arg.W - w) / 2;
	float y = (d + 1) * arg.h;

	vector<TreeNode> Pp;
	float mp = 0.0f;
	float Ap = 0.0f;
	float wp = 0.0f;
	float gp = 0.0f;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		vector<TreeNode> C;

		if (p.sticky) {
			C.push_back(p);
		}
		else {
			for (TreeNode& pd : p.descendant_list) {
				C.push_back(pd);
			}
		}

		float delta = 0;
		float o = 0;

		Point2 p1 = Point2(0, 0);

		for (TreeNode& c : C) {
			if (c.sticky) {
				p1 = p0.add(Point2(p.position.z, 0));
				c.setPosition(Point3(x, y, arg.sigma * p.position.z));
				x = x + c.position.z;
				c.span += 1;
			}
			else {
				p1 = p0.add(Point2(p.position.z, 0).scale(Weight(c) / Weight(C)));
				delta = Weight(c) / A * U;
				arg.sigma = min(delta, arg.epsilon);
				o = (delta - arg.sigma) / 2;
				drawQuadrangle(vertexData, index, p0, p1, Point2(x + o + arg.sigma, y), Point2(x + o, y));
				c.setPosition(Point3(x + o, y, arg.sigma));
				x = x + delta + gammap;
			}
			p0 = p1;
			vector<TreeNode> Cp = c.descendant_list; //wrong because not references???
			int n = c.descendant_list.size();
			c.sticky = n == 0;
			if (c.sticky && c.span < arg.lambda) {
				Pp.push_back(c);
				gp += c.position.z;
			}
			if (!c.sticky) {
				Pp.push_back(c);
				mp += n;
				Ap += Weight(Cp);
				wp += delta;
			}
		}
	}
	if (mp > 0) {
		SRIP2_r(d + 1, Pp, mp, Ap, wp + arg.rho * (arg.W - wp), arg.sigma * gp, arg, vertexData, index);
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

// Weight equals the number of subnodes
float SpaceReclaimingIciclePlot::Weight(const TreeNode c) {
	int totalWeight = 1;

	for (TreeNode n : c.descendant_list) {
		totalWeight += Weight(n);
	}

	return totalWeight;
}

float SpaceReclaimingIciclePlot::Weight(const vector<TreeNode> C) {
	int weight = 1;
	for (TreeNode n : C) {
		weight += Weight(n);
	}
	return weight;
}

void SpaceReclaimingIciclePlot::displaceQuadranglesX(const float x, float* vertexData, int sizeVertexData) {
	for (int i = 0; i < sizeVertexData; i = i + 2) {
		vertexData[i] = vertexData[i] + x;
		//cout << " " << vertexData[i] << " ";
	}
}

void SpaceReclaimingIciclePlot::displaceQuadranglesY(const float y, float* vertexData, int sizeVertexData) {
	for (int i = 1; i < sizeVertexData; i = i + 2) {
		vertexData[i] = vertexData[i] + y;
		//cout << " " << vertexData[i] << " ";
	}
}

float* SpaceReclaimingIciclePlot::getVertexDataArray() {
	return this->vertexDataArray;
}

int SpaceReclaimingIciclePlot::getVertexDataArraySize() {
	return this->sizeVertexDataArray;
}