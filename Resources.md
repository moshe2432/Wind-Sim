# Resources
 
Research and reference material used while designing and building Wind-Sim. Kept separate from the README so it can grow without cluttering the project overview.
 
 
## Fluid Physics / Simulation
 
- [Build your own X](https://github.com/codecrafters-io/build-your-own-x#build-your-own-3d-renderer) — a huge list of "build it from scratch" tutorials for all kinds of things, including renderers; good general reference.
- [Coding Adventure: Simulating Fluids](https://www.youtube.com/watch?v=rSKMYc1CQHE&list=PLFt_AvWsXl0ehjAfLFsp1PGaatzAwo0uK&index=19) — YouTube series with several videos on fluid sim from scratch, good visual/intuitive companion to the papers below.
- [Grid Fluids seminar notes (Uni Freiburg)](https://cg.informatik.uni-freiburg.de/intern/seminar/gridFluids_fluid-EulerParticle.pdf) — grid-based (Eulerian) vs. particle-based fluid overview.
- [IEEE paper](https://ieeexplore.ieee.org/abstract/document/10885003) — (add note once read).
- [Google Books reference](https://books.google.co.il/books?id=1-LqBgAAQBAJ&printsec=frontcover&redir_esc=y#v=onepage&q&f=false) — (add note once read).
- [Real-Time Fluid Dynamics for Games (Stam)](https://graphics.cs.cmu.edu/nsp/course/15-464/Fall09/papers/StamFluidforGames.pdf) — **great starting point for v1.** Full working C code for a 2D Navier-Stokes solver (Eulerian grid, Gauss-Seidel diffusion, semi-Lagrangian advection, incompressibility projection). Directly mentions internal boundaries (objects in the flow) and fixed-velocity inflow boundaries (wind tunnel) as extensions.
## Rendering / Visualization
 
- [raylib](https://www.raylib.com/) — simple C library for windowing, 2D drawing, and basic 3D (camera, meshes, lighting). Good fit for a from-scratch physics project since it stays out of the way and doesn't pull in an engine. Plan: use for the 2D Stam-solver prototype first (draw the density grid as a texture), then reuse for early 3D visualization once the solver is extended to 3D.
- [SFML](https://www.sfml-dev.org/) — alternative to raylib, more C++-idiomatic API (`sf::Texture`, `sf::Sprite`). Similar tier, pick one.
- [GLFW](https://www.glfw.org/) + raw OpenGL — lower-level option if I want to learn the rendering side properly (shaders, VAOs/VBOs) instead of using raylib's built-ins. More boilerplate before anything shows on screen.
- Note for later: visualizing 3D volumetric density (smoke/wind field, not just solid meshes) needs ray-marching or slice rendering in a shader — a separate skill from normal mesh rendering. Not needed for v1, but will matter once the wind tunnel needs to show flow, not just objects.
## 3D Model Handling (upload, parsing, collision)
 
-
## Terrain Generation
 
- [Wind field simulation (Mann, 1998)](https://www.sciencedirect.com/science/article/pii/S0266892097000362) — advanced paper on generating realistic turbulent wind fields statistically (spectral tensor + FFT). Not for airflow-around-objects — good fit for the "wind over terrain" future idea, not the core physics engine.
## General / Similar Projects
 
- [Physics-based fluid simulation in computer graphics: Survey (2024)](https://doi.org/10.1007/s41095-023-0368-y) — broad survey of ~300 CG fluid sim papers (SPH, Eulerian, hybrid, fluid-solid coupling, multiphase, etc.). Useful as a map of the field before committing to a technique; not a tutorial. Appendix lists open-source libraries (SPlisHSPlasH, MantaFlow) worth looking at as reference implementations.
<!--
Format suggestion:
- [Title](url) — one-line note on what this covers or why it's relevant
-->
