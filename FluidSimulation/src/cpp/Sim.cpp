#include "Sim.h"

const int N = 120;
const int iter = 4;

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;


using std::cout;
using std::endl;


Sim::Sim() :
    fluid(nullptr), grid(nullptr), shader(nullptr), window(nullptr)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2D Fluid Simulation", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return;
    }
}

void Sim::Init()
{
    glm::vec3 vertices[] = {
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // Store mesh data in vectors for the mesh
    std::vector <glm::vec3> verts(vertices, vertices + sizeof(vertices) / sizeof(glm::vec3));
    std::vector <unsigned int> ind(indices, indices + sizeof(indices) / sizeof(unsigned int));
    
    // Generates Shader object using shaders Grid.vert and Grid.frag
    shader = new Shader("res/shaders/GridVertex.vert", "res/shaders/GridFrag.vert");
    shader->Bind();
    glm::mat4 proj = glm::ortho(0.0f, (float)N, 0.0f, (float)N, -1.0f, 1.0f);
    shader->SetUniformMatrix4fv("projection", proj);

    // Create Grid 
    grid = new Grid(verts, ind);
    // Create Fluid
    fluid = new Fluid(0.00005f, 0.00008f, 0.001f);
}

void Sim::Run()
{
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid->AddQuad(glm::vec2(i, j), glm::vec4(0, 0, 0, 1.0f));
        }
    }
    grid->Update();

    while (!glfwWindowShouldClose(window))
    {

        processInput();

        float angle = rand();
        float Vx = 120 * cos(angle);
        float Vy = 120 * sin(angle);


        fluid->FluidCubeAddDensity(N / 2, N / 2, 1.0f);
        fluid->FluidCubeAddVelocity(N / 2, N / 2, Vx, Vy);
        

        fluid->FluidCubeStep();

        glClear(GL_COLOR_BUFFER_BIT);

  
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                float d = fluid->density[IX(i, j)];
                grid->AddQuad(glm::vec2(i, j), glm::vec4(d, d, d, 1.0f));
            }
        }
        grid->Update();
        grid->Draw(*shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();

}

Sim::~Sim()
{
    delete grid;
    delete fluid;
    delete shader;
}

void Sim::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}