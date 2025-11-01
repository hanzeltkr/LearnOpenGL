#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"


GLfloat mixValue = 0.2f;

GLint screenWidth = 800;
GLint screenHeight = 600;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void Do_Movement();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void right_button_callback(GLFWwindow* window, int button, int action, int mods);

GLuint loadCubemap(vector<const GLchar*> texture_faces);



glm::vec3 playerCubePos(0.0f, 0.0f, 0.0f);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool rightMousePressed = false;



GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::vec3 lightPos(0.0f, 0.0f, 3.0f);




int main() {



	// Tell GLFW what version of OpenGL we are using 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);



	// Create a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load GLAD
	gladLoadGL();

	glEnable(GL_DEPTH_TEST); 

	// Tell OpenGL the viewport size
	glViewport(0, 0, screenWidth, screenHeight);



	// Register Mouse and Scroll Callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Optional: hide and grab cursor
	glfwSetMouseButtonCallback(window, right_button_callback);



	// Register the function with proper callback
	glfwSetKeyCallback(window, key_callback);




	// Render a triangle-------------------------------------------------------
	float vertices1[] = {
		//	 positions	//	 normals		// texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
		

	/*GLuint indices1[] = {
		0, 1, 3,
		1, 2, 3
	};*/


	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f)
	};



	//---------------------------------------------------------------------------
	// Create Vertex Array Object (VAO)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create Vertex Buffer Object (VBO)
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	/*
	// Create Element Buffer Object (EBO)
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);
	*/

	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Set color attributes pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Lamp VAO
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0); // Unbind VAO (optional, but good practice)



	// Create and compile shaders
	Shader ourShader("vertex.vert", "mirror.frag");
	ourShader.Use();
	GLfloat horizontalOffset = 0.0f;
	GLfloat verticalOffset = 0.0f;
	GLint OffsetLocation = glGetUniformLocation(ourShader.Program, "Offset");
	glUniform2f(OffsetLocation, horizontalOffset, verticalOffset);
	GLint lightColorLoc = glGetUniformLocation(ourShader.Program, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f); // Set the color of the light

	Shader shaderSingleColor("vertex.vert", "shaderSingleColor.frag");



	// Load texture
	/*int width, height, w1, h1, specW, specH, emisW, emisH;
	unsigned char* image = SOIL_load_image("crate_diffuse.png", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char* image2 = SOIL_load_image("face.png", &w1, &h1, 0, SOIL_LOAD_RGB);
	unsigned char* spec1 = SOIL_load_image("crate_specular.png", &specW, &specH, 0, SOIL_LOAD_RGB);
	unsigned char* emis1 = SOIL_load_image("crate_emission.jpg", &emisW, &emisH, 0, SOIL_LOAD_RGB);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Load and generate the texture 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D); 



	GLuint specularMap;
	glGenTextures(1, &specularMap);
	glBindTexture(GL_TEXTURE_2D, specularMap);
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Load and generate the texture 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, specW, specH, 0, GL_RGB, GL_UNSIGNED_BYTE, spec1);
	glGenerateMipmap(GL_TEXTURE_2D);



	GLuint emissionMap;
	glGenTextures(1, &emissionMap);
	glBindTexture(GL_TEXTURE_2D, emissionMap);
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Load and generate the texture 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, emisW, emisH, 0, GL_RGB, GL_UNSIGNED_BYTE, emis1);
	glGenerateMipmap(GL_TEXTURE_2D);



	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w1, h1, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	SOIL_free_image_data(image2);
	SOIL_free_image_data(spec1);
	SOIL_free_image_data(emis1);
	glBindTexture(GL_TEXTURE_2D, 0);*/




	// Image loading
	int width, height;
	unsigned char* image = SOIL_load_image("window.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	float grassVertices[] = {
		// positions        // texture coords
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,

		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f
	};

	vector<glm::vec3> vegetation;
	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	GLuint vegetationVAO, vegetationVBO;
	glGenVertexArrays(1, &vegetationVAO);
	glGenBuffers(1, &vegetationVBO);
	glBindVertexArray(vegetationVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);
	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Set texture attributes pointers
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	
	Shader grassShader("vertex2.vert", "fragment.frag");

	GLuint grassTexture;
	glGenTextures(1, &grassTexture);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture




	// Screen quad for post-processing------------------------------------------------

	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// Render buffer object
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check completeness AFTER all attachments
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);



	GLuint quadVAO, quadVBO;
	GLfloat quadVertices[] = {
		// Positions        // Texture Coords
		-0.3f,  1.0f, 0.0f, 0.0f, 1.0f,
		-0.3f, 0.7f, 0.0f, 0.0f, 0.0f,
		0.3f, 0.7f, 0.0f, 1.0f, 0.0f,

		-0.3f,  1.0f, 0.0f, 0.0f, 1.0f,
		0.3f, 0.7f, 0.0f, 1.0f, 0.0f,
		0.3f,  1.0f, 0.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	Shader screenShader("quad.vert", "quad.frag");




	//--------------------------------------------------------------------------------------------
	// Transformations
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans(1.0f);
	//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
	trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	vec = trans * vec;
	GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	



	// Set up the model, view and projection matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

	// Camera position
	view = camera.GetViewMatrix();

	// Get their uniform location
	GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
	// Pass them to the shaders
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	// Note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	

	GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");
	glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); // Set the camera position




	//--------------------------------------------------------------------------------------
	// Lighting shader

	Shader lampShader("lamp.vert", "lamp.frag");
	lampShader.Use();
	GLint lampPosLoc = glGetUniformLocation(lampShader.Program, "lightPos");
	glUniform3f(lampPosLoc, lightPos.x, lightPos.y, lightPos.z); // Set the position of the light

	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // Make it smaller
	GLint lampModelLoc = glGetUniformLocation(lampShader.Program, "model");
	glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(model));
	GLint lampViewLoc = glGetUniformLocation(lampShader.Program, "view");
	GLint lampProjLoc = glGetUniformLocation(lampShader.Program, "projection");

	

	ourShader.Use();
	// Set the material properties
	GLint matDiffuseLoc = glGetUniformLocation(ourShader.Program, "material.diffuse");
	GLint matSpecularLoc = glGetUniformLocation(ourShader.Program, "material.specular");
	GLint matShininessLoc = glGetUniformLocation(ourShader.Program, "material.shininess");
	GLint matEmissionLoc = glGetUniformLocation(ourShader.Program, "material.emission");

	glUniform1i(matDiffuseLoc, 0);
	glUniform1i(matSpecularLoc, 1);
	glUniform1f(matShininessLoc, 32.0f); 
	glUniform1i(matEmissionLoc, 2);

	// Set the light properties
	GLint lightAmbientLoc = glGetUniformLocation(ourShader.Program, "dirLight.ambient");
	GLint lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "dirLight.diffuse");
	GLint lightSpecularLoc = glGetUniformLocation(ourShader.Program, "dirLight.specular");

	glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lightDiffuseLoc, 3.0f, 3.0f, 3.0f);
	glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

	// For directional light
	GLint lightDirectionLoc = glGetUniformLocation(ourShader.Program, "dirLight.direction");
	glUniform3f(lightDirectionLoc, -0.2f, -1.0f, -0.3f);

	// For point light
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].specular"),1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);

	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].constant"), 1.0f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].linear"), 0.09f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].quadratic"), 0.032f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};

	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
	glUniform3f(glGetUniformLocation(ourShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);

	// For spot light
	lightDirectionLoc = glGetUniformLocation(ourShader.Program, "spotLight.direction");
	GLint lightCutOffLoc = glGetUniformLocation(ourShader.Program, "spotLight.cutOff");
	GLint outerCutOffLoc = glGetUniformLocation(ourShader.Program, "spotLight.outerCutOff");
	GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "spotLight.position");
	glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); 
	glUniform3f(lightDirectionLoc, camera.Front.x, camera.Front.y, camera.Front.z);
	glUniform1f(lightCutOffLoc, glm::cos(glm::radians(12.5f))); // Inner cut-off angle
	glUniform1f(outerCutOffLoc, glm::cos(glm::radians(17.5f))); // Outer cut-off angle*/
	glUniform3f(glGetUniformLocation(ourShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(ourShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "spotLight.constant"), 1.0f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "spotLight.linear"), 0.09f);
	glUniform1f(glGetUniformLocation(ourShader.Program, "spotLight.quadratic"), 0.032f);



	shaderSingleColor.Use();
	modelLoc = glGetUniformLocation(shaderSingleColor.Program, "model");
	viewLoc = glGetUniformLocation(shaderSingleColor.Program, "view");
	projLoc = glGetUniformLocation(shaderSingleColor.Program, "projection");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


	// Bind textures to texture units
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionMap);*/




	//--------------------------------------------------------------------------------
	// Model loading
	Model bag("Model/backpack/backpack.obj");

	//---------------------------------------------------------------------------------
	// Cubemap loading
	
	// Inputting texture faces
	vector<const GLchar*> texture_faces;
	texture_faces.push_back("skybox/skybox/right.jpg");
	texture_faces.push_back("skybox/skybox/left.jpg");
	texture_faces.push_back("skybox/skybox/top.jpg");
	texture_faces.push_back("skybox/skybox/bottom.jpg");
	texture_faces.push_back("skybox/skybox/front.jpg");
	texture_faces.push_back("skybox/skybox/back.jpg");
	GLuint cubemapTexture = loadCubemap(texture_faces);
	
	Shader skyboxShader("skybox.vert", "skybox.frag");

	GLuint skyboxVAO, skyboxVBO;
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
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	
	



	// Extra code here

	// Set the polygon mode to wireframe (optional, for debugging)
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// For x-ray vision
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;*/

	// Get the maximum number of texture units available
	/*int maxTextures = 0;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextures);
	std::cout << "Max texture units: " << maxTextures << std::endl;*/	




	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL); // Set depth function to less than or equal to ensure correct depth testing
	glEnable(GL_STENCIL_TEST); // Enable stencil testing
	
	glEnable(GL_BLEND); // Enable blending for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set the blending function for transparency

	//glEnable(GL_CULL_FACE); // Enable face culling
	//glCullFace(GL_BACK); // Cull back faces
	//glFrontFace(GL_CW); // Set counter-clockwise vertex order as front face

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		
		// Check and call events
		glfwPollEvents();

		Do_Movement(); // Process input

		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear the color and depth buffers



		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		

		// Rendering commands here

		// Set color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);



		ourShader.Use();
		// Draw the triangle

		//glUniform1f(glGetUniformLocation(ourShader.Program, "mixValue"), mixValue);

		
		/*
		// Making rear view mirror
		camera.Yaw += 180.0f; // Rotate the camera 180 degrees around the Y-axis
		camera.ProcessMouseMovement(0.0f, 0.0f, false); // Update the camera vectors based on the new yaw value
		view = camera.GetViewMatrix();
		camera.Yaw -= 180.0f; // Rotate the camera back to its original orientation
		camera.ProcessMouseMovement(0.0f, 0.0f, true);
		*/


		view = camera.GetViewMatrix();
		viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
		projLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
		
		// For spot light
		glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightDirectionLoc, camera.Front.x, camera.Front.y, camera.Front.z);
		/*std::cout << "Camera Position: " << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << std::endl;
		std::cout << "Camera Front: " << camera.Front.x << ", " << camera.Front.y << ", " << camera.Front.z << std::endl;*/



		glBindVertexArray(VAO);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Set stencil buffer operations
		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Always pass stencil test and set stencil value to 1
		glStencilMask(0xFF); // Enable writing to the stencil buffer
		glm::mat4 model = glm::mat4(1.0f);
		modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bag.Draw(ourShader); // Draw the model


		
		// Draw the lamp
		glBindVertexArray(lightVAO);
		lampShader.Use();

		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(lampProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		//----------------------------------------------------------------------------
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // Clear the color and depth buffers



		view = camera.GetViewMatrix();
		ourShader.Use();
		glBindVertexArray(VAO);
		viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(lightPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform3f(lightDirectionLoc, camera.Front.x, camera.Front.y, camera.Front.z);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glUniform1i(glGetUniformLocation(ourShader.Program, "skybox"), 0);
		GLint cameraPosLoc = glGetUniformLocation(ourShader.Program, "cameraPos");
		glUniform3f(cameraPosLoc, camera.Position.x, camera.Position.y, camera.Position.z); // Update camera position

		model = glm::mat4(1.0f);
		modelLoc = glGetUniformLocation(ourShader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		bag.Draw(ourShader); // Draw the model



		// Draw the lamps
		glBindVertexArray(lightVAO);
		lampShader.Use();

		for (GLuint i = 0; i < 4; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
			glUniformMatrix4fv(lampModelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glUniformMatrix4fv(lampViewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(lampProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		// Draw skybox last
		glDepthMask(GL_FALSE);
		skyboxShader.Use();

		// Getting current view and projection for fragment and vertex shader
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		viewLoc = glGetUniformLocation(skyboxShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
		projLoc = glGetUniformLocation(skyboxShader.Program, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Texture setup for skybox
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);

		// Draw the skybox
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);


		//-----------------------------------------------------------------------------
		
		
		screenShader.Use();
		glBindVertexArray(quadVAO);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(screenShader.Program, "screenTexture"), 0); // Set the uniform to use texture unit 0		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		


		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
		glBindVertexArray(0); // Unbind VAO (optional, but good practice)
		glEnable(GL_DEPTH_TEST);
		


		// Swap buffers
		glfwSwapBuffers(window);
	}
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) std::cout << "OpenGL error: " << err << std::endl;



	glStencilMask(0xFF); // Re-enable writing to the stencil buffer
	glStencilFunc(GL_ALWAYS, 0, 0xFF); // Reset stencil buffer
	glEnable(GL_DEPTH_TEST); // Re-enable depth testing




	// Clean and delete all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &lightVAO);
	glDeleteBuffers(1, &vegetationVAO);
	glDeleteBuffers(1, &vegetationVBO);
	glDeleteBuffers(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &fbo);
	glDeleteBuffers(1, &rbo);
	//glDeleteTextures(1, &texture1);
	//glDeleteTextures(1, &texture2);
	glDeleteProgram(ourShader.Program);
	glDeleteProgram(lampShader.Program);

	glfwTerminate();

	return 0;
}



// Define a callback function for key events
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	// Set WindowShouldClose to true if escape key is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_UP) {
		mixValue += 0.05f;
		if (mixValue >= 1.0f) {
			mixValue = 1.0f;
		}
		playerCubePos.y += 0.1f;
	}

	if (key == GLFW_KEY_DOWN) {
		mixValue -= 0.05f;
		if (mixValue <= 0.0f) {
			mixValue = 0.0f;
		}
		playerCubePos.y -= 0.1f;
	}
	if (key == GLFW_KEY_LEFT) {
		playerCubePos.x -= 0.1f;
	}
	if (key == GLFW_KEY_RIGHT) {
		playerCubePos.x += 0.1f;
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

}

void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	// Light controls
	if (keys[GLFW_KEY_UP])
		lightPos.y += 0.1f;
	if (keys[GLFW_KEY_DOWN])
		lightPos.y -= 0.1f;
	if (keys[GLFW_KEY_LEFT])
		lightPos.x -= 0.1f;
	if (keys[GLFW_KEY_RIGHT])
		lightPos.x += 0.1f;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!rightMousePressed) return;

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

void right_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			rightMousePressed = true;
			firstMouse = true;
		}
		else if (action == GLFW_RELEASE)
			rightMousePressed = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

GLuint loadCubemap(vector<const GLchar*> texture_faces) {

	// Generating a texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	// Load each face of the cubemap
	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < texture_faces.size(); i++) {
		image = SOIL_load_image(texture_faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		if (!image) {
			std::cerr << "Failed to load cubemap face: " << texture_faces[i] << std::endl;
			continue;
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0); // Unbind the texture

	return textureID;
}