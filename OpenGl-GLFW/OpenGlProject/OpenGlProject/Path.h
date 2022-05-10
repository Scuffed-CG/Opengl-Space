#ifndef PATH_CLASS_H
#define PATH_CLASS_H

#include "curve.h"
#include "shaderClass.h"
#include "Camera.h"

class Path
{
public:
	Path(std::vector<Curve> curves);
	void Draw(Shader& shader, Camera& camera);


	std::vector<Curve> path;
	std::vector<glm::vec3> pathPoints;
private:
	void calcPoints();
};

#endif