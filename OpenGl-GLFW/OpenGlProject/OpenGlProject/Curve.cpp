#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>

class BezierCurve {
public:
	void RegisterPoint(float x, float y) {
		points.push_back({ x,y });
	}

	void ClearPoints() {
		points.clear();
	}

	std::vector<glm::vec2> GetCurve() {
		std::vector<glm::vec2> curvePoints;

		float curveStart = points[0].x;

		float curveLength = 0.0f;

		for (int i = 0; i < points.size() - 1; i++)
		{
			curveLength += abs(sqrt(pow(points[i + 1].x - points[i].x, 2) + pow(points[i + 1].y - points[i].y, 2)));
		}

		for (float currentPoint = 0.0f; currentPoint < curveLength; ++currentPoint)
		{
			float t = currentPoint / curveLength;

			int n = points.size() - 1;
			glm::vec2 point(0.0f, 0.0f);

			for (int i = 0; i <= n; i++)
			{
				point += GLfloat(binomialCoefficient(n, i) * pow(1 - t, n - i) * pow(t, i)) * points[i];
			}

			curvePoints.push_back(point);
		}

		return curvePoints;
	}

	std::vector<glm::vec2> points;

private:

	int factorial(int x) {
		if (x > 1)
			return x * factorial(x - 1);
		else
			return 1;
	}

	int binomialCoefficient(int n, int k)
	{
		return factorial(n) / (factorial(k) * factorial(n - k));
	}
};