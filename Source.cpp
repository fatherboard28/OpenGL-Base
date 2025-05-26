#define RESOURCES "./resources/"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include "Mesh.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposin, double yposin);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubemap(std::vector<std::string> faces);
void setBlock(Shader shader, glm::vec3 coord, glm::vec3 color);

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//create glfw window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

	//check if glfw is working
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	//setting the callback function to run on every frame
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//check if glad is working
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to load GLAD" << std::endl;
		return -1;
	}

	//hide / capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//enables depth testing
	glEnable(GL_DEPTH_TEST);

	Shader lightCube(".\\shaders\\cube_shader.vs", ".\\shaders\\cube_shader.fs");
	Shader skyBoxShader(".\\shaders\\skybox.vs", ".\\shaders\\skybox.fs");

	Shader block_shader(".\\shaders\\shader.vs", ".\\shaders\\shader.fs");

	//cube vertices
	float SIZE = 0.5f;
	float cube_vertices[] = {
	-SIZE, -SIZE, -SIZE,  0.0f,  0.0f, -1.0f,
	 SIZE, -SIZE, -SIZE,  0.0f,  0.0f, -1.0f,
	 SIZE,  SIZE, -SIZE,  0.0f,  0.0f, -1.0f,
	 SIZE,  SIZE, -SIZE,  0.0f,  0.0f, -1.0f,
	-SIZE,  SIZE, -SIZE,  0.0f,  0.0f, -1.0f,
	-SIZE, -SIZE, -SIZE,  0.0f,  0.0f, -1.0f,
                                    
	-SIZE, -SIZE,  SIZE,  0.0f,  0.0f,  1.0f,
	 SIZE, -SIZE,  SIZE,  0.0f,  0.0f,  1.0f,
	 SIZE,  SIZE,  SIZE,  0.0f,  0.0f,  1.0f,
	 SIZE,  SIZE,  SIZE,  0.0f,  0.0f,  1.0f,
	-SIZE,  SIZE,  SIZE,  0.0f,  0.0f,  1.0f,
	-SIZE, -SIZE,  SIZE,  0.0f,  0.0f,  1.0f,
                                    
	-SIZE,  SIZE,  SIZE, -1.0f,  0.0f,  0.0f,
	-SIZE,  SIZE, -SIZE, -1.0f,  0.0f,  0.0f,
	-SIZE, -SIZE, -SIZE, -1.0f,  0.0f,  0.0f,
	-SIZE, -SIZE, -SIZE, -1.0f,  0.0f,  0.0f,
	-SIZE, -SIZE,  SIZE, -1.0f,  0.0f,  0.0f,
	-SIZE,  SIZE,  SIZE, -1.0f,  0.0f,  0.0f,
                                    
	 SIZE,  SIZE,  SIZE,  1.0f,  0.0f,  0.0f,
	 SIZE,  SIZE, -SIZE,  1.0f,  0.0f,  0.0f,
	 SIZE, -SIZE, -SIZE,  1.0f,  0.0f,  0.0f,
	 SIZE, -SIZE, -SIZE,  1.0f,  0.0f,  0.0f,
	 SIZE, -SIZE,  SIZE,  1.0f,  0.0f,  0.0f,
	 SIZE,  SIZE,  SIZE,  1.0f,  0.0f,  0.0f,
                                    
	-SIZE, -SIZE, -SIZE,  0.0f, -1.0f,  0.0f,
	 SIZE, -SIZE, -SIZE,  0.0f, -1.0f,  0.0f,
	 SIZE, -SIZE,  SIZE,  0.0f, -1.0f,  0.0f,
	 SIZE, -SIZE,  SIZE,  0.0f, -1.0f,  0.0f,
	-SIZE, -SIZE,  SIZE,  0.0f, -1.0f,  0.0f,
	-SIZE, -SIZE, -SIZE,  0.0f, -1.0f,  0.0f,
                                    
	-SIZE,  SIZE, -SIZE,  0.0f,  1.0f,  0.0f,
	 SIZE,  SIZE, -SIZE,  0.0f,  1.0f,  0.0f,
	 SIZE,  SIZE,  SIZE,  0.0f,  1.0f,  0.0f,
	 SIZE,  SIZE,  SIZE,  0.0f,  1.0f,  0.0f,
	-SIZE,  SIZE,  SIZE,  0.0f,  1.0f,  0.0f,
	-SIZE,  SIZE, -SIZE,  0.0f,  0.0f,  0.0f
	};
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	/*
	// .. :: Initialization code :: ..
	unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	// 1. bind Vertex Array object
	glBindVertexArray(cubeVAO);

	// 2. copy vertices array in a vertex buffer for OpenGL to use --- setup vbo
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

	// 3. then set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//texture pos attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/

	unsigned int lightBoiVAO;
	glGenVertexArrays(1, &lightBoiVAO);
	glBindVertexArray(lightBoiVAO);

	unsigned int VBO;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

	//SKY BOX
	unsigned int skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	

	std::vector<std::string> faces
	{
		".\\resources\\basic_skybox\\top.jpg",
		".\\resources\\basic_skybox\\top.jpg",
		".\\resources\\basic_skybox\\top.jpg",
		".\\resources\\basic_skybox\\top.jpg",
		".\\resources\\basic_skybox\\top.jpg",
		".\\resources\\basic_skybox\\top.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	
	//render loop. each time this runs is a frame
	while (!glfwWindowShouldClose(window)) {
		//per frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render container
		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 32; i++) {
			for (unsigned int j = 0; j < 32; j++) {
				setBlock(block_shader, glm::vec3(i, -2, j), glm::vec3(45.0f, 156.0f, 69.0f));
			}
		}
		setBlock(block_shader, glm::vec3(2, -1, 2), glm::vec3(45.0f, 156.0f, 69.0f));

		setBlock(block_shader, lightPos, glm::vec3(255, 155, 155));

		glBindVertexArray(lightBoiVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//skybox
		glDepthFunc(GL_LEQUAL);
		skyBoxShader.use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyBoxShader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		skyBoxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);
		//------


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//cleanup
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

/*
Places a block with a given shader, coordinate, and color
*/
void setBlock(Shader shader, glm::vec3 coord, glm::vec3 color) {
	shader.use();

	//Color
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos(0.0f, 5.0f, 0.0f);
	shader.setVec3("lightPos", lightPos);

	//model transformation
	glm::mat4 model = glm::mat4(1.0f);

	//Translate the block to the given coords
	model = glm::translate(model, coord);
	shader.setMat4("model", model);

	//view transformation. From World -> View
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("view", view);

	//projection transformation. From View -> Clip
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	shader.setMat4("projection", projection);

	//Sets the color of the block
	//TODO: make this a texture not a color
	shader.setVec3("Color", (color / 255.0f));

	glDrawArrays(GL_TRIANGLES, 0, 36);

}

//callback funtion to resize window when user does it by hand
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

//if you press escape you will close the app
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//for the skybox
unsigned int loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}

