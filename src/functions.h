#ifndef RAYA_FUNCTIONS_H
#define RAYA_FUNCTIONS_H

#include "../include/raylib.h"
#include "./raylib.h"

#include "../include/raymath.h"
#include "../include/rlgl.h"
#include <stdbool.h>
#include <stddef.h>

void atomPhysics();
void initCamera();
void cameraControls();
void updateSSBO();
void initComputeShader();

#endif
