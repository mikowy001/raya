#include "./universe.h"


int mouseWheelSensivity = 10;
float movementSpeed = 10;
float rotationSpeed = 2.5;
float selectionDuration = 10.0f;
int AddKey = KEY_G;
int selectKey = KEY_F;
int deleteKey = KEY_R;

int camMovementUp = KEY_W;
int camMovementDown = KEY_S;
int camMovementLeft = KEY_A;
int camMovementRight = KEY_D;

int camZoomOut = KEY_Q;
int camZoomIn = KEY_E;

int pauseKey = KEY_P;

int newtonianConst = 10000;

int paddingTop = 35;

atom* atomsList = NULL;
size_t atomCount = 0;

bool isPaused = false;
float scrollActionSpeed;
Camera2D camera = { 0 };

float halfScreenWidth;
float halfScreenHeight;


