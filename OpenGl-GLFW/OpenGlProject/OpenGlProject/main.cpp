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
	Shader pickProgram("default.vert", "pick.frag", "default.geom"); // hehehe
	Shader lightProgram("light.vert", "light.frag");
	Shader lineProgram("line.vert", "line.frag");

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 sunColor = glm::vec4((243.0f / 255.0f), (100.0f / 255.0f), (20.0f / 255.0f), 1.0f);
	sunColor *= 10000;
	lightColor *= 50;
	glm::vec3 earthPos = glm::vec3(0.0f, -10.0f, -15.0f);
	glm::vec3 sunPos = glm::vec3(-100.0f, 20.0f, 90.0f);

	shaderProgram.Activate();
	shaderProgram.setVec3("dirLight.color", 0.0f, 0.0f, 0.0f);
	shaderProgram.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	shaderProgram.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	shaderProgram.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	shaderProgram.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);	

	shaderProgram.setVec3("pointLights[0].position", earthPos);
	shaderProgram.setVec3("pointLights[0].color", lightColor);
	shaderProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	shaderProgram.setVec3("pointLights[0].diffuse", 0.4f, 0.4f, 0.4f);
	shaderProgram.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.5f);
	shaderProgram.setFloat("pointLights[0].constant", 1.0f);
	shaderProgram.setFloat("pointLights[0].linear", 0.09f);
	shaderProgram.setFloat("pointLights[0].quadratic", 0.032f);	

	shaderProgram.setVec3("pointLights[1].position", sunPos);
	shaderProgram.setVec3("pointLights[1].color", sunColor);
	shaderProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	shaderProgram.setVec3("pointLights[1].diffuse", 0.4f, 0.4f, 0.4f);
	shaderProgram.setVec3("pointLights[1].specular", 0.5f, 0.5f, 0.5f);
	shaderProgram.setFloat("pointLights[1].constant", 1.0f);
	shaderProgram.setFloat("pointLights[1].linear", 0.09f);
	shaderProgram.setFloat("pointLights[1].quadratic", 0.032f);

	lightProgram.Activate();
	lightProgram.setVec4("lightColor", lightColor);

	lineProgram.Activate();
	lineProgram.setVec4("color", 0.5f, 0.5f, 0.3f, 1.0f);

//--------------------------------------initializing camera----------------------------------
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

//--------------------------------------initializing models----------------------------------
	std::string parentDir = (std::filesystem::current_path().std::filesystem::path::parent_path()).string();
	std::string modelDir = "/Models/";

	std::vector<Texture> textures;

	Mesh lightCube(lightVertices, lightIndices, textures);

	Model astronaut((parentDir + modelDir + "astronaut/scene.gltf"), 0);
	Model earth((parentDir + modelDir + "earth/scene.gltf"), 1);
	Model sun((parentDir + modelDir + "sun/scene.gltf"), 2);
	Model spaceship((parentDir + modelDir + "spaceship/scene.gltf"), 3);

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
	bool earthWasoff = false;
	bool sunWasoff = false;
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

		glClearColor(pow((7.0f/255.0f), gamma), pow((8.0f/255.0f), gamma), pow((8.0f/255.0f), gamma), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);


		if (curr_time - prev_time >= (double)(1 / 60)) {
			camera.Inputs(window);
			prev_time = glfwGetTime();
		}

		camera.updateMatrix(90.0f, 0.1f, 1000.0f);
		//-----------------------------------------------------------------Pickking Zone-------------------------------------------------------------------
		if (camera.mouseReleased){
			glCullFace(GL_BACK);
			if (!camera.getMoveCamera())// If first person don't draw the moving object
				spaceship.Draw(pickProgram, camera, trans, rot, glm::vec3(0.5f));
			earth.Draw(pickProgram, camera, earthPos, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(10.0f));
			sun.Draw(pickProgram, camera, sunPos, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f));
			astronaut.Draw(pickProgram, camera, glm::vec3(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.2f));
			glCullFace(GL_FRONT);

			glFlush();
			glFinish();

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			unsigned char data[4];
			glReadPixels(width / 2, height / 2, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
			int pickedID =
				data[0] +
				data[1] * 256 +
				data[2] * 256 * 256;
			//-----------------------------------------------------------------Pickking Effect Zone-------------------------------------------------------------------
			if (pickedID == earth.id) {
				shaderProgram.Activate();
				if (earthWasoff) {
					shaderProgram.setVec3("pointLights[0].color", lightColor);
					earthWasoff = false;
				}
				else
				{
					shaderProgram.setVec3("pointLights[0].color", glm::vec3(0.0f));
					earthWasoff = true;
				}
					
			}
			else if(pickedID == sun.id)
			{
				shaderProgram.Activate();
				if (sunWasoff) {
					shaderProgram.setVec3("pointLights[1].color", sunColor);
					sunWasoff = false;
				}
				else
				{
					shaderProgram.setVec3("pointLights[1].color", glm::vec3(0.0f));
					sunWasoff = true;
				}		
			}
		}

		//-----------------------------------------------------------------Object Movement Zone-------------------------------------------------------------------
		if (camera.getMoveCamera()){ //If camera is first person
			if(firstPersonWasOff)
				engine->play2D("Media/eyebrow.wav");
			camera.setPosition(path.pathPoints[currentPoint] + glm::vec3(0.0f, 5.0f, 0.0f)); //Move camera
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

		glCullFace(GL_BACK);
		if(!camera.getMoveCamera())// If first person don't draw the moving object
			spaceship.Draw(shaderProgram, camera, trans, rot, glm::vec3(0.5f));
		earth.Draw(shaderProgram, camera, earthPos, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(10.0f));
		sun.Draw(shaderProgram, camera, sunPos, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(2.0f));
		astronaut.Draw(shaderProgram, camera, glm::vec3(1.0f), glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3(0.2f));
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
