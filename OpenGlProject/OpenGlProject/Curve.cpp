#include "Curve.h"

void Curve::addControlPoint(float x, float y, float z) {
	ctrl_points.push_back({ x,y,z });
}

void Curve::clearControlPoints() {
	ctrl_points.clear();
}

std::vector<glm::vec3> Curve::getCurve() {
	std::vector<glm::vec3> curvePoints;

	float curveLength = getCurveLength();

	for (float currentPoint = 0.0f; currentPoint < curveLength; ++currentPoint)
	{
		float t = currentPoint / curveLength;
		int n = ctrl_points.size() - 1;
		glm::vec3 point(0.0f, 0.0f, 0.0f);

		for (int i = 0; i <= n; i++)
		{
			point += GLfloat(binCo(n, i) * pow(1 - t, n - i) * pow(t, i)) * ctrl_points[i];
		}

		curvePoints.push_back(point);
	}

	return curvePoints;
}

float Curve::getCurveLength() {

	float curveLength = 0.0f;

	for (int i = 0; i < ctrl_points.size() - 1; i++)
	{
		curveLength += abs(sqrt(pow(ctrl_points[i + 1].x - ctrl_points[i].x, 2) + pow(ctrl_points[i + 1].y - ctrl_points[i].y, 2) + pow(ctrl_points[i + 1].z - ctrl_points[i].z, 2)));
	}

	return curveLength;
}

int Curve::fact(int x) {
	if (x > 1)
		return x * fact(x - 1);
	else
		return 1;
}

int Curve::binCo(int n, int k)
{
	return fact(n) / (fact(k) * fact(n - k));
}