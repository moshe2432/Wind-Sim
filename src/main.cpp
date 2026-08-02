#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "SimpleGrid.h"
#include "input/input.h"
#include "rendering/rendering.h"
#include "config.h"
#include "physics/physics.h"


static SimpleGrid u(N,N), v(N,N), u_prev(N,N), v_prev(N,N), dens(N,N),dens_prev(N,N);


int main(){
    const int screenWidth = SCREEN_WIDTH;
    const int screenHeight = SCREEN_HEIGHT;
    const int gridGapWidth = GRID_GAP_WIDTH;
    const int gridGapHeight = GRID_GAP_HEIGHT;
    const int cellSize = (screenHeight - 2 * gridGapHeight) / (N + 2);
    const int gridPixelSize = cellSize * (N + 2);

    bool showDensity = true;

    InitWindow(screenWidth, screenHeight, "Wind-Sim");
    SetTargetFPS(60);



    while (!WindowShouldClose()) {

        // Get user input and update the simulation state
        get_input(u_prev, v_prev, dens_prev, GetFrameTime());

        if (IsKeyPressed(KEY_SPACE)) {
            showDensity = !showDensity;
        }

        // Update the Physics simulation here
        UpdatePhysics(u, v, u_prev, v_prev, dens, dens_prev, VISC, DIFF, GetFrameTime());


        BeginDrawing();
        // Draw the simulation here
            ClearBackground(BLACK);
            DrawText("Wind-Sim", 10, 10, 20, RAYWHITE);

            //draw the current fps in the top right corner of the window
            DrawText(TextFormat("FPS: %d", GetFPS()), screenWidth - 100, 10, 20, RAYWHITE);

            if (showDensity) {
                DrawDensity(dens);
            }else {
                DrawGrid();
                DrawVelocityArrows(u, v);
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
