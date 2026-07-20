#include "raylib.h"
#include "Grid.cpp"

#define N 64

static Grid u(N), v(N), u_prev(N), dens(N),dens_prev(N);


int main(){
    const int screenWidth = 800;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Wind-Sim");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Update the Physics simulation here
        
        BeginDrawing();
        // Draw the simulation here
            ClearBackground(BLACK);
            DrawText("Wind-Sim", 10, 10, 20, RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}