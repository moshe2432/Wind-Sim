
#include "input.h"
#include "../config.h" 
#include "raylib.h"
#include "raymath.h"
#include <cmath>
#include "../SimpleGrid.h"

/*
* This function maps the window position to the grid position. It takes in the x and y coordinates of the window position and maps them to the corresponding grid position. The implementation of this function is currently empty and needs to be filled in with the appropriate logic to perform the mapping.
*
* Input:
* - x: The x coordinate of the window position.
* - y: The y coordinate of the window position.
*
* Output:
* - x: The mapped x coordinate of the grid position.
* - y: The mapped y coordinate of the grid position.
*/
void map_window_pos_to_grid_pos(Vector2* pos){
    pos->x = pos->x - GRID_GAP_WIDTH;
    pos->y = pos->y - GRID_GAP_HEIGHT;
}


/*
*This function maps the grid position to the window position. It takes in the x and y coordinates of the grid position and maps them to the corresponding window position. The implementation of this function is currently empty and needs to be filled in with the appropriate logic to perform the mapping.
*
* Input:
* - x: The x coordinate of the grid position.
* - y: The y coordinate of the grid position.
*
* Output:
* - x: The mapped x coordinate of the window position.
* - y: The mapped y coordinate of the window position.
*/
void map_grid_pos_to_window_pos(Vector2* pos){
    pos->x = pos->x + GRID_GAP_WIDTH;
    pos->y = pos->y + GRID_GAP_HEIGHT;
}

/*
* This function maps the grid position to the cell coordinates. It takes in the x and y coordinates of the grid position and maps them to the corresponding cell coordinates.
* Input:
* - x: The x coordinate of the grid position.
* - y: The y coordinate of the grid position.
* Output:
* - x: The mapped x coordinate of the cell coordinates.
* - y: The mapped y coordinate of the cell coordinates.
*/
void map_grid_pos_to_cell_cords(float* x,float* y,Vector2* pos){
    *x = pos->x / CELL_SIZE;
    *y = pos->y / CELL_SIZE;
}

/*
*/
void map_cell_cords_to_grid_pos(float x,float y,Vector2* pos){
    pos->x = x * CELL_SIZE + (CELL_SIZE/2);
    pos->y = y * CELL_SIZE + (CELL_SIZE/2);
}



void get_input(SimpleGrid& u_prev, SimpleGrid& v_prev, SimpleGrid& dens_prev, float dt){
    // This function should handle user input to modify the velocity and density fields.
    // For example, you can use mouse input to add forces or density to the simulation.
    float x,y;
    
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
                map_cell_cords_to_grid_pos(j, i, &cellPos);
                
                
                if (Vector2Distance(mousePos, cellPos) < MOUSE_RADIUS) {// If the mouse is close to the cell center, add density to that cell
                    dens_prev(i, j) += DENSITY_INJECTION_RATE; // Add density to the cell
                    //u_prev(i, j) += MOUSE_FORCE * mouseDelta.x / (CELL_SIZE * N * dt); // Add horizontal velocity to the cell
                    //v_prev(i, j) += MOUSE_FORCE * mouseDelta.y / (CELL_SIZE * N * dt); // Add vertical velocity to the cell
                }
            }
        }
        
    }else if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
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
                map_cell_cords_to_grid_pos(j, i, &cellPos);
                
                
                if (Vector2Distance(mousePos, cellPos) < MOUSE_RADIUS) {// If the mouse is close to the cell center, add density to that cell
                    //dens_prev(i, j) += DENSITY_INJECTION_RATE; // Add density to the cell
                    u_prev(i, j) -= MOUSE_FORCE * mouseDelta.x / (CELL_SIZE * N * dt); // Add horizontal velocity to the cell
                    v_prev(i, j) -= MOUSE_FORCE * mouseDelta.y / (CELL_SIZE * N * dt); // Add vertical velocity to the cell
                }
            }
        }
    }
                
}