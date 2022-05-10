#define _USE_MATH_DEFINES

#include"Model.h"
#include"Framebuffer.h"
#include"Path.h"
#include <filesystem>
#include <cmath>

#include <irrKlang/irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

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


std::vector<glm::vec3> getCurvePoints(std::vector<Curve> path);

int main()
{
	ISoundEngine* engine = createIrrKlangDevice();
	if (!engine)
		return 0;
	engine->setSoundVolume(0.25f);

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

//--------------------------------------initializing shaders----------------------------------
	Shader shaderProgram("default.vert", "default.frag", "default.geom");
	Shader lightProgram("light.vert", "light.frag");
	Shader lineProgram("line.vert", "line.frag");

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

	lineProgram.Activate();
	lineProgram.setVec4("color", lightColor);

//--------------------------------------initializing camera----------------------------------
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

//--------------------------------------initializing models----------------------------------
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

	std::vector<Curve> curves;
	for (size_t i = 0; i < 4; i++)
	{
		curves.push_back(Curve());
	}

	curves[0].addControlPoint(0.0f, 0.0f, 0.0f);
	curves[0].addControlPoint(5.0f, -10.0f, 0.0f);
	curves[0].addControlPoint(10.0f, -5.0f, 15.0f);
	curves[0].addControlPoint(15.0f, 0.0f, 15.0f);

	curves[1].addControlPoint(15.0f, 0.0f, 15.0f);
	curves[1].addControlPoint(20.0f, 0.0f, 15.0f);
	curves[1].addControlPoint(5.0f, 0.0f, 30.0f);
	curves[1].addControlPoint(0.0f, 0.0f, 30.0f);

	curves[2].addControlPoint(0.0f, 0.0f, 30.0f);
	curves[2].addControlPoint(-5.0f, 10.0f, 30.0f);
	curves[2].addControlPoint(-10.0f, 5.0f, 15.0f);
	curves[2].addControlPoint(-15.0f, 0.0f, 15.0f);

	curves[3].addControlPoint(-15.0f, 0.0f, 15.0f);
	curves[3].addControlPoint(-20.0f, 0.0f, 15.0f);
	curves[3].addControlPoint(-5.0f, 0.0f, 0.0f);
	curves[3].addControlPoint(0.0f, 0.0f, 0.0f);

	Path path(curves);
//--------------------------------------setting render settings----------------------------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	
//--------------------------------------initializing animation variables----------------------------------
	double prev_time = glfwGetTime();
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
	bool firstPersonWasOff = true;
	bool firstPersonWasOn = false;
	bool brakesWereOff = true;
	bool brakesWereOn = false;
	int currentPoint = 0;
	//End Temp vars
	
	Framebuffer framebuffer(width, height, "framebuffer.vert", "framebuffer.frag");


//--------------------------------------rendering----------------------------------
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

		camera.updateMatrix(90.0f, 0.1f, 100.0f);

		//-----------------------------------------------------------------Object Movement Zone-------------------------------------------------------------------
		if (camera.getMoveCamera()){ //If camera is first person
			if(firstPersonWasOff)
				engine->play2D("Media/eyebrow.wav");
			camera.setPosition(curvePoints[currentPoint] + glm::vec3(0.0f, 5.0f, 0.0f)); //Move camera
			firstPersonWasOff = false; 
			firstPersonWasOn = true;
		}
		else {
			if(firstPersonWasOn)
				engine->play2D("Media/eyebrow.wav");
			firstPersonWasOff = true;
			firstPersonWasOn = false;
		}
		if (camera.getMoveObject()) {
			if (brakesWereOn)
				engine->play2D("Media/motor.wav");
			brakesWereOff = true; 
			brakesWereOn = false;
			if (curr_time - prev_time_backpack >= (double)(0.05f)) {

				if (!(currentPoint + 1 < path.pathPoints.size()))
					temp = path.pathPoints[currentPoint] - path.pathPoints[0];
				else
					temp = path.pathPoints[currentPoint] - path.pathPoints[currentPoint + 1];

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
				trans = path.pathPoints[currentPoint];
				++currentPoint;
				if (!(currentPoint < path.pathPoints.size()))
					currentPoint = 0;
				prev_time_backpack = glfwGetTime();
			}
		}
		else {
			if (brakesWereOff)
				engine->play2D("Media/brakes.wav");
			brakesWereOff = false;
			brakesWereOn = true;
		}

		//-----------------------------------------------------------------End of Zone------------------------------------------------------------

		path.Draw(lineProgram, camera);

		glDisable(GL_CULL_FACE);
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightCube.Draw(lightProgram, camera, lightModel);
		glEnable(GL_CULL_FACE);

		glCullFace(GL_BACK);
		if(!camera.getMoveCamera())// If first person don't draw the moving object
			backpack.Draw(shaderProgram, camera, trans, rot);
		crow.Draw(shaderProgram, camera, { 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f, 0.0f}, { 0.5f, 0.5f, 0.5f });

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
