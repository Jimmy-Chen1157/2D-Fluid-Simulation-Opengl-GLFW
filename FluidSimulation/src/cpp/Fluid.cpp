#include "Fluid.h"

Fluid::Fluid(float diffusion, float viscosity, float _dt)
{
    size = N;
    dt = _dt;
    diff = diffusion;
    visc = viscosity;

    s = (float*)calloc(N*N, sizeof(float));
    density = (float*)calloc(N*N, sizeof(float));

    Vx = (float*)calloc(N*N, sizeof(float));
    Vy = (float*)calloc(N*N, sizeof(float));

    Vx0 = (float*)calloc(N*N, sizeof(float));
    Vy0 = (float*)calloc(N*N, sizeof(float));
}


Fluid::~Fluid()
{
    delete[] s;
    delete[] density;
    delete[] Vx;
    delete[] Vy;
    delete[] Vx0;
    delete[] Vy0;
}

void Fluid::Fade()
{
    for (int i = 0; i < N*N; i++)
    {
        float d = density[i];
        if (d - 0.0015>= 0) { density[i] = d - 0.0015; }
    }
}


void Fluid::FluidCubeAddDensity(int x, int y, float amount)
{
    density[IX(x, y)] += amount;
}

void Fluid::FluidCubeAddVelocity(int x, int y, float amountX, float amountY)
{
    int index = IX(x, y);

    Vx[index] += amountX;
    Vy[index] += amountY;
}

void Fluid::FluidCubeStep()
{
    float visc = this->visc;
    float diff = this->diff;
    float dt = this->dt;
    float* Vx = this->Vx;
    float* Vy = this->Vy;
    float* Vx0 = this->Vx0;
    float* Vy0 = this->Vy0;
    float* s = this->s;
    float* density = this->density;

    diffuse(1, Vx0, Vx, visc, dt);
    diffuse(2, Vy0, Vy, visc, dt);

    project(Vx0, Vy0, Vx, Vy);

    advect(1, Vx, Vx0, Vx0, Vy0, dt);
    advect(2, Vy, Vy0, Vx0, Vy0, dt);

    project(Vx, Vy, Vx0, Vy0);

    diffuse(0, s, density, diff, dt);
    advect(0, density, s, Vx, Vy, dt);

    Fade();
}

void Fluid::set_bnd(int b, float* x)
{
    for (int i = 1; i < N - 1; i++) {
        x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
        x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
    }

    for (int j = 1; j < N - 1; j++) {
        x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
        x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
    }


    x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N - 1)] = 0.5f * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
    x[IX(N - 1, 0)] = 0.5f * (x[IX(N - 2, 0)] + x[IX(N - 1, 1)]);
    x[IX(N - 1, N - 1)] = 0.5f * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);
}

void Fluid::lin_solve(int b, float* x, float* x0, float a, float c)
{
    double cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) {
        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                x[IX(i, j)] =
                    (x0[IX(i, j)]
                        + a * (x[IX(i + 1, j)]
                            + x[IX(i - 1, j)]
                            + x[IX(i, j + 1)]
                            + x[IX(i, j - 1)]
                            )) * cRecip;
            }
        }

        set_bnd(b, x);
    }
}

void Fluid::diffuse(int b, float* x, float* x0, float diff, float dt)
{
    float a = dt * diff * (N - 2) * (N - 2);
    lin_solve(b, x, x0, a, 1 + 6 * a);
}

void Fluid::project(float* velocX, float* velocY, float* p, float* div)
{
    for (int j = 1; j < N - 1; j++) {
        for (int i = 1; i < N - 1; i++) {
            div[IX(i, j)] = -0.5f * (
                velocX[IX(i + 1, j)]
                - velocX[IX(i - 1, j)]
                + velocY[IX(i, j + 1)]
                - velocY[IX(i, j - 1)]
                ) / N;
            p[IX(i, j)] = 0;
        }
    }
 
    set_bnd(0, div);
    set_bnd(0, p);
    lin_solve(0, p, div, 1, 6);

    for (int j = 1; j < N - 1; j++) {
        for (int i = 1; i < N - 1; i++) {
            velocX[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)]
                - p[IX(i - 1, j)]) * N;
            velocY[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)]
                - p[IX(i, j - 1)]) * N;
        }
    }
   
    set_bnd(1, velocX);
    set_bnd(2, velocY);
}

void Fluid::advect(int b, float* d, float* d0, float* velocX, float* velocY, float dt)
{
    float i0, i1, j0, j1;

    float dtx = dt * (N - 2);
    float dty = dt * (N - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float Nfloat = N;
    float ifloat, jfloat;
    int i, j;

    for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
        for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
            tmp1 = dtx * velocX[IX(i, j)];
            tmp2 = dty * velocY[IX(i, j)];
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if (x < 0.5f) x = 0.5f;
            if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
            i0 = floorf(x);
            i1 = i0 + 1.0f;
            if (y < 0.5f) y = 0.5f;
            if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
            j0 = floorf(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;


            int i0i = i0;
            int i1i = i1;
            int j0i = j0;
            int j1i = j1;

            d[IX(i, j)] =
                s0 * (t0 * d0[IX(i0i, j0i)]
                    + t1 * d0[IX(i0i, j1i)])
                + s1 * (t0 * d0[IX(i1i, j0i)]
                    + t1 * d0[IX(i1i, j1i)]);
        }
    }

    set_bnd(b, d);
}
