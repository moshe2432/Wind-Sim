#pragma once

#include "../Grid.h"


// Mirrors interior edge values onto the grid's border/corners to enforce solid-wall boundaries.
void set_bnd(int b, Grid& x);
// Adds a source field into a grid, scaled by dt.
void add_source(Grid& x, Grid& s, float dt);
// Diffuses a grid at the given rate via Gauss-Seidel relaxation.
void diffusion(int b, Grid& x, Grid& x0, float diff, float dt);
// Advects a grid through a velocity field via semi-Lagrangian backtracing.
void advect(int b, Grid& d, Grid& d0, Grid& u, Grid& v, float dt);
// Advances the density field by one timestep (source, diffuse, advect).
void dens_step(Grid& x, Grid& x0, Grid& u, Grid& v, float diff, float dt);
// Advances the velocity field by one timestep (source, diffuse, project, advect, project).
void vel_step(Grid& u, Grid& v, Grid& u0, Grid& v0, float visc, float dt);
// Projects the velocity field onto its divergence-free component (Hodge decomposition).
void project(Grid& u, Grid& v, Grid& p, Grid& div);
// Runs one full simulation step: velocity update followed by density advection.
void UpdatePhysics(Grid& u, Grid& v, Grid& u_prev, Grid& v_prev, Grid& dens, Grid& dens_prev, float visc, float diff, float dt);
