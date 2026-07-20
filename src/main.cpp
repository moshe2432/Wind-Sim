#include "raylib.h"
#include <cmath>
#include "Grid.h"

#define N 16

static Grid u(N), v(N), u_prev(N), dens(N),dens_prev(N);


static void DrawArrow(Vector2 start, Vector2 end, Color color){
    DrawLineEx(start, end, 2.0f, color);

    float angle = atan2f(end.y - start.y, end.x - start.x);
    float headLen = 6.0f;
    float headAngle = 25.0f * DEG2RAD;

    Vector2 left  = { end.x - headLen * cosf(angle - headAngle), end.y - headLen * sinf(angle - headAngle) };
    Vector2 right = { end.x - headLen * cosf(angle + headAngle), end.y - headLen * sinf(angle + headAngle) };

    DrawLineEx(end, left, 2.0f, color);
    DrawLineEx(end, right, 2.0f, color);
}


/*
Draw the grid lines and the arrows representing the velocity field (u, v) on the grid.
*/
static void DrawGrid(int cellSize, int gridGapWidth, int gridGapHeight){
    for (int i = 0; i < N+3; i++)
    {
        DrawLine(i*cellSize + gridGapWidth, gridGapHeight, i*cellSize + gridGapWidth, gridGapHeight + cellSize * (N + 2), GRAY);
        DrawLine(gridGapWidth, i*cellSize + gridGapHeight, gridGapWidth + cellSize * (N + 2), i*cellSize + gridGapHeight, GRAY);
    }


            for (int i = 1; i <= N; i++)
            {
                for (int j = 1; j <= N; j++)
                {
                    float u_val = u(i, j);
                    float v_val = v(i, j);
                    Vector2 start = { gridGapWidth + j * cellSize + cellSize / 2.0f, gridGapHeight + i * cellSize + cellSize / 2.0f };
                    Vector2 end = { start.x + u_val * 10.0f, start.y + v_val * 10.0f };
                    DrawArrow(start, end, RED);
                }
            }
            
}



static void UpdatePhysics(float dt){
    // Placeholder for physics update logic
    // This function should update the velocity fields (u, v) and density fields (dens) based on the simulation rules.
    // For now, we will just add some random perturbations to the velocity fields to visualize movement.
}

int main(){
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int gridGapWidth = 320;
    const int gridGapHeight = 40;
    const int cellSize = (screenHeight - 2 * gridGapHeight) / (N + 2);
    const int gridPixelSize = cellSize * (N + 2);

    //start with random values for u and v
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            u(i, j) = (float)rand() / RAND_MAX * 2.0f - 1.0f;
            v(i, j) = (float)rand() / RAND_MAX * 2.0f - 1.0f;
        }
    }

    InitWindow(screenWidth, screenHeight, "Wind-Sim");
    SetTargetFPS(60);

    

    while (!WindowShouldClose()) {
        // Update the Physics simulation here

        BeginDrawing();
        // Draw the simulation here
            ClearBackground(BLACK);
            DrawText("Wind-Sim", 10, 10, 20, RAYWHITE);
            DrawGrid(cellSize, gridGapWidth, gridGapHeight);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}