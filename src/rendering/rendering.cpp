#include "rendering.h"
#include "raylib.h"
#include "raymath.h"
#include "../Grid.h"
#include "../config.h"

static Color DensityToColor(int, int, Grid&);
static void DrawArrow(Vector2, Vector2, Color);

/*
*Draw an arrow from the start point to the end point with the specified color.
*Inputs:
*   start: The starting point of the arrow (Vector2).
*   end: The ending point of the arrow (Vector2).
*   color: The color of the arrow (Color).
*/
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
*Draw the grid lines on the screen based on the cell size and grid gap dimensions.
*/
void DrawGrid(){
    for (int i = 0; i < N+3; i++)
    {
        DrawLine(i * CELL_SIZE + GRID_GAP_WIDTH, GRID_GAP_HEIGHT, i*CELL_SIZE + GRID_GAP_WIDTH, GRID_GAP_HEIGHT + CELL_SIZE * (N + 2), GRAY);
        DrawLine(GRID_GAP_WIDTH, i*CELL_SIZE + GRID_GAP_HEIGHT, GRID_GAP_WIDTH + CELL_SIZE * (N + 2), i*CELL_SIZE + GRID_GAP_HEIGHT, GRAY);
    }      
}


void DrawVelocityArrows(Grid& u, Grid& v){
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            float u_val = u(i, j);
            float v_val = v(i, j);
            Vector2 start = { GRID_GAP_WIDTH + j * CELL_SIZE + CELL_SIZE / 2.0f, GRID_GAP_HEIGHT + i * CELL_SIZE + CELL_SIZE / 2.0f };
            Vector2 end = { start.x + u_val * 10.0f, start.y + v_val * 10.0f };
            DrawArrow(start, end, RED);
        }
    }
}

void DrawDensity(Grid& dens){
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            int x = GRID_GAP_WIDTH + j * CELL_SIZE;
            int y = GRID_GAP_HEIGHT + i * CELL_SIZE;
            DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, DensityToColor(i, j, dens));
        }
    }
}

static Color DensityToColor(int i, int j, Grid& dens){
    // Map density value to a color (e.g., from blue to red)
    float d = Clamp(dens(i, j) / DENSITY_SCALE, 0.0f, 1.0f);
    Color c = Fade(WHITE, d);   // WHITE with alpha scaled by density
    return c;
}
