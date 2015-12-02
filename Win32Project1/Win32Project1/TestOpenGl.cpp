//#include <GL\glew.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "vector3.h"
#include <chrono>
#include "SOIL/src/SOIL.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assert.h>
#include "../Obj_Collisions_Layer/Obj_Reader.h"

// Do not include the OpenGL headers yourself, as GLFW does this for you
// Do not include windows.h or other platform - specific headers unless you plan on using those APIs directly
// If you do need to include such headers, do it before including the GLFW one and it will detect this

// Shader sources
const GLchar* vertexSource =
"#version 150 core\n"
"in vec3 position;"
"in vec3 color;"
"in vec2 texcoord;"
"out vec3 Color;"
"out vec2 Texcoord;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 proj;"
"void main() {"
"	Color = vec3(1.0,1.0,1.0);"
"	Texcoord = texcoord;"
"	gl_Position =  proj * view * model * vec4(position / 10, 1.0);"
"}";

const GLchar* fragmentSource =
"#version 150 core\n"
"in vec3 Color;"
"in vec2 Texcoord;"
"out vec4 outColor;"
"uniform sampler2D tex;"
"void main() {"
"	vec4 texColor = texture(tex, Texcoord);"
"   outColor = texColor;"
"}";

const int iScreenSizeX = 640;
const int iScreenSizeY = 480;
GLint uniColor;
GLuint tex[2];
GLuint iShaderProgram;
auto t_start = std::chrono::high_resolution_clock::now();

vector<float> vVertice;
vector<float> CreateVerticeTab()
{

	// on a inversée les y et les z

	vector<float> vVertice;
	for (unsigned int i = 0; i < ObjReader::GetFaceList().size(); i++)
	{
		const int fVertex1 = ObjReader::GetFaceList()[i].v_first;
		const int fVertex2 = ObjReader::GetFaceList()[i].v_second;
		const int fVertex3 = ObjReader::GetFaceList()[i].v_third;

		const int fUV1 = ObjReader::GetFaceList()[i].v_first;
		const int fUV2 = ObjReader::GetFaceList()[i].v_second;
		const int fUV3 = ObjReader::GetFaceList()[i].v_third;

		vVertice.push_back(ObjReader::GetVertex(fVertex1 - 1).x);
		vVertice.push_back(ObjReader::GetVertex(fVertex1 - 1).z);
		vVertice.push_back(ObjReader::GetVertex(fVertex1 - 1).y);

		vVertice.push_back(ObjReader::GetUV(fUV1 - 1).u);
		vVertice.push_back(ObjReader::GetUV(fUV1 - 1).v);

		vVertice.push_back(ObjReader::GetVertex(fVertex2 - 1).x);
		vVertice.push_back(ObjReader::GetVertex(fVertex2 - 1).z);
		vVertice.push_back(ObjReader::GetVertex(fVertex2 - 1).y);

		vVertice.push_back(ObjReader::GetUV(fUV2 - 1).u);
		vVertice.push_back(ObjReader::GetUV(fUV2 - 1).v);

		vVertice.push_back(ObjReader::GetVertex(fVertex3 - 1).x);
		vVertice.push_back(ObjReader::GetVertex(fVertex3 - 1).z);
		vVertice.push_back(ObjReader::GetVertex(fVertex3 - 1).y);

		vVertice.push_back(ObjReader::GetUV(fUV3 - 1).u);
		vVertice.push_back(ObjReader::GetUV(fUV3 - 1).v);
	}
	return vVertice;
}
vector<GLint> CreateElementTab()
{
	vector<GLint> vElement;
	for (int i = 0; i < ObjReader::GetFaceList().size() / 2.0f; ++i)
	{
		vElement.push_back(ObjReader::GetFaceList()[i].v_first);
		vElement.push_back(ObjReader::GetFaceList()[i + 1].v_second);
		vElement.push_back(ObjReader::GetFaceList()[i + 2].v_third);
	}
	return vElement;
}
GLuint CreateAndCompileVertexShader(const GLchar * cVertex)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // create a shader
	glShaderSource(vertexShader, 1, &cVertex, NULL); // take the "shadercode" and put him in the shader;
	glCompileShader(vertexShader); // Compile the shader into code that can be executed by the graphics card now

	return vertexShader;
}
GLuint CreateAndCompileFragmentShader(const GLchar * cFragment)
{
	// The fragment shader is compiled in exactly the same way

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cFragment, NULL);
	glCompileShader(fragmentShader);

	return fragmentShader;
}
GLuint CombineShaderPrograme(GLuint iVertexShader, GLuint iFragmentShader)
{
	GLuint shaderProgram = glCreateProgram(); // create shader program
	glAttachShader(shaderProgram, iVertexShader); // assign vertex to him
	glAttachShader(shaderProgram, iFragmentShader); // assign fragment to him

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram); //the actual result will not change until a program has been linked again.

	glUseProgram(shaderProgram); // To actually start using the shaders in the program

	return shaderProgram;
}
void MakingLinkVertxDataToAttributs(GLuint ShaderProgram)
{
	GLint posAttrib = glGetAttribLocation(ShaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	GLint texAttrib = glGetAttribLocation(ShaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
}
void CreateGladBuffer()
{
	// Create Vertex Array Object

	vVertice = CreateVerticeTab();
	const GLfloat * fVertice = vVertice.data();
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo; // Create a Vertex Buffer Object and copy the vertex data to it
	glGenBuffers(1, &vbo); // generate 1 buffer

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // actual buffer

	glBufferData(GL_ARRAY_BUFFER, vVertice.size() * sizeof(float), fVertice, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	vector <GLint> vElement = CreateElementTab();
	const GLint * iElement = vElement.data();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vElement.size() * sizeof(GLint), iElement, GL_STATIC_DRAW);

	GLuint iVertexShader = CreateAndCompileVertexShader(vertexSource);
	GLuint iFragmentShader = CreateAndCompileFragmentShader(fragmentSource);

	GLint iStatut;
	glGetShaderiv(iVertexShader, GL_COMPILE_STATUS, &iStatut);
	assert(iStatut);

	glGetShaderiv(iFragmentShader, GL_COMPILE_STATUS, &iStatut);
	assert(iStatut);

	iShaderProgram = CombineShaderPrograme(iVertexShader, iFragmentShader);
	MakingLinkVertxDataToAttributs(iShaderProgram);

	glEnable(GL_DEPTH_TEST);

}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void error_callback(int error, const char* description)
{
	puts(description);
}
void CreateBuffer(GLFWwindow* window)
{
	int iWidth, iHeight;

	glfwGetFramebufferSize(window, &iWidth, &iHeight); // This function retrieves the size, in pixels, of the framebuffer of the specified window

	const float fRatio = iWidth / (float)iHeight;
	glViewport(0, 0, iWidth, iHeight); // The framebuffer size needs to be retrieved for glViewport.
	glClear(GL_COLOR_BUFFER_BIT); // Bitwise OR of masks that indicate the buffers to be cleared. The three masks are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT.

	glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for subsequent matrix operations.
	glLoadIdentity(); // glLoadIdentity replaces the current matrix with the identity matrix.
	glOrtho(-fRatio, fRatio, -1.f, 1.f, 1.f, -1.f); // multiply the current matrix with an orthographic matrix

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void CreateTex()
{
	int width, height;
	unsigned char* image;
	glGenTextures(1, tex);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);

	image = SOIL_load_image("data/img.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(iShaderProgram, "tex"), 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
GLFWwindow * InitWindowsAndLibrary()
{
	if (!glfwInit()) // Before most GLFW functions may be called, the library must be initialized.  
	{
		printf("ERROR : glfw is not Init !");
		exit(EXIT_FAILURE); // stdl function
	}

	GLFWwindow * tWindow = glfwCreateWindow(iScreenSizeX, iScreenSizeY, "Test", NULL, NULL); // creat a windows with a size and a title

	if (!tWindow) // check if Windows != NULL
	{
		printf("ERROR : tWindow is not initialized !");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(tWindow); //This function makes the OpenGL context of the specified window current on the calling thread.

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // init glad
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		exit(EXIT_FAILURE);
	}

	return tWindow;
}
void SetCamera(glm::mat4 mView)
{
	GLint uniView = glGetUniformLocation(iShaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(mView));

	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(iShaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}
void SetPositionModel(GLint iModel, const glm::vec3 & vPosition, float fRotations)
{
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(fRotations), vPosition);
	glUniformMatrix4fv(iModel, 1, GL_FALSE, glm::value_ptr(model));
}
void Clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
int main()
{

	GLFWwindow * tWindow = InitWindowsAndLibrary();

	// ----- open file ------

	ObjReader::AnalyseFile("data/fps_level.obj");
	// ----- Test Glad ------

	CreateGladBuffer();

	// ----------------------
	glfwSwapInterval(1); // The swap interval indicates how many frames to wait until swapping the buffers

	glfwSetKeyCallback(tWindow, key_callback);	 // pas encore trop compris le principe	^^
	glfwSetErrorCallback(error_callback); // The error callback receives a description of the error and (when possible) its cause.

	CreateTex();

	glm::mat4 view = glm::lookAt
		(
		glm::vec3(1.5f, 1.5f, 1.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	SetCamera(view);
	GLint uniModel = glGetUniformLocation(iShaderProgram, "model");

	float angle = 0;

	while (!glfwWindowShouldClose(tWindow))	//the window isn't actually closed
	{
		SetPositionModel(uniModel, glm::vec3(0.0f, 0.0f, 1.0f), angle += 1);

		Clear();

		glDrawArrays(GL_TRIANGLES, 0, vVertice.size());

		glfwSwapBuffers(tWindow); // the GLFW use a double buffer, when the frame is rendered, the buffer need to be swapped.

		glfwPollEvents(); // call the event	 // if i press the key echap, that will be a record like a event.
	}

	glfwDestroyWindow(tWindow);
	glfwTerminate(); // When you are done using GLFW, just before the application exits
	exit(EXIT_SUCCESS); // the application exit
}