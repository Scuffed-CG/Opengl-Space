#include"Model.h"
#include"Framebuffer.h"
#include"Curve.h"
#include <filesystem>

const unsigned int width = 1920;
const unsigned int height = 1080;
float gamma = 2.2f;

std::vector<Vertex> vertices =
{
	Vertex{glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f)}
};



std::vector<GLuint> indices =
{
	0, 1, 2,
	0, 2, 3
};

std::vector<Vertex> lightVertices =
{ //     COORDINATES     //
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}
};

std::vector<GLuint> lightIndices =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


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
	Shader lightProgram("light.vert", "light.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);

	shaderProgram.Activate();
	shaderProgram.setVec3("dirLight.color", 1.0f, 1.0f, 1.0f);
	shaderProgram.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shaderProgram.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shaderProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shaderProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);	

	shaderProgram.setVec3("pointLights[0].position", 0.5f, 0.5f, 0.5f);
	shaderProgram.setVec3("pointLights[0].color", lightColor.r, lightColor.g, lightColor.b);
	shaderProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shaderProgram.setVec3("pointLights[0].diffuse", 0.4f, 0.4f, 0.4f);
	shaderProgram.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.5f);
	shaderProgram.setFloat("pointLights[0].constant", 1.0f);
	shaderProgram.setFloat("pointLights[0].linear", 0.09f);
	shaderProgram.setFloat("pointLights[0].quadratic", 0.032f);

	lightProgram.Activate();
	lightProgram.setVec4("lightColor", lightColor);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));


	std::string parentDir = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
	std::string modelDir = "/Models/";

	std::vector<Texture> textures =
	{
		Texture((parentDir + modelDir + "/textures/diffuse.png").c_str(), "diffuse", 0),
		Texture((parentDir + modelDir + "/textures/normal.png").c_str(), "normal", 1),
	};

	Mesh plane(vertices, indices, textures);
	Mesh lightCube(lightVertices, lightIndices, textures);

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
	glEnable(GL_MULTISAMPLE);
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

		glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
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

		glDisable(GL_CULL_FACE);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightCube.Draw(lightProgram, camera, lightModel);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);

		backpack.Draw(shaderProgram, camera);
		crow.Draw(shaderProgram, camera);

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