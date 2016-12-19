// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// TODO: reference additional headers your program requires here
#include <stdlib.h>
#include <windows.h>
#include "SDL.h"
#include <math.h>

#include <string>
#include <assert.h>
#include <vector>
#include <algorithm>

#define RAD(angle) ((angle) * M_PI / 180)
#define NEXT(cur, max) (((cur) + 1) % (max))
#define PREV(cur, max) ((cur) == 0 ? (max) - 1 : (cur) - 1)

// Screen dimension constants
const int	SCREEN_WIDTH = 1024;
const int	SCREEN_HEIGHT = 640;
// Radius for the shapes to draw in 2D OR actual side length of shapes in 3D
const int	SIDE_LENGTH = 75;
// Used to compensate for possible mistakes in floating point types accuracy
const float	ALMOST_ZERO = 0.000001;
