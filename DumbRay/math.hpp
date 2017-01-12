#pragma once

#include "math.h"

int CMID(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }

// ?????t ? [0, 1] ?????
float _interp(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }