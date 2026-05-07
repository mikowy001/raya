#ifndef RAYA_COREDATA_H
#define RAYA_COREDATA_H

#include "./raylib.h"

#include <stddef.h>

#define MAXINPUTCHARS 50

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

typedef struct {
	Camera2D camera;

	struct {
		atom *atomsList;
		size_t atomsCount;
	} Atoms;

	struct {
		int addKey;
		int selectKey;
		int deleteKey;

		int camMovementUp;
		int camMovementDown;
		int camMovementLeft;
		int camMovementRight;

		int camZoomOut;
		int camZoomIn;

		int pauseKey;
	} Keybinds;

	struct {
		bool isPaused;

		float halfScreenWidth;
		float halfScreenHeight;
	} Info;

	struct {
		int newtonianConst;
		int paddingTop;

		float scrollActionSpeed;

		int mouseWheelSensivity;
		float movementSpeed;
		float rotationSpeed;

		float selectionDuration;
	} Settings;

	struct {
		Shader shaderCompute;
		int dtLoc;
		int countLoc;
		unsigned int ssbo;
	} Shaders;
} CoreData;

extern CoreData DATA;

#endif
