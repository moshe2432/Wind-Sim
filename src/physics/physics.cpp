#include "physics.h"
#include "../config.h"
#include "../SimpleGrid.h"


/*
* Applies boundary conditions to a grid's outer border and corners after a solver step.
* Interior edge values are mirrored onto the boundary; for b == 1 the horizontal
* component is negated across the left/right walls, and for b == 2 the vertical
* component is negated across the top/bottom walls, so that flow cannot cross a
* solid wall. Any other value of b (e.g. density, pressure) is mirrored unchanged.
* Corners are set to the average of their two adjacent edge cells.
*
* Inputs:
*   b: field type (0 = scalar field, 1 = horizontal velocity, 2 = vertical velocity).
*   x: grid whose border cells are updated in place.
*/
void set_bnd(int b, SimpleGrid& x){
    int n = x.getN();
    int m = x.getM();
    for (int j = 1; j <= m; j++) {
        x(0, j) = (b == 1) ? -x(1, j) : x(1, j);
        x(n + 1, j) = (b == 1) ? -x(n, j) : x(n, j);
    }
    for (int i = 1; i <= n; i++) {
        x(i, 0) = (b == 2) ? -x(i, 1) : x(i, 1);
        x(i, m + 1) = (b == 2) ? -x(i, m) : x(i, m);
    }
    x(0, 0) = 0.5f * (x(1, 0) + x(0, 1));
    x(0, m + 1) = 0.5f * (x(1, m + 1) + x(0, m));
    x(n + 1, 0) = 0.5f * (x(n, 0) + x(n + 1, 1));
    x(n + 1, m + 1) = 0.5f * (x(n, m + 1) + x(n + 1, m));
}

/*
* Adds a source field into a grid, scaled by the timestep. Used to inject external
* forces into the velocity field or new density from an emitter/user input.
*
* Inputs:
*   x: grid to update in place.
*   s: source values to add.
*   dt: timestep to scale the source by.
*/
void add_source(SimpleGrid& x, SimpleGrid& s, float dt){
    for (int i = 1; i <= x.getN(); i++)
        for (int j = 1; j <= x.getM(); j++)
            x(i, j) += dt * s(i, j);

}

/*
* Diffuses a grid at the given rate. Diffusion is solved implicitly (each new value
* depends on its own diffused neighbors) because the naive explicit update blows up
* for large diff*dt; the implicit system is solved approximately with 10 iterations
* of Gauss-Seidel relaxation.
*
* Inputs:
*   b: boundary condition type, forwarded to set_bnd.
*   x: grid to write the diffused result into.
*   x0: grid holding the values from before diffusion.
*   diff: diffusion rate.
*   dt: timestep.
*/
void diffusion(int b, SimpleGrid& x, SimpleGrid& x0, float diff, float dt){
    int i,j,k;
    float a = dt * diff * x.getN() * x.getM();
    for (k = 0; k < 10; k++) {
        for (i = 1; i <= x.getN(); i++) {
            for (j = 1; j <= x.getM(); j++) {
                x(i, j) = (x0(i, j) + a * (x(i-1, j) + x(i+1, j) + x(i, j-1) + x(i, j+1))) / (1 + 4 * a);
            }
        }
    }
    set_bnd(b, x);
}

/*
* Advects a grid through a velocity field using semi-Lagrangian backtracing: for each
* cell, traces backwards along (u, v) by dt to find where its value came from, then
* bilinearly interpolates that value out of the previous grid. This is unconditionally
* stable (unlike forward integration), which is what lets the simulation take
* arbitrarily large timesteps without blowing up.
*
* Inputs:
*   b: boundary condition type, forwarded to set_bnd.
*   d: grid to write the advected result into.
*   d0: grid holding the values from before advection.
*   u, v: velocity field used to trace positions backwards.
*   dt: timestep.
*/
void advect(int b, SimpleGrid& d, SimpleGrid& d0, SimpleGrid& u, SimpleGrid& v, float dt){
    int i,j;
    int x0, y0, x1, y1;//indexes of the grid cells surrounding the old particle's position
    float x, y, s0, t0, s1, t1;//weights for bilinear interpolation
    float dt0x = dt * d.getN();
    float dt0y = dt * d.getM();

    for ( i = 1; i <= d.getN(); i++)
    {
        for ( j = 1; j <= d.getM(); j++)
        {
            // Backtrace the particle's position using the velocity field
            x = i - dt0x * u(i, j);
            y = j - dt0y * v(i, j);

            // Clamp the position to be within the grid boundaries
            if (x < 0.5f) x = 0.5f;
            if (x > d.getN() + 0.5f) x = d.getN() + 0.5f;
            x0 = (int)x;
            x1 = x0 + 1;

            if (y < 0.5f) y = 0.5f;
            if (y > d.getM() + 0.5f) y = d.getM() + 0.5f;
            y0 = (int)y;
            y1 = y0 + 1;

            // Compute the weights for bilinear interpolation
            s1 = x - x0;
            s0 = 1 - s1;
            t1 = y - y0;
            t0 = 1 - t1;

            // Perform bilinear interpolation to compute the new density value at (i, j)
            d(i, j) = s0 * (t0 * d0(x0, y0) + t1 * d0(x0, y1)) +
                      s1 * (t0 * d0(x1, y0) + t1 * d0(x1, y1));
        }
    }

    set_bnd(b, d);
}

/*
* Advances the density field by one timestep: injects sources, diffuses, then advects
* it through the velocity field. SimpleGrids are pointer-swapped between stages rather than
* copied, so x0 alternates between holding the source and holding scratch state.
*
* Inputs:
*   x: density grid, updated in place.
*   x0: density source grid, also used as scratch space between stages.
*   u, v: velocity field the density is advected through.
*   diff: diffusion rate.
*   dt: timestep.
*/
void dens_step(SimpleGrid& x, SimpleGrid& x0, SimpleGrid& u, SimpleGrid& v, float diff, float dt){
    add_source(x, x0, dt);
    x.swap(x0);
    diffusion(0, x, x0, diff, dt);
    x.swap(x0);
    advect(0, x, x0, u, v, dt);
}

/*
* Advances the velocity field by one timestep: injects forces, diffuses (viscosity),
* projects out divergence, self-advects, then projects again. The velocity field is
* self-advected (advected through itself) since it carries itself along with the flow.
* project() is called both before and after advection because advect() is more
* accurate when the input velocity field is already divergence-free.
*
* Inputs:
*   u, v: velocity field, updated in place.
*   u0, v0: force source grids, also used as scratch space between stages.
*   visc: viscosity.
*   dt: timestep.
*/
void vel_step(SimpleGrid& u, SimpleGrid& v, SimpleGrid& u0, SimpleGrid& v0, float visc, float dt){
    // Add the source terms to the velocity fields
    add_source(u, u0, dt);
    add_source(v, v0, dt);

    // Perform diffusion on the velocity fields
    u.swap(u0); diffusion(1, u, u0, visc, dt);
    v.swap(v0); diffusion(2, v, v0, visc, dt);

    // Project the velocity fields to make them divergence-free
    project(u, v, u0, v0);

    u.swap(u0);
    v.swap(v0);
    // Perform advection on the velocity fields
    advect(1, u, u0, u0, v0, dt);
    advect(2, v, v0, u0, v0, dt);

    project(u, v, u0, v0);

}

/*
* Projects the velocity field onto its divergence-free component (Hodge decomposition),
* enforcing that the fluid conserves mass/incompressibility. Solves the discrete Poisson
* equation for a scalar pressure field via 20 iterations of Gauss-Seidel relaxation, then
* subtracts the pressure gradient from the velocity field to cancel its divergence.
*
* Inputs:
*   u, v: velocity field, updated in place to be divergence-free.
*   p: scratch grid used to hold the solved pressure field.
*   div: scratch grid used to hold the velocity field's divergence.
*/
void project(SimpleGrid& u, SimpleGrid& v, SimpleGrid& p, SimpleGrid& div){
    int i, j, k;
    int n = u.getN();
    int m = u.getM();
    float h = 1.0f / n;

    //calculate the divergence of the velocity field
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            div(i, j) = -0.5f * h * (u(i+1, j) - u(i-1, j) + v(i, j+1) - v(i, j-1));
            p(i, j) = 0;
        }
    }
    set_bnd(0, div);set_bnd(0, p);
    //solve for the pressure field using the Gauss-Seidel method
    for (k = 0; k < 10; k++) {
        for (i = 1; i <= n; i++) {
            for (j = 1; j <= m; j++) {
                p(i, j) = (div(i, j) + p(i-1, j) + p(i+1, j) + p(i, j-1) + p(i, j+1)) / 4;
            }
        }
        set_bnd(0, p);
    }

    //subtract the pressure gradient from the velocity field to make it divergence-free
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= m; j++) {
            u(i, j) -= 0.5f * (p(i+1, j) - p(i-1, j)) / h;
            v(i, j) -= 0.5f * (p(i, j+1) - p(i, j-1)) / h;
        }
    }
    set_bnd(1, u); set_bnd(2, v);
}

/*
* Runs one full simulation step: updates the velocity field, then advects the density
* field through it. Called once per frame from main().
*
* Inputs:
*   u, v: velocity field, updated in place.
*   u_prev, v_prev: this frame's force sources (e.g. from user input).
*   dens: density field, updated in place.
*   dens_prev: this frame's density sources (e.g. from user input).
*   visc: viscosity.
*   diff: diffusion rate.
*   dt: timestep.
*/
void UpdatePhysics(SimpleGrid& u, SimpleGrid& v, SimpleGrid& u_prev, SimpleGrid& v_prev, SimpleGrid& dens, SimpleGrid& dens_prev, float visc, float diff, float dt){
    vel_step(u, v, u_prev, v_prev, visc, dt);
    dens_step(dens, dens_prev, u, v, diff, dt);
}
