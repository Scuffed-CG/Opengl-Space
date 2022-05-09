#define _USE_MATH_DEFINES

#include"Model.h"
#include"Framebuffer.h"
#include"Curve.h"
#include <filesystem>
#include <cmath>

const unsigned int width = 1920;
const unsigned int height = 1080;

std::vector<glm::vec3> getCurvePoints();

int main()
{

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGlProject", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();
	glViewport(0, 0, width, height);


	Shader shaderProgram("default.vert", "default.frag", "default.geom");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 10.0f));


	std::string parentDir = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
	std::string modelDir = "/Models/";

	Model backpack((parentDir + modelDir + "backpack/backpack.obj"));
	Model crow((parentDir + modelDir + "crow/scene.gltf"));

	std::vector<glm::vec3> curvePoints = getCurvePoints();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	double prev_time = glfwGetTime();

	//Temp vars
	double prev_time_camera = glfwGetTime();
	double prev_time_backpack = glfwGetTime();
	glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 temp = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat RotationAngleRoll = 0; // Angle in radians
	GLfloat RotationAnglePitch = 0; // Angle in radians
	GLfloat RotationAngleYaw = 0; // Angle in radians
	glm::quat rot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat RotationX = glm::quat(cos(RotationAngleRoll / 2), 0.0f, 0.0f, sin(RotationAngleRoll / 2));
	glm::quat RotationY = glm::quat(cos(RotationAngleYaw / 2), 0.0f, sin(RotationAngleYaw / 2), 0.0f);
	glm::quat RotationZ = glm::quat(cos(RotationAnglePitch / 2), sin(RotationAnglePitch / 2), 0.0f, 0.0f);

	bool moveCamera = false;
	bool moveBackpack = true;
	int currentPoint = 0;
	//End Temp vars
	Framebuffer framebuffer(width, height, "framebuffer.vert", "framebuffer.frag");

	while (!glfwWindowShouldClose(window))
	{
		double curr_time = glfwGetTime();

		framebuffer.Bind();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		if (curr_time - prev_time >= (double)(1 / 60)) {
			camera.Inputs(window);
			prev_time = glfwGetTime();
		}

		camera.updateMatrix(90.0f, 0.1f, 100.0f);

		//-----------------------------------------------------------------Object Movement Zone-------------------------------------------------------------------
		if (camera.getMoveCamera()) //If camera is first person
			camera.setPosition(curvePoints[currentPoint] + glm::vec3(0.0f, 5.0f, 0.0f)); //Move camera

		if (camera.getMoveObject()) {
			if (curr_time - prev_time_backpack >= (double)(0.05f)) {

				if (!(currentPoint + 1 < curvePoints.size()))
					temp = curvePoints[currentPoint] - curvePoints[0];
				else
					temp = curvePoints[currentPoint] - curvePoints[currentPoint + 1];

				if (temp.z < 0) {
					RotationAnglePitch = -atan(temp.y / temp.z);
					RotationAngleYaw = atan(temp.x / temp.z);
				}
				else {
					RotationAngleYaw = M_PI + atan(temp.x / temp.z);
					RotationAnglePitch = atan(temp.y / temp.z);
				}

				RotationY = glm::quat(cos(RotationAngleYaw / 2), 0.0f, sin(RotationAngleYaw / 2), 0.0f);
				RotationZ = glm::quat(cos(RotationAnglePitch / 2), sin(RotationAnglePitch / 2), 0.0f, 0.0f);
				rot = RotationX * RotationY * RotationZ;
				trans = curvePoints[currentPoint];
				++currentPoint;
				if (!(currentPoint < curvePoints.size()))
					currentPoint = 0;
				prev_time_backpack = glfwGetTime();
			}
		}

		//-----------------------------------------------------------------End of Zone------------------------------------------------------------

		glCullFace(GL_BACK);
		if(!camera.getMoveCamera())// If first person don't draw the moving object
			backpack.Draw(shaderProgram, camera, trans, rot);
		crow.Draw(shaderProgram, camera, { 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f, 0.0f}, { 0.5f, 0.5f, 0.5f });


		glCullFace(GL_FRONT);

		GLint mode;
		glGetIntegerv(GL_POLYGON_MODE, &mode);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		framebuffer.Draw();

		glPolygonMode(GL_FRONT_AND_BACK, mode);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

std::vector<glm::vec3> getCurvePoints() {
	Curve curve1, curve2, curve3, curve4;

	curve1.addControlPoint(0.0f, 0.0f, 0.0f);
	curve1.addControlPoint(5.0f, -10.0f, 0.0f);
	curve1.addControlPoint(10.0f, -5.0f, 15.0f);
	curve1.addControlPoint(15.0f, 0.0f, 15.0f);

	curve2.addControlPoint(15.0f, 0.0f, 15.0f);
	curve2.addControlPoint(20.0f, 0.0f, 15.0f);
	curve2.addControlPoint(5.0f, 0.0f, 30.0f);
	curve2.addControlPoint(0.0f, 0.0f, 30.0f);

	curve3.addControlPoint(0.0f, 0.0f, 30.0f);
	curve3.addControlPoint(-5.0f, 10.0f, 30.0f);
	curve3.addControlPoint(-10.0f, 5.0f, 15.0f);
	curve3.addControlPoint(-15.0f, 0.0f, 15.0f);

	curve4.addControlPoint(-15.0f, 0.0f, 15.0f);
	curve4.addControlPoint(-20.0f, 0.0f, 15.0f);
	curve4.addControlPoint(-5.0f, 0.0f, 0.0f);
	curve4.addControlPoint(0.0f, 0.0f, 0.0f);


	// Vul 1 vector met alle punten
	std::vector<glm::vec3> curvePoints = curve1.getCurve();
	std::vector<glm::vec3> tempPoints;
	tempPoints = curve2.getCurve();
	for (int i = 1; i < tempPoints.size(); ++i) {
		curvePoints.push_back(tempPoints[i]);
	}
	tempPoints = curve3.getCurve();
	for (int i = 1; i < tempPoints.size(); ++i) {
		curvePoints.push_back(tempPoints[i]);
	}
	tempPoints = curve4.getCurve();
	for (int i = 1; i < tempPoints.size() - 1; ++i) {
		curvePoints.push_back(tempPoints[i]);
	}

	return curvePoints;
}