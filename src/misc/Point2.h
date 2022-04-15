struct Point2 {
	float x;
	float y;

	Point2(float x, float y) : x(x), y(y) {};

	Point2 add(Point2 p) {
		return Point2(x + p.x, y + p.y);
	}

	Point2 sub(Point2 p) {
		return Point2(x - p.x, y - p.y);
	}

	Point2 scale(float s) {
		return Point2(s * x, s * y);
	}

	void print() {
		std::cout << "(" << x << "," << y << ")";
	}
};