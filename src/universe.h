#ifndef RAYA_UNIVERSE_H
#define RAYA_UNIVERSE_H

#include "../include/raylib.h"
#include "./raylib.h"

#include "../include/raymath.h"
#include "../include/rlgl.h"
#include <stdbool.h>
#include <stddef.h>

// core data
typedef struct {
  Vector2 pos;
  Vector2 vel;
  int charge;
  int rot;
  float size;
  int selected;
} atom;

typedef struct {
  int count;
  int charge;
  bool typing;
} multiSpawningInfo;

// GLOBAL SETTINGS !!!
extern int mouseWheelSensivity;
// arrow moving
extern float movementSpeed;
extern float rotationSpeed;
// spawningUI SETTINGS
#define MAXINPUTCHARS 50
extern float selectionDuration;
// rendering SETTINGS

// KEYBINDS !!!
extern int AddKey;
extern int selectKey;
extern int deleteKey;

extern int camMovementUp;
extern int camMovementDown;
extern int camMovementLeft;
extern int camMovementRight;

extern int camZoomOut;
extern int camZoomIn;

extern int pauseKey;

// physics SETTINGS
extern int newtonianConst;

// info texts SETTINGS
extern int paddingTop;

extern atom *atomsList;
extern size_t atomCount;

extern bool isPaused;
extern float scrollActionSpeed;
extern Camera2D camera;

extern float halfScreenWidth;
extern float halfScreenHeight;

extern Camera2D camera;

// SHADERSSSSSSSSSSs
extern Shader shaderCompute;
extern int dtLoc;
extern int countLoc;
extern unsigned int ssbo;

// physics
void atomPhysics();
void initCamera();
void cameraControls();
void updateSSBO();
void initComputeShader();

#endif
