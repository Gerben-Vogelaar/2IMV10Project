#include "SpaceReclaimingIciclePlot.h"

SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(Newick& newickTree, SRIP1_arg arg, bool expirimental) {
	if (expirimental) {
		int multVector = 50;

		this->sizeVertexDataArray = 8 * newickTree.getTreeSize() * (multVector);
		this->vertexDataArray = new float[sizeVertexDataArray];

		cout << "nr vertex: " << sizeVertexDataArray << endl;

		int index = 0;

		//SRIP1Expirimental_initQ(arg, newickTree, vertexDataArray, index, multVector);
		SRIP1Expirimental_initQ_h(arg, newickTree, vertexDataArray, index, multVector);
	}
	else {
		int multVector = 50;

		this->sizeVertexDataArray = 8 * newickTree.getTreeSize() * (multVector - 1);
		this->vertexDataArray = new float[sizeVertexDataArray];

		cout << "nr vertex: " << sizeVertexDataArray << endl;

		int index = 0;

		//SRIP1Expirimental_initQ(arg, newickTree, vertexDataArray, index, multVector);
		SRIP1Expirimental_initQ(arg, newickTree, vertexDataArray, index, multVector);
	}
}

/* multVector specifies the additional amount of meshes per quadrangle*/
SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(Newick& newickTree, SRIP1_arg arg, bool expirimental, int multVector) {
	this->sizeVertexDataArray = 12 * newickTree.getTreeSize() * (multVector - 1);
	this->vertexDataArray = new float[sizeVertexDataArray];

	int index = 0;

	if (expirimental) {
		SRIP1_s_expirimental_init(arg, newickTree, vertexDataArray, index, multVector);
	}
	else {
		SRIP1_s_init(arg, newickTree, vertexDataArray, index, multVector);
	}
}

SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg, bool expirimental) {
	SpaceReclaimingIciclePlot(newickTree, arg, expirimental, 1);
}

SpaceReclaimingIciclePlot::SpaceReclaimingIciclePlot(Newick& newickTree, SRIP2_arg arg, bool expirimental, int multVector) {
	this->sizeVertexDataArray = 12 * newickTree.getTreeSize() * (multVector - 1);
	this->vertexDataArray = new float[sizeVertexDataArray];

	int index = 0;

	if (expirimental) {
		//SRIP2_experimental_init(arg, newickTree, vertexDataArray, index);
	}
	else {
		SRIP2_init(arg, newickTree, vertexDataArray, index, multVector);
	}
}

//shouldnt be a reference! changes made in a copy suffices
void SpaceReclaimingIciclePlot::SRIP1_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index)
{
	drawQuadrangleByTriangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h));

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1_r(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index);
	}

};

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
			drawQuadrangleByTriangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y));
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

void SpaceReclaimingIciclePlot::SRIP1_s_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector)
{
	drawQuadrangleByTriangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h), multVector);

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1_s_r(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index, 0,  multVector);
	}
}

void SpaceReclaimingIciclePlot::SRIP1_s_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector)
{
	float U = w - (m - 1) * arg.gamma; //total width - number of white spaces size gamma (= (m - 1) * gamma)
	float x = ((arg.W - w) / 2.0f); // -pOffset;
	float y = (d + 1.0f) * arg.h;

	float finalOffset = 0.0f;
	float mpp = 0.0f;

	float xp = (arg.W - w) / 2.0f;

	//Calculate offset on each level 
	for (TreeNode p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;

			finalOffset += (p1.x - xp) + (p0.x - (xp + delta));

			xp += delta + arg.gamma;
			p0 = p1;
			int n = c.descendant_list.size();
			if (n > 0) {
				mpp += n;
			}
		}
	}

	finalOffset = finalOffset / (2 * mpp);

	finalOffset += pOffset;

	std::cout << "final, p: " << finalOffset << " " << pOffset << endl;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode& c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			
			drawQuadrangleByTriangle(vertexData, index, p0.add(Point2(pOffset, 0.0f)), p1.add(Point2(pOffset, 0.0f)), Point2(x + delta, y).add(Point2(finalOffset, 0.0f)), Point2(x, y).add(Point2(finalOffset, 0.0f)), multVector);
			
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
		SRIP1_s_r(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index, finalOffset, multVector);
	}
}

void SpaceReclaimingIciclePlot::SRIP1Expirimental_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index)
{
	drawQuadrangleByTriangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h));

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1Expirimental_r(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index, 0);
	}
}

void SpaceReclaimingIciclePlot::SRIP1Expirimental_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset)
{
	float U = w - (m - 1) * arg.gamma; //total width - number of white spaces size gamma (= (m - 1) * gamma)
	float x = ((arg.W - w) / 2.0f); // -pOffset;
	float y = (d + 1.0f) * arg.h;

	float finalOffset = 0.0f;
	float mpp = 0.0f;

	float xp = (arg.W - w) / 2.0f;

	//Calculate offset on each level 
	for (TreeNode p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;

			finalOffset += (p1.x - xp) + (p0.x - (xp + delta));

			xp += delta + arg.gamma;
			p0 = p1;
			int n = c.descendant_list.size();
			if (n > 0) {
				mpp += n;
			}
		}
	}

	finalOffset = finalOffset / (2 * mpp);

	finalOffset += pOffset;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode& c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			//drawQuadrangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y));
			
			//drawQuadrangle(vertexData, index, p0.add(Point2(finalOffset, 0.0f)) , p1.add(Point2(finalOffset, 0.0f)), Point2(x + delta, y).add(Point2(pOffset, 0.0f)), Point2(x, y).add(Point2(pOffset, 0.0f)));
			
			drawQuadrangleByTriangle(vertexData, index, p0.add(Point2(pOffset, 0.0f)) , p1.add(Point2(pOffset, 0.0f)), Point2(x + delta, y).add(Point2(finalOffset, 0.0f)), Point2(x, y).add(Point2(finalOffset, 0.0f)));
			//xOffset += p0.x - x;
			//xOffset2 += p1.x - (x + delta);
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
		SRIP1Expirimental_r(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index, finalOffset);
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
void SpaceReclaimingIciclePlot::SRIP2_init(SRIP2_arg arg, Newick& tree, float* vertexData, int& index, int multVector) {
	//compute weights
	//... TODO

	//offset
	float o = (arg.W - arg.epsilon) / 2;

	drawQuadrangleByTriangle(vertexData, index, Point2(o, 0), Point2(arg.W - o, 0), Point2(arg.W - o, arg.h), Point2(o, arg.h), multVector);

	TreeNode r = tree.getSourceNode();

	r.setPosition(Point3(o, arg.h, arg.epsilon));

	if (r.descendant_list.size() > 0) {
		SRIP2_r(1, vector<TreeNode>{r}, r.descendant_list.size(), Weight(r.descendant_list), arg.W, 0, arg, vertexData, index, multVector);
	}
}

void SpaceReclaimingIciclePlot::SRIP2_r(int d, vector<TreeNode> P, int m, float A, float w, float g, SRIP2_arg arg, float* vertexData, int& index, int multVector) {
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
				drawQuadrangleByTriangle(vertexData, index, p0, p1, Point2(x + o + arg.sigma, y), Point2(x + o, y), multVector);
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
		SRIP2_r(d + 1, Pp, mp, Ap, wp + arg.rho * (arg.W - wp), arg.sigma * gp, arg, vertexData, index, multVector);
	}
}

void SpaceReclaimingIciclePlot::drawQuadrangleByTriangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4) {
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

void SpaceReclaimingIciclePlot::drawQuadrangleByTriangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector) {
	float topWidth = (p3.x - p4.x) / (multVector - 1);
	float botWidth = (p2.x - p1.x) / (multVector - 1);

	float height = p4.y - p1.y;

	float topx = p4.x;
	float botx = p1.x;
	float y = p1.y;

	for (int i = 0; i < multVector - 1; i++) {
		vertexData[index++] = botx;
		vertexData[index++] = y;

		vertexData[index++] = topx;
		vertexData[index++] = y + height;

		vertexData[index++] = botx + botWidth;
		vertexData[index++] = y;

		vertexData[index++] = topx;
		vertexData[index++] = y + height;

		vertexData[index++] = botx + botWidth;
		vertexData[index++] = y;

		vertexData[index++] = topx + topWidth;
		vertexData[index++] = y + height;

		botx += botWidth;
		topx += topWidth;
	}
}

void SpaceReclaimingIciclePlot::drawQuadrangleByQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4) {
	vertexData[index++] = p1.x;
	vertexData[index++] = p1.y;

	vertexData[index++] = p2.x;
	vertexData[index++] = p2.y;

	vertexData[index++] = p3.x;
	vertexData[index++] = p3.y;

	vertexData[index++] = p4.x;
	vertexData[index++] = p4.y;
}

void SpaceReclaimingIciclePlot::drawQuadrangleByQuadrangle(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector) {

	float topWidth = (p3.x - p4.x) / (multVector - 1);
	float botWidth = (p2.x - p1.x) / (multVector - 1);

	float height = p4.y - p1.y;

	float topx = p4.x;
	float botx = p1.x;
	float y = p1.y;

	for (int i = 0; i < multVector - 1; i++) {
		vertexData[index++] = botx;
		vertexData[index++] = y;

		vertexData[index++] = topx;
		vertexData[index++] = y + height;

		vertexData[index++] = topx + topWidth;
		vertexData[index++] = y + height;

		vertexData[index++] = botx + botWidth;
		vertexData[index++] = y;

		botx += botWidth;
		topx += topWidth;
	}
}

void swap(Point2* a, Point2* b)
{
	Point2 temp = *a;
	*a = *b;
	*b = temp;
}

void SpaceReclaimingIciclePlot::drawQuadrangleByQuadrangleHorizontal(float* vertexData, int& index, Point2 p1, Point2 p2, Point2 p3, Point2 p4, int multVector) {
	float topWidth = (p3.x - p4.x) / (multVector - 1);
	float botWidth = (p2.x - p1.x) / (multVector - 1);

	const float COMP_ERROR = 0.01f;

	//1. determine order of points (we get 'new' p1,p2,p3,p4)

	if (p1.x > p2.x) swap(&p1, &p2);
	if (p3.x > p4.x) swap(&p3, &p4);
	if (p1.x > p3.x) swap(&p1, &p3);
	if (p2.x > p4.x) swap(&p2, &p4);
	if (p2.x > p3.x) swap(&p2, &p3);

	//2. det width each fig (triangle, quad, triangle)]

	float p1p2_width = p2.x - p1.x;
	float p2p3_width = p3.x - p2.x;
	float p3p4_width = p4.x - p3.x;
	float width_total = p4.x - p1.x;

	int stepsT1 = multVector * (p1p2_width / width_total);
	int stepsT3 = multVector * (p3p4_width / width_total);
	int stepsT2 = multVector - stepsT1 - stepsT3;
		
	//3. draw left-triangle p1, 

	float slope1 = 0.0f;
	float slope2 = 0.0f;

	//cout << "POINT: (" << p1.x << "," << p1.y << ")" << endl;

	//if (p1.x == p2.x) { // -> pick slope p1, p3
	if (std::abs(p1.y - p2.y) < 0.001) { // -> pick slope p1, p3
		slope1 = p1.sub(p3).y / p1.sub(p3).x; // check if scaling correct.
	}
	else { //p1.x == p3.x
		slope1 = p1.sub(p2).y / p1.sub(p2).x; // check if scaling correct.
	}

	float incrX = p1p2_width / stepsT1;
	
	//instantiate the values such that iff we have 0 steps, we have non null values.
	float x = p1.x;
	float y = p1.y;
	float x2 = p1.x;
	float y2 = p1.y;
	float yflat = p2.y; //prev p2.y

	for (int i = 0; i < stepsT1; i++) {
		x = p1.x + incrX * i;
		y = p1.y + incrX * i * slope1;
		x2 = p1.x + incrX * (i + 1);
		y2 = p1.y + incrX * (i + 1) * slope1;
		yflat = p1.y;

		vertexData[index++] = x;
		vertexData[index++] = y;
		vertexData[index++] = x2;
		vertexData[index++] = y2;
		vertexData[index++] = x2;
		vertexData[index++] = yflat;
		vertexData[index++] = x;
		vertexData[index++] = yflat;
	}

	//to connect the rectangle -> start at (x, yflat) -> p1' and (x2,y2) -> p2'
	Point2 p_1 = Point2(x2, yflat); //horizontal line
	Point2 p_2 = Point2(x2, y2);	//sloped line

	//3b. draw right-triangle

	if (std::abs(p3.y - p4.y) < COMP_ERROR) { // -> pick slope p4, p2
		slope2 = p2.sub(p4).y / p2.sub(p4).x; // check if scaling correct.
	}
	else { //p2.x == p4.x
		slope2 = p3.sub(p4).y / p3.sub(p4).x; // check if scaling correct.
	}

	incrX = p3p4_width / stepsT3;

	//instantiate the values such that iff we have 0 steps, we have non null values.
	x = p4.x;
	y = p4.y;
	x2 = p4.x;
	y2 = p4.y;
	yflat = p3.y;

	for (int i = 0; i < stepsT3; i++) {

		x = p4.x - incrX * i;
		y = p4.y - incrX * i * slope2;
		x2 = p4.x - incrX * (i + 1);
		y2 = p4.y - incrX * (i + 1) * slope2;
		yflat = p4.y;

		vertexData[index++] = x;
		vertexData[index++] = y;
		vertexData[index++] = x2;
		vertexData[index++] = y2;
		vertexData[index++] = x2;
		vertexData[index++] = yflat;
		vertexData[index++] = x;
		vertexData[index++] = yflat;
	}

	Point2 p_3 = Point2(x2, yflat); //horizontal line
	Point2 p_4 = Point2(x2, y2);	//sloped line

	//swap p_1 and p_2 , p_3 and p_4 based on y value.

	////4. draw quad p2, p3
	float x_1 = p_2.x;
	float y_1 = p_2.y;
	float x2_1 = p_2.x;
	float y2_1 = p_2.y;
	
	float x_2 = p_1.x;
	float y_2 = p_1.y;
	float x2_2 = p_1.x;
	float y2_2 = p_1.y;

	//Sort such that p_1.y == p_4.y
	if (p_1.y < p_2.y) {
		swap(p_1, p_2);
	}

	if (p_3.y > p_4.y) {
		swap(p_3, p_4);
	}

	slope1 = p_4.sub(p_1).y / p_4.sub(p_1).x;
	slope2 = p_3.sub(p_2).y / p_3.sub(p_2).x;

	incrX = p2p3_width / stepsT2;
	
	for (int i = 0; i < stepsT2; i++) {

		x_1 = p_1.x + incrX * i;
		y_1 = p_1.y + incrX * i * slope1;
		x2_1 = p_1.x + incrX * (i + 1);
		y2_1 = p_1.y + incrX * (i + 1) * slope1;

		x_2 = p_2.x + incrX * i;
		y_2 = p_2.y + incrX * i * slope2;
		x2_2 = p_2.x + incrX * (i + 1);
		y2_2 = p_2.y + incrX * (i + 1) * slope2;

		vertexData[index++] = x_1;
		vertexData[index++] = y_1;
		vertexData[index++] = x2_1;
		vertexData[index++] = y2_1;
		vertexData[index++] = x2_2;
		vertexData[index++] = y2_2;
		vertexData[index++] = x_2;
		vertexData[index++] = y_2;
	}
}

// Weight equals the number of subnodes
float SpaceReclaimingIciclePlot::Weight(const TreeNode c) {
	int totalWeight = 1;

	for (TreeNode n : c.descendant_list) {
		totalWeight += Weight(n);
	}

	return totalWeight;
}

void SpaceReclaimingIciclePlot::SRIP1_s_expirimental_init(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector)
{ 
	cout << "nonononononono!!!!!!!!!!" << endl;
}

float SpaceReclaimingIciclePlot::Weight(const vector<TreeNode> C) {
	int weight = 1;
	for (TreeNode n : C) {
		weight += Weight(n);
	}
	return weight;
}

void SpaceReclaimingIciclePlot::SRIP1_s_expirimental_r(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, int multVector)
{
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
			drawQuadrangleByTriangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y), multVector);
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
		SRIP1_s_expirimental_r(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index, multVector);
	}
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

// BELOW FOR TESTING PURPOSES!

void SpaceReclaimingIciclePlot::SRIP1Expirimental_initQ(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector) {
	drawQuadrangleByQuadrangle(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h), multVector);

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1Expirimental_rQ(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index, 0, multVector);
	}
}
void SpaceReclaimingIciclePlot::SRIP1Expirimental_rQ(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector) {
	float U = w - (m - 1) * arg.gamma; //total width - number of white spaces size gamma (= (m - 1) * gamma)
	float x = (arg.W - w) / 2.0f;
	float y = (d + 1.0f) * arg.h;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode& c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			drawQuadrangleByQuadrangle(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y), multVector);
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
		SRIP1Expirimental_rQ(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index, 1, multVector);
	}
}

void SpaceReclaimingIciclePlot::SRIP1Expirimental_initQ_h(SRIP1_arg arg, Newick& tree, float* vertexData, int& index, int multVector)
{
	drawQuadrangleByQuadrangleHorizontal(vertexDataArray, index, Point2(0.0f, 0.0f), Point2(arg.W, 0.0f), Point2(arg.W, arg.h), Point2(0.0f, arg.h), multVector);

	TreeNode source = tree.getSourceNode();

	source.position = Point3(0.0f, arg.h, arg.W);

	if (source.descendant_list.size() > 0) {
		SRIP1Expirimental_rQ_h(1, vector<TreeNode>{source}, source.descendant_list.size(), arg.W, arg, vertexDataArray, index, 0, multVector);
	}

}

void SpaceReclaimingIciclePlot::SRIP1Expirimental_rQ_h(int d, vector<TreeNode> P, int m, float w, SRIP1_arg arg, float* vertexData, int& index, float pOffset, int multVector)
{
	float U = w - (m - 1) * arg.gamma; //total width - number of white spaces size gamma (= (m - 1) * gamma)
	float x = (arg.W - w) / 2.0f;
	float y = (d + 1.0f) * arg.h;

	vector<TreeNode> Pp;
	float wp = 0.0f;
	float mp = 0.0f;

	for (TreeNode& p : P) {
		Point2 p0 = Point2(p.position.x, p.position.y);

		for (TreeNode& c : p.descendant_list) {
			Point2 p1 = p0.add(Point2(p.position.z, 0.0f).scale(1.0f / (p.descendant_list.size())));
			float delta = U / m;
			drawQuadrangleByQuadrangleHorizontal(vertexData, index, p0, p1, Point2(x + delta, y), Point2(x, y), multVector);
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
		SRIP1Expirimental_rQ_h(d + 1, Pp, mp, wp + arg.rho * (arg.W - wp), arg, vertexData, index, 1, multVector);
	}
}
