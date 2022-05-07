#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>

class Curve {
public:

	void addControlPoint(float x, float y, float z);
	void clearControlPoints();

	float getCurveLength();
	std::vector<glm::vec3> getCurve();

private:

	std::vector<glm::vec3> ctrl_points;

	int fact(int x);
	int binCo(int n, int k);
};


#endif