#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "Grid.h"
#include "input/input.h"

#define N 256
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRID_GAP_WIDTH 320
#define GRID_GAP_HEIGHT 40
#define MOUSE_RADIUS 15
#define MOUSE_FORCE 1.0f
#define VISC 0.0001f
#define DIFF 0.0001f
#define DENSITY_SCALE 3.0f
#define DENSITY_INJECTION_RATE 25.0f

static Grid u(N), v(N), u_prev(N), v_prev(N), dens(N),dens_prev(N);

static void DrawArrow(Vector2 start, Vector2 end, Color color);
static void DrawGrid(int cellSize, int gridGapWidth, int gridGapHeight);
static void DrawVelocityArrows(int cellSize, int gridGapWidth, int gridGapHeight);
static void DrawDensity(int cellSize, int gridGapWidth, int gridGapHeight);
static Color DensityToColor(int i, int j);
static void set_bnd(int b, Grid& x);
static void add_source(Grid& x, Grid& s, float dt);
static void diffusion(int b, Grid& x, Grid& x0, float diff, float dt);
static void advect(int b, Grid& d, Grid& d0, Grid& u, Grid& v, float dt);
static void dens_step(Grid& x, Grid& x0, Grid& u, Grid& v, float diff, float dt);
static void vel_step(Grid& u, Grid& v, Grid& u0, Grid& v0, float visc, float dt);
static void project(Grid& u, Grid& v, Grid& p, Grid& div);
static void UpdatePhysics(float dt);


/***************************Drawing Functions**********************************/
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
}

static void DrawVelocityArrows(int cellSize, int gridGapWidth, int gridGapHeight){
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

static void DrawDensity(int cellSize, int gridGapWidth, int gridGapHeight){
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            int x = gridGapWidth + j * cellSize;
            int y = gridGapHeight + i * cellSize;
            DrawRectangle(x, y, cellSize, cellSize, DensityToColor(i, j));
        }
    }
}

static Color DensityToColor(int i, int j){
    // Map density value to a color (e.g., from blue to red)
    float d = Clamp(dens(i, j) / DENSITY_SCALE, 0.0f, 1.0f);
    Color c = Fade(WHITE, d);   // WHITE with alpha scaled by density
    return c;
}


/********************************Physics Functions********************************/
static void set_bnd(int b, Grid& x){
    // This function should implement the boundary conditions for the simulation.
    for (int i = 1; i <= N; i++) {
        x(0, i) = (b == 1) ? -x(1, i) : x(1, i);
        x(N + 1, i) = (b == 1) ? -x(N, i) : x(N, i);
        x(i, 0) = (b == 2) ? -x(i, 1) : x(i, 1);
        x(i, N + 1) = (b == 2) ? -x(i, N) : x(i, N);
    }
    x(0, 0) = 0.5f * (x(1, 0) + x(0, 1));
    x(0, N + 1) = 0.5f * (x(1, N + 1) + x(0, N));
    x(N + 1, 0) = 0.5f * (x(N, 0) + x(N + 1, 1));
    x(N + 1, N + 1) = 0.5f * (x(N, N + 1) + x(N + 1, N));
}
/*
Add the source term to the grid.
for example if you have a velocity field and you want to add an external force, you can use this function to add the force to the velocity field.
*/
static void add_source(Grid& x, Grid& s, float dt){
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)    
            x(i, j) += dt * s(i, j);
    
}

static void diffusion(int b, Grid& x, Grid& x0, float diff, float dt){
    // This function should implement the diffusion step of the simulation.
    int i,j,k;
    float a = dt * diff * N * N;
    for (k = 0; k < 10; k++) {
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                // in this step we are solving the linear system of equations using the Gauss-Seidel method.
                x(i, j) = (x0(i, j) + a * (x(i-1, j) + x(i+1, j) + x(i, j-1) + x(i, j+1))) / (1 + 4 * a);
            }
        }
    }
    set_bnd(b, x);
}

static void advect(int b, Grid& d, Grid& d0, Grid& u, Grid& v, float dt){
    // This function should implement the advection step of the simulation.
    int i,j;
    int x0, y0, x1, y1;//indexes of the grid cells surrounding the old particle's position
    float x, y, s0, t0, s1, t1;//weights for bilinear interpolation
    float dt0 = dt * N;

    for ( i = 1; i <= N; i++)
    {
        for ( j = 1; j <= N; j++)
        {
            // Backtrace the particle's position using the velocity field
            x = i - dt0 * u(i, j);
            y = j - dt0 * v(i, j);

            // Clamp the position to be within the grid boundaries
            if (x < 0.5f) x = 0.5f;
            if (x > N + 0.5f) x = N + 0.5f;
            x0 = (int)x;
            x1 = x0 + 1;

            if (y < 0.5f) y = 0.5f;
            if (y > N + 0.5f) y = N + 0.5f;
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

static void dens_step(Grid& x, Grid& x0, Grid& u, Grid& v, float diff, float dt){
    // This function should implement the density update step of the simulation.

    //after each step we need to swap the grids so that the new density field becomes the old density field for the next step.
    //to save computation time we can just swap the pointers of the grids we send to the methods instead of copying the data.
    add_source(x, x0, dt);
    x.swap(x0);
    diffusion(0, x, x0, diff, dt);
    x.swap(x0);
    advect(0, x, x0, u, v, dt);
}

static void vel_step(Grid& u, Grid& v, Grid& u0, Grid& v0, float visc, float dt){
    // This function should implement the velocity update step of the simulation.

    //after each step we need to swap the grids so that the new velocity field becomes the old velocity field for the next step.
    //to save computation time we can just swap the pointers of the grids we send to the methods instead of copying the data.
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

static void project(Grid& u, Grid& v, Grid& p, Grid& div){
    // This function should implement the projection step of the simulation to enforce incompressibility.
    // For now, we will just set the pressure field to zero and the divergence field to zero.
    int i, j, k;
    float h = 1.0f / N;
    
    //calculate the divergence of the velocity field
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            div(i, j) = -0.5f * h * (u(i+1, j) - u(i-1, j) + v(i, j+1) - v(i, j-1));
            p(i, j) = 0;
        }
    }
    set_bnd(0, div);set_bnd(0, p);
    //solve for the pressure field using the Gauss-Seidel method
    for (k = 0; k < 10; k++) {
        for (i = 1; i <= N; i++) {
            for (j = 1; j <= N; j++) {
                p(i, j) = (div(i, j) + p(i-1, j) + p(i+1, j) + p(i, j-1) + p(i, j+1)) / 4;
            }
        }
        set_bnd(0, p);
    }

    //subtract the pressure gradient from the velocity field to make it divergence-free
    for (i = 1; i <= N; i++) {
        for (j = 1; j <= N; j++) {
            u(i, j) -= 0.5f * (p(i+1, j) - p(i-1, j)) / h;
            v(i, j) -= 0.5f * (p(i, j+1) - p(i, j-1)) / h;
        }
    }
    set_bnd(1, u); set_bnd(2, v);
}

static void UpdatePhysics(float dt){
    // Placeholder for physics update logic
    // This function should update the velocity fields (u, v) and density fields (dens) based on the simulation rules.
   
    vel_step(u, v, u_prev, v_prev, VISC, dt);
    dens_step(dens, dens_prev, u, v, DIFF, dt);

}


/******************************Input Functions**********************************/
static void get_input(Grid& u_prev, Grid& v_prev, Grid& dens_prev, float dt){
    // This function should handle user input to modify the velocity and density fields.
    // For example, you can use mouse input to add forces or density to the simulation.
    float x,y;
    float cellSize = (SCREEN_HEIGHT - 2 * GRID_GAP_HEIGHT) / (N + 2);
    
    // Reset the previous velocity and density fields to zero
    u_prev.reset();
    v_prev.reset();
    dens_prev.reset();
    
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Convert mouse position in the window to its position in the grid
        Vector2 mousePos = GetMousePosition();
        map_window_pos_to_grid_pos(&mousePos);
        Vector2 mouseDelta = GetMouseDelta();
        Vector2 cellPos = {0,0};
        
        
        //add density to all the grid cells that are close to the mouse position
        for (int i = 1; i <= N; i++)
        {
            for (int j = 1; j <= N; j++)
            {   
                // Calculate the position of the cell center in the grid
                //map_cell_cords_to_grid_pos(j, i, &cellPos);
                cellPos = { j * cellSize + (cellSize / 2), i * cellSize + (cellSize / 2) };
                
                if (Vector2Distance(mousePos, cellPos) < MOUSE_RADIUS) {// If the mouse is close to the cell center, add density to that cell
                    dens_prev(i, j) += DENSITY_INJECTION_RATE; // Add density to the cell
                    u_prev(i, j) += MOUSE_FORCE * mouseDelta.x / (cellSize * N * dt); // Add horizontal velocity to the cell
                    v_prev(i, j) += MOUSE_FORCE * mouseDelta.y / (cellSize * N * dt); // Add vertical velocity to the cell
                }
            }
        }
        
    }
}



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
        UpdatePhysics(GetFrameTime());
        
        
        BeginDrawing();
        // Draw the simulation here
            ClearBackground(BLACK);
            DrawText("Wind-Sim", 10, 10, 20, RAYWHITE);

            //draw the current fps in the top right corner of the window
            DrawText(TextFormat("FPS: %d", GetFPS()), screenWidth - 100, 10, 20, RAYWHITE);
            
            if (showDensity) {
                DrawDensity(cellSize, gridGapWidth, gridGapHeight);
            }else {
                DrawGrid(cellSize, gridGapWidth, gridGapHeight);
                DrawVelocityArrows(cellSize, gridGapWidth, gridGapHeight);
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}