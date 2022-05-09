#include"Model.h"
#include"Framebuffer.h"
#include"Curve.h"
#include <filesystem>

const unsigned int width = 1920;
const unsigned int height = 1080;

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

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	std::string parentDir = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
	std::string modelDir = "/Models/";

	Model backpack((parentDir + modelDir + "backpack/backpack.obj"));
	Model crow((parentDir + modelDir + "crow/scene.gltf"));
	

	//-----------------------------------------------------------------Testing zone-------------------------------------------------------------------

	Curve curve1, curve2, curve3, curve4;

	curve1.addControlPoint(0.0f, 0.0f, 0.0f);
	curve1.addControlPoint(5.0f, 0.0f, 0.0f);
	curve1.addControlPoint(10.0f, 0.0f, 15.0f);
	curve1.addControlPoint(15.0f, 0.0f, 15.0f);

	curve2.addControlPoint(15.0f, 0.0f, 15.0f);
	curve2.addControlPoint(20.0f, 0.0f, 15.0f);
	curve2.addControlPoint(5.0f, 0.0f, 30.0f);
	curve2.addControlPoint(0.0f, 0.0f, 30.0f);

	curve3.addControlPoint(0.0f, 0.0f, 30.0f);
	curve3.addControlPoint(-5.0f, 0.0f, 30.0f);
	curve3.addControlPoint(-10.0f, 0.0f, 15.0f);
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

	//-----------------------------------------------------------------End of Testing zone------------------------------------------------------------

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	double prev_time = glfwGetTime();

	//Temp vars
	double prev_time_camera = glfwGetTime();
	double prev_time_backpack = glfwGetTime();
	glm::vec3 trans = glm::vec3(0.0f, 0.0f, 0.0f);
	bool moveCamera = false;
	bool moveBackpack = false;
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


		//-----------------------------------------------------------------Testing zone-------------------------------------------------------------------
		if (moveCamera)
			if (curr_time - prev_time_camera >= (double)(0.05f)) {
				camera.setPosition(curvePoints[currentPoint]);
				++currentPoint;
				if (!(currentPoint < curvePoints.size()))
					currentPoint = 0;
				prev_time_camera = glfwGetTime();
			}
		//-----------------------------------------------------------------End of Testing zone------------------------------------------------------------

		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//-----------------------------------------------------------------Testing zone-------------------------------------------------------------------
		if (moveBackpack) {
			if (curr_time - prev_time_backpack >= (double)(0.05f)) {
				trans = curvePoints[currentPoint];
				++currentPoint;
				if (!(currentPoint < curvePoints.size()))
					currentPoint = 0;
				prev_time_backpack = glfwGetTime();
			}
		}
		else {
			trans = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		//-----------------------------------------------------------------End of Testing zone------------------------------------------------------------

		glCullFace(GL_BACK);

		crow.Draw(shaderProgram, camera);
		backpack.Draw(shaderProgram, camera);
		
		
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