#pragma once
#include "../Grid.h"
#include "raylib.h"

void map_window_pos_to_grid_pos(Vector2*);
void map_grid_pos_to_window_pos(Vector2*);
void map_grid_pos_to_cell_cords(float*,float*,Vector2*);
void map_cell_cords_to_grid_pos(float,float,Vector2*);
//void get_input(Grid& u_prev, Grid& v_prev, Grid& dens_prev, float dt);