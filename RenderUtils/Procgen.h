#pragma once
#include "Globjs.h"

//Generate a grid
Geometry GenGrid(int sqr, float dim);

//Make some noise
Texture GenNoise(unsigned sqr, unsigned octaves);