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

#define RAD(angle) ((angle) * M_PI / 180)
#define NEXT(cur, max) (((cur) + 1) % (max))

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
