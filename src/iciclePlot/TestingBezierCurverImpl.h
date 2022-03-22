#pragma once

#include <iostream>

class TestingBezierCurverImpl {

public:
	float* elements;
	int size; //number of elements

	//points per line is the number of points on the upper and lower side of the square
	// i > 1
	void generateElements(int nrPointsLine, float initX, float initY, float maxWidth, float height) {
		size = 12 * (nrPointsLine - 1);
		size += 12 * ((nrPointsLine / 2) - 1);
		elements = new float[size];

		float x = 0.0;

		//float width = maxWidth / (float)nrPointsLine;
		
		float width = (float)maxWidth / (float)nrPointsLine;

		int index = 0;

		for (int i = 0; i < nrPointsLine - 1; i++) {

			elements[index++] = x;
			elements[index++] = 0;

			elements[index++] = x;
			elements[index++] = height;

			elements[index++] = x + width;
			elements[index++] = 0;

			elements[index++] = x;
			elements[index++] = height;

			elements[index++] = x + width;
			elements[index++] = 0;

			elements[index++] = x + width;
			elements[index++] = height;

			x += width;
		}

		x = maxWidth / 4.0;

		for (int i = 0; i < (nrPointsLine / 2) - 1; i++) {

			elements[index++] = x;
			elements[index++] = height;

			elements[index++] = x;
			elements[index++] = height * 2;

			elements[index++] = x + width;
			elements[index++] = height;

			elements[index++] = x;
			elements[index++] = height * 2;

			elements[index++] = x + width;
			elements[index++] = height;

			elements[index++] = x + width;
			elements[index++] = height * 2;

			x += width;
		}


		std::cout << "VERTEX generated:: " << index << std::endl;
	}
};