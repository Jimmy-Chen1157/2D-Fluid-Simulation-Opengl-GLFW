#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "Fluid.h"
#include "Grid.h"

class Sim
{
private:
	GLFWwindow* window;
	Grid* grid;
	Fluid* fluid;
	Shader* shader;

public:
	Sim();
	~Sim();
	void processInput();
	void Init();
	void Run();
};