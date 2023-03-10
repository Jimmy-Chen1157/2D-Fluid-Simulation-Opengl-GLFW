#pragma once

#include <cmath>

#define IX(x, y) ((x) + (y) * N)

extern const int N;
extern const int iter;

class Fluid
{
public:
    int size;
    float dt;
    float diff;
    float visc;

    float* s;
    float* density;

    float* Vx;
    float* Vy;

    float* Vx0;
    float* Vy0;

public:
    Fluid(float diffusion, float viscosity, float dt);
    ~Fluid();
    void FluidCubeStep();
    void Fade();

    void FluidCubeAddDensity(int x, int y, float amount);
    void FluidCubeAddVelocity(int x, int y, float amountX, float amountY);
    
    void set_bnd(int b, float* x);
    void lin_solve(int b, float* x, float* x0, float a, float c);
    void diffuse(int b, float* x, float* x0, float diff, float dt);
    void project(float* velocX, float* velocY, float* p, float* div);
    void advect(int b, float* d, float* d0, float* velocX, float* velocY, float dt);
};