#pragma once
#include "raylib.h"
#include "raymath.h"
#include "../Grid.h"

void DrawGrid();
void DrawVelocityArrows(Grid& u, Grid& v);
void DrawDensity(Grid& dens);