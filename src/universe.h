#include <raylib.h>
#include <stdbool.h>

//core data
typedef struct {
	Vector2 pos;
	Vector2 vel;
	int charge;
    int rot;
	float size;
	bool selected;
} atom;

typedef struct {
	int count;
	int charge;
	bool typing;
} multiSpawningInfo;

//GLOBAL SETTINGS !!!
int mouseWheelSensivity = 10;
//arrow moving
float movementSpeed = 10;
float rotationSpeed = 2.5;
//spawningUI SETTINGS
#define MAXINPUTCHARS  50
const float selectionDuration = 10.0f;
//rendering SETTINGS

//KEYBINDS !!!
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

//physics SETTINGS
int newtonianConst = 25000;

//info texts SETTINGS
int paddingTop = 35;


atom* atomsList = NULL;
size_t atomCount = 0;

bool isPaused = false;
float scrollActionSpeed;
Camera2D camera = { 0 };

float halfScreenWidth;
float halfScreenHeight;



//physics
void atomPhysics();
