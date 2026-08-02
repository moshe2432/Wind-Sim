#pragma once
#include "raylib.h"
#include "raymath.h"
#include "../SimpleGrid.h"

void DrawGrid();
void DrawVelocityArrows(SimpleGrid& u, SimpleGrid& v);
void DrawDensity(SimpleGrid& dens);