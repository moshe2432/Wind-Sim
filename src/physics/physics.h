#pragma once

#include "../SimpleGrid.h"


// Mirrors interior edge values onto the grid's border/corners to enforce solid-wall boundaries.
void set_bnd(int b, SimpleGrid& x);
// Adds a source field into a grid, scaled by dt.
void add_source(SimpleGrid& x, SimpleGrid& s, float dt);
// Diffuses a grid at the given rate via Gauss-Seidel relaxation.
void diffusion(int b, SimpleGrid& x, SimpleGrid& x0, float diff, float dt);
// Advects a grid through a velocity field via semi-Lagrangian backtracing.
void advect(int b, SimpleGrid& d, SimpleGrid& d0, SimpleGrid& u, SimpleGrid& v, float dt);
// Advances the density field by one timestep (source, diffuse, advect).
void dens_step(SimpleGrid& x, SimpleGrid& x0, SimpleGrid& u, SimpleGrid& v, float diff, float dt);
// Advances the velocity field by one timestep (source, diffuse, project, advect, project).
void vel_step(SimpleGrid& u, SimpleGrid& v, SimpleGrid& u0, SimpleGrid& v0, float visc, float dt);
// Projects the velocity field onto its divergence-free component (Hodge decomposition).
void project(SimpleGrid& u, SimpleGrid& v, SimpleGrid& p, SimpleGrid& div);
// Runs one full simulation step: velocity update followed by density advection.
void UpdatePhysics(SimpleGrid& u, SimpleGrid& v, SimpleGrid& u_prev, SimpleGrid& v_prev, SimpleGrid& dens, SimpleGrid& dens_prev, float visc, float diff, float dt);
