#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

//-------------------------------------------------
// Shader source code stored as char arrays
//-------------------------------------------------

const char* vertexShaderCode = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderCode = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";

int main()
{
	// GLFW initialization and specifiying OpenGL window context version 
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates the actual window, throws an error in case of failure
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Error! Could not create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))		// This function requires a valid OpenGL context 
	{
		std::cout << "Error! Could not load glad" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);		// Defines the size of the OpenGL rendering viewport, this is independent of window size

	// OpenGL version info and GPU currently in use
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//-------------------------------------------------
	// Shader compile functions
	//-------------------------------------------------

	// This is just a basic example on how to handle shaders in OpenGL

	unsigned int vertexShader, fragmentShader, shaderProgram;
	int status;
	char log[1024];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(vertexShader, 1024, NULL, log);
		std::cout << "Error! Vertex shader compilation failed!" << std::endl;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status)
	{
		glGetShaderInfoLog(fragmentShader, 1024, NULL, log);
		std::cout << "Error! Fragment shader compilation failed!" << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (!status)
	{
		glGetProgramInfoLog(shaderProgram, 1024, NULL, log);
		std::cout << "Error! Could not link shader program!" << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//-------------------------------------------------
	// Vertex data
	//-------------------------------------------------

	float vertices[] = {
		// Vertex Positions	// Vertex Colour information
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	//-------------------------------------------------
	// OpenGL buffers
	//-------------------------------------------------

	unsigned int VBO;
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 	// OpenGL core profile requires a VAO to be bound

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Copies the data from system RAM to GPU RAM

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);	// Defines the data layout of the VBO and stores it in the VAO
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 1);	// we can unbind the VBO because it has be registered to the VAO using glVertexAttribPointer

	//-------------------------------------------------
	// Main render loop
	//-------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Resource de-allocation for a cleaner exit. This is optionnal as the OS should handle this automatically

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();

	return 0;
}