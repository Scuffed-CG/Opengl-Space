#include "Path.h"

Path::Path(std::vector<Curve> curves) {
	Path::path = curves;
	calcPoints();
}

void Path::Draw(Shader& shader, Camera& camera) {
	shader.Activate();
	camera.Matrix(shader, "camMatrix");

	glLineWidth(3.0f);
	std::vector<unsigned int> indecies;

	for (int i = 0; i < pathPoints.size(); i++)
	{
		if (i != pathPoints.size() - 1) {
			indecies.push_back(i);
			if (i < pathPoints.size())
				indecies.push_back(i + 1);
		}
	}
	GLuint lineVAO, lineVBO, lineEBO;

	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glGenBuffers(1, &lineEBO);

	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pathPoints.size(), pathPoints.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indecies.size(), indecies.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glDrawElements(GL_LINES, indecies.size(), GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &lineEBO);
	glDeleteBuffers(1, &lineVBO);
	glDeleteVertexArrays(1, &lineVAO);
}

void Path::calcPoints() {
	pathPoints.clear();
	std::vector<glm::vec3> tmpPoints;

	for (size_t i = 0; i < path.size(); i++)
	{
		tmpPoints = path[i].getCurve();
		pathPoints.insert(pathPoints.end(), tmpPoints.begin(), tmpPoints.end());
	}
	pathPoints.push_back(pathPoints[0]);
}
