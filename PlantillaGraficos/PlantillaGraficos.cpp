// PlantillaGraficos.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <math.h>

#include <iostream>

#include "Shader.h"
#include "Vertice.h"
#include <vector>
using namespace std;
//Cada elemento que queramos renderear necesita un vertex array y un buffer
vector<Vertice> triangulo;
GLuint vertexArrayTrianguloID;
GLuint bufferTrianguloID;

vector<Vertice> cuadrado;
GLuint vertexArrayCuadradoID;
GLuint bufferCuadradoID;

vector<Vertice> suelo;
GLuint vertexArraySueloID;
GLuint bufferSueloID;
//Instancia de shader
Shader *shader;
//Identificadores para mapeo de atributos de entrada del vertex shader
GLuint posicionID;
GLuint colorID;

void inicializarCuadrado() {
	Vertice v1 = {
		vec3(-0.2f,0.2f,0.0f),
		vec4(0.639f,0.588f,0.321f,1.0f)
	};
	Vertice v2 = {
		vec3(0.2f,0.2f,0.0f),
		vec4(0.639f,0.588f,0.321f,1.0f)
	};
	Vertice v3 = {
		vec3(0.2f,-0.2f,0.0f),
		vec4(0.796f,0.752f,0.501f,1.0f)
	};
	Vertice v4 = {
		vec3(-0.2f,-0.2f,0.0f),
		vec4(0.796f,0.752f,0.501f,1.0f)
	};
	Vertice v5 = {
		vec3(-0.9f,-0.3f,0.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};
	Vertice v6 = {
		vec3(0.9f,-0.3f,0.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};
	Vertice v7 = {
		vec3(0.9f,-0.5f,0.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};
	Vertice v8 = {
		vec3(-0.9f,-0.5f,0.0f),
		vec4(0.1f,0.8f,0.2f,1.0f)
	};
	cuadrado.push_back(v1);
	cuadrado.push_back(v2);
	cuadrado.push_back(v3);
	cuadrado.push_back(v4);
	cuadrado.push_back(v5);
	cuadrado.push_back(v6);
	cuadrado.push_back(v7);
	cuadrado.push_back(v8);
}

void inicializarTriangulo() {
	Vertice v1 = {
		vec3(0.0f,0.3f,0.0f),
		vec4(0.8f,0.1f,0.0f,1.0f)
	};
	Vertice v2 = {
		vec3(-0.3f,-0.3f,0.0f),
		vec4(0.8f,0.1f,0.0f,1.0f)
	};
	Vertice v3 = {
		vec3(0.3f,-0.3f,0.0f),
		vec4(0.8f,0.1f,0.0f,1.0f)
	};
	triangulo.push_back(v1);
	triangulo.push_back(v2);
	triangulo.push_back(v3);
}

void inicializarSuelo() {
	Vertice v1 = {
		vec3(-1.0f,-1.0f,0.0f),
		vec4(0.282f,0.894f,0.247f,1.0f)
	};
	Vertice v2 = {
		vec3(-1.0f,-0.6f,0.0f),
		vec4(0.282f,0.894f,0.247f,1.0f)
	};
	Vertice v3 = {
		vec3(1.0f,-0.6f,0.0f),
		vec4(0.282f,0.894f,0.247f,1.0f)
	};
	Vertice v4 = {
	vec3(1.0f,-1.0f,0.0f),
	vec4(0.917f,0.858f,0.619f,1.0f)
	};
	suelo.push_back(v1);
	suelo.push_back(v2);
	suelo.push_back(v3);
	suelo.push_back(v4);
}

void dibujar() {
	//Elegir shader
	shader->enlazar();
	//Elegir el vertex array
	glBindVertexArray(vertexArrayTrianguloID);
	//Dibujar
	glDrawArrays(GL_TRIANGLES, 0, triangulo.size());
	//Proceso dibujo de Cuadrado
	glBindVertexArray(vertexArrayCuadradoID);
	glDrawArrays(GL_QUADS, 0, cuadrado.size());

	glBindVertexArray(vertexArraySueloID);
	glDrawArrays(GL_QUADS, 0, suelo.size());
	//Soltar vertex array
	glBindVertexArray(0);
	//Desenlazar shader
	shader->desenlazar();

}

int main()
{
	//Declarar una ventana
	GLFWwindow* window;

	//Si no se pudo iniciar GLFW
	//Terminamos la ejecución
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	//Si se pudo iniciar GLFW
	//Inicializamos la ventana
	window = glfwCreateWindow(600, 600, "Ventana", NULL, NULL);

	//Si no se pudo crear la ventana
	//Terminamos la ejecución
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecemos la ventana como contexto
	glfwMakeContextCurrent(window);
	//una vez establecido el contexto
	//se activan las funciones "modernas" (gpu)
	glewExperimental = true;

	GLenum errores = glewInit();
	if (errores != GLEW_OK) {
		glewGetErrorString(errores);
	}

	const GLubyte* versionGL = glGetString(GL_VERSION);
	cout << "Version OpenGL: " << versionGL;

	inicializarTriangulo();
	inicializarCuadrado();
	inicializarSuelo();

	const char* rutaVertexShader = "VertexShader.shader";
	const char* rutaFragmentShader = "FragmentShader.shader";
	shader = new Shader(rutaVertexShader,rutaFragmentShader);
	//Mapeo de atributos
	posicionID = glGetAttribLocation(shader->getID(), "posicion");
	colorID = glGetAttribLocation(shader->getID(), "color");

	shader->desenlazar();
	//Crear el vertex array del triangulo
	glGenVertexArrays(1, &vertexArrayTrianguloID);
	glBindVertexArray(vertexArrayTrianguloID);
	//Vertex buffer
	glGenBuffers(1, &bufferTrianguloID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferTrianguloID);
	//Asociar datos al buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * triangulo.size(), triangulo.data(), GL_STATIC_DRAW);
	//Habilitar atributos de shader
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	//Especificar a OpenGL como comunicarse
	glVertexAttribPointer(posicionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)sizeof(vec3));
	//Proceso de inicializar Vertex Array para el cuadrado
	glGenVertexArrays(1, &vertexArrayCuadradoID);
	glBindVertexArray(vertexArrayCuadradoID);
	glGenBuffers(1, &bufferCuadradoID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferCuadradoID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * cuadrado.size(), cuadrado.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(posicionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)sizeof(vec3));
	//Suelo
	glGenVertexArrays(1, &vertexArraySueloID);
	glBindVertexArray(vertexArraySueloID);
	glGenBuffers(1, &bufferSueloID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferSueloID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertice) * suelo.size(), suelo.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(posicionID);
	glEnableVertexAttribArray(colorID);
	glVertexAttribPointer(posicionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertice), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertice), (void*)sizeof(vec3));
	//Soltar el vertex array y el buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Ciclo de dibujo (DrawLoop)
	while (!glfwWindowShouldClose(window)) {
		//Establecer región de dibujo
		glViewport(00, 0, 600, 600);
		//Establecemos el color de borrado
		//Valores RGBA
		glClearColor(0.674f, 0.843f, 0.945, 1);
		//Borrar
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Actualizar valores y dibujar
		dibujar();
		glfwPollEvents();
		glfwSwapBuffers(window);

	}
	//Después del ciclo de dibujo
	//Eliminamos ventana y procesos de GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

}