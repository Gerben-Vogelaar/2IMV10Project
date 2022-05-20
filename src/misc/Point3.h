/* to be refactured -> use glm::vec3 instead*/


struct Point3 {
	float x;
	float y;
	float z;

	Point3() : x(0), y(0), z(0) {};

	Point3(float x, float y, float z) : x(x), y(y), z(z) {};

	Point3 add(Point3 p) {
		return Point3(x + p.x, y + p.y, z + p.z);
	}

	Point3 scale(float s) {
		return Point3(s * x, s * y, s * z);
	}
};