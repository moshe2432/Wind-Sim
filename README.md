# Wind-Sim
Physics Simulation Of Wind


## About
the  purpose of this project is to learn, that mean i will try to minimize the use of AI agents. 
the goal is to understand the physics and engineering behind a real-time fluid simulation and to have a cool project to work on.
in this project i intend to build a physics based wind simulation and a way to present that simulation on screen.

## Goal 
### the must have for this project are:
1. Physics engine - fluid physics engine simulate airflow around objects.
2. Visualization — render the flow on screen in a way that's easy to interpret.

### future ideas for the project:
1. Wind tunnel simulation - where the user can upload a 3D model to the tunnel and see how the wind flows when it comes into contact with it.
2. Wind over terrain - create randomly a unique terrain wind direction and simulate how the wind pass over it.
3. Paragliding Game - need #2 first.

the physics part of the project will be written in C++.
the rendering/visualization part of the project TBD.

## Status: 
### early planning
gathering resources to plan and design. searching for similar project on Github/YouTube that will help me in the design.

## Milestones
### 1 — 2D fluid core (CPU, raylib)
Port Stam's 2D solver (add_source, diffuse, advect, project) to C++. Visualize the density grid as a texture in raylib. Goal: see smoke swirl on screen, driven by mouse-added forces/density like in the paper's prototype.
### 2 — Internal boundaries (objects in 2D flow)
Add a boolean occupancy grid so a solid shape (circle, square) sits inside the fluid and the flow visibly goes around it. This is your first real "airflow around an object" result, even in 2D.
### 3 — Fixed-velocity inflow (2D wind tunnel)
Add a boundary condition with constant inflow velocity on one side, matching Stam's wind-tunnel suggestion. Now you have a genuine 2D wind tunnel: object + directional flow.
### 4 — Extend solver to 3D
Add the z-component arrays and third loop dimension to diffuse/advect/project. Same raylib window, but now simulating a 3D grid (can still visualize a 2D slice at first to sanity-check).
### 5 — Basic 3D visualization
Move to raylib's 3D mode: camera, basic volumetric or slice-based rendering of the density/velocity field, so you can orbit around the simulated volume.
### 6 — 3D object + inflow (real wind tunnel)
Combine milestones 2+3 in 3D: a solid object sitting in a 3D domain with directional inflow. This is your "must-have" goal achieved.
### 7 — 3D model upload
Load a simple 3D mesh (start with a basic format like .obj) and voxelize it into the occupancy grid, replacing the hardcoded test shapes from Milestone 6.
### 8 — Renderer upgrade: raylib → raw OpenGL
Once the physics core is solid, rebuild the rendering layer in GLFW + OpenGL for full control, particularly to support proper volumetric rendering (ray-marching) of the flow field.
### 9 — GPU port of the solver
Port the CPU Gauss-Seidel solver to a compute shader, using the CPU version as a correctness reference. This is where performance for larger 3D grids comes from.
### 10 - ???
