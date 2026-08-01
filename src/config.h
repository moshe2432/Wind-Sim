#pragma once

#define N 256                   // Horazantel number of cells
#define M 256                   // Vertical munber of cells
#define SCREEN_WIDTH 1280       // Width of the window in pixels
#define SCREEN_HEIGHT 720       // Height of the window in pixels
#define GRID_GAP_WIDTH 320      // Width of the gap between the grid and the window edges in pixels
#define GRID_GAP_HEIGHT 40      // Height of the gap between the grid and the window edges in pixels
#define CELL_SIZE ((SCREEN_HEIGHT - 2 * GRID_GAP_HEIGHT) / (N + 2)) // Size of each cell in the grid in pixels

#define VISC 0.0001f            // Kinematic viscosity of the velocity field
#define DIFF 0.0001f            // Diffusion rate of the density field

#define MOUSE_RADIUS 15         // Radius around the mouse position to affect the grid cells in pixels
#define MOUSE_FORCE 5.0f        // Force applied to the velocity field when the mouse is dragged

#define DENSITY_SCALE 3.0f      // Scale factor for the density field to control the visual representation of density
#define DENSITY_INJECTION_RATE 25.0f // Rate at which density is injected into the simulation
