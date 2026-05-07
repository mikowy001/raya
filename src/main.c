#include <math.h>

#include "./functions.h"
#include "./coredata.h"
#include "raylib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

CoreData DATA;

Vector2 randomInCircle(Vector2 center, float radius) {
	float theta = (float)GetRandomValue(0, 360) * DEG2RAD;
	float r = sqrtf(GetRandomValue(1, radius * radius));

	Vector2 toReturn = (Vector2){(float)center.x + r * cosf(theta),
															 (float)center.y + r * sinf(theta)};

	return toReturn;
}

void atomSpawn(Vector2 pos, int charge, int rotation, int count, bool random) {

	DATA.Atoms.atomsList = realloc(DATA.Atoms.atomsList, (DATA.Atoms.atomsCount + count) * sizeof(atom));

	if (!DATA.Atoms.atomsList) {
		assert("ERROR: MALLOC EXPLODED: check atom array setup in atomSpawn");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < count; i++) {

		if (random) {
			DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].pos = randomInCircle(GetMousePosition(), DATA.Settings.scrollActionSpeed);
		}
		if (!random) {
			DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].pos = pos;
		}

		DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].charge = charge;
		DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].rot = rotation;

		switch (charge) {
		case 1:
			DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].size = 25;
			break;
		case 0:
			DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].size = 25;
			break;
		case -1:
			DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].size = 10;
			break;
		default: {
			assert("ERROR: INVALID CHARGE AT SPAWNING ATOM");
			exit(EXIT_FAILURE);
		}
		}
		DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].selected = 0;
		DATA.Atoms.atomsList[DATA.Atoms.atomsCount + i].vel = (Vector2){0, 0};
	}

	DATA.Atoms.atomsCount = DATA.Atoms.atomsCount + count;
	updateSSBO();
	// TraceLog(LOG_INFO, "debug: %d", scrollActionSpeed);	debug
	TraceLog(LOG_INFO, "DZIALA TUTAJ");
}

void atomDelete(size_t index) {
	if (index >= DATA.Atoms.atomsCount)
		return;

	for (size_t i = index; i < DATA.Atoms.atomsCount - 1; i++) {
		DATA.Atoms.atomsList[i] = DATA.Atoms.atomsList[i + 1];
	}

	DATA.Atoms.atomsCount--;

	if (DATA.Atoms.atomsCount > 0) {
		atom *temp = realloc(DATA.Atoms.atomsList, sizeof(atom) * DATA.Atoms.atomsCount);
		if (temp != NULL) {
			DATA.Atoms.atomsList = temp;
		}
	} else {
		free(DATA.Atoms.atomsList);
		DATA.Atoms.atomsList = NULL;
	}
}

void circleSelect() {
	if (!DATA.Info.isPaused) {
		if (DATA.Settings.scrollActionSpeed >= 2) {
			DATA.Settings.scrollActionSpeed += GetMouseWheelMove() * DATA.Settings.mouseWheelSensivity;
		} else {
			DATA.Settings.scrollActionSpeed = 2;
		}
		Vector2 circleCenter = GetMousePosition();

		// TraceLog(LOG_INFO, "mousewheel: %d", selectorSize);	 debug for printing
		// out the selection circle size

		int selected = 0;
		if (IsKeyDown(KEY_LEFT_SHIFT)) {
			TraceLog(LOG_INFO, "%.1f, %.1f vecttisr mouse", GetMousePosition().x, GetMousePosition().y);
			if (IsKeyPressed(DATA.Keybinds.selectKey)) {
				for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
					if (CheckCollisionPointCircle(DATA.Atoms.atomsList[i].pos, GetScreenToWorld2D(circleCenter, DATA.camera), DATA.Settings.scrollActionSpeed / DATA.camera.zoom)) {
						DATA.Atoms.atomsList[i].selected = true;
						selected++;
					}
				}
				TraceLog(LOG_INFO, "t:%.1f		Added to selection %d atoms", GetTime(),selected);
			}
		}
		if (!IsKeyDown(KEY_LEFT_SHIFT)) {

			if (IsKeyPressed(DATA.Keybinds.selectKey)) {
				for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
					DATA.Atoms.atomsList[i].selected = 0;
					if (CheckCollisionPointCircle(DATA.Atoms.atomsList[i].pos, GetScreenToWorld2D(circleCenter, DATA.camera), DATA.Settings.scrollActionSpeed / DATA.camera.zoom)) {
						DATA.Atoms.atomsList[i].selected = 1;
						selected++;
					}
				}
				TraceLog(LOG_INFO, "t:%.1f		Selected %d atoms", GetTime(), selected);
			}
		}
	}
}

multiSpawningInfo spawningUI(double elapsed, bool reset) {
	static char inputString[MAXINPUTCHARS + 1] = "\0";
	static int currentDigitCount = 0;
	if (reset) {
		for (int i = 0; i < MAXINPUTCHARS + 1; i++) {
			inputString[i] = ' ';
			currentDigitCount = 0;
		}
		TraceLog(LOG_INFO, "RESETED STRING");
	}
	bool typing = true;
	Rectangle textBox = (Rectangle){5, 5, GetScreenWidth() - 10, 25};
	static int frameCounter = 0;
	(void)frameCounter;

	int key = GetCharPressed();
	while (key > 0) {
		if ((key >= 48) && (key <= 57) && (currentDigitCount < MAXINPUTCHARS)) {
			inputString[currentDigitCount] = (char)key;
			inputString[currentDigitCount + 1] = '\0';
			currentDigitCount++;
		}

		key = GetCharPressed();
	}

	if (IsKeyPressed(KEY_BACKSPACE)) {
		currentDigitCount--;
		if (currentDigitCount < 0)
			currentDigitCount = 0;
		inputString[currentDigitCount] = '\0';
	}

	DrawText(inputString, 10, 14, 16, WHITE);
	DrawRectangleRoundedLines(textBox, 0.4f, 1, WHITE);
	DrawLineEx((Vector2){10, 30},
						 (Vector2){(GetScreenWidth() - 10) - (elapsed / DATA.Settings.selectionDuration *
																									(GetScreenWidth() - 20)),30}, 2.0f, RED);
	int countToReturn = atoi(inputString);
	multiSpawningInfo returnThing = (multiSpawningInfo){countToReturn, 0, typing};
	return returnThing;
}

short selectingChargeUI(double elapsed, bool reset) {
	(void)reset;
	short selectedCharge = 1;
	bool chosen = false;
	if (IsKeyPressed(KEY_ONE)) {
		selectedCharge = 1;
		chosen = true;
	}
	if (IsKeyPressed(KEY_TWO)) {
		selectedCharge = 0;
		chosen = true;
	}
	if (IsKeyPressed(KEY_THREE)) {
		selectedCharge = -1;
		chosen = true;
	}
	Rectangle textBox = (Rectangle){5, 5, GetScreenWidth() - 10, 25};
	DrawRectangleRoundedLines(textBox, 0.4f, 1, WHITE);
	DrawLineEx((Vector2){10, 30},(Vector2){(GetScreenWidth() - 10) - (elapsed / DATA.Settings.selectionDuration * GetScreenWidth() - 20), 30}, 2.0f, RED);

	if (chosen) {
		return selectedCharge;
	} else {
		return -10;
	}
}

void atomSpawning() {

	// "A" key for spawning an atom

	static double selectionStartTime = 0.0f;

	short spawningCharge = 1;
	static bool chosen;
	static bool shiftWasPressed;

	static bool enteringNumbers;
	static bool selectingCharge;

	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		shiftWasPressed = true;
	}

	if (IsKeyPressed(DATA.Keybinds.addKey)) {
		selectionStartTime = GetTime();
		if (shiftWasPressed) {
			enteringNumbers = true;
			selectingCharge = false;
		} else {
			enteringNumbers = false;
			selectingCharge = true;
		}
	}
	static unsigned int spawningCount;
	if (enteringNumbers) {
		double elapsed = GetTime() - selectionStartTime;
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
		multiSpawningInfo spawningInfo = spawningUI(elapsed, false);
		spawningCount = spawningInfo.count; // how many atoms to spawn

		if (elapsed >= DATA.Settings.selectionDuration) { // if timer ends
			enteringNumbers = false;
			shiftWasPressed = false;
			selectingCharge = true;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			spawningUI(elapsed, true);
			if (spawningCount == 0) {
				spawningCount = 1;
			}
			selectionStartTime = GetTime();
		}
		if (IsKeyPressed(KEY_ENTER)) { // if user selects number to spawn
			enteringNumbers = false;
			shiftWasPressed = false;
			selectingCharge = true;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			spawningUI(elapsed, true);
			// TraceLog(LOG_INFO, "enter: %d", spawningCount);
			selectionStartTime = GetTime();
			if (spawningCount == 0) {
				spawningCount = 1;
			}
		}
	}
	short chargeToSpawn = -10;
	if (selectingCharge) {

		double elapsed = GetTime() - selectionStartTime;
		chargeToSpawn = selectingChargeUI(elapsed, false);
		if (elapsed >= DATA.Settings.selectionDuration) {
			selectingCharge = false;
			chargeToSpawn = 0;
			chosen = false;
		}
		if (chargeToSpawn > -10) {
			chosen = true;
		}
	}
	if (chosen == true) {
		selectingCharge = false;

		atomSpawn(randomInCircle(GetMousePosition(), DATA.Settings.scrollActionSpeed), chargeToSpawn, 0, spawningCount, true);

		chosen = false;

		TraceLog(LOG_INFO, "t:%.1f		Spawned %d atoms, charge: %d", GetTime(), spawningCount, spawningCharge);
		spawningCount = 0;
	}
}

void arrowMoving() {
	if (!DATA.Info.isPaused) {

		DATA.Settings.movementSpeed = DATA.Settings.scrollActionSpeed / 4;
		if (IsKeyDown(KEY_UP)) {
			for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
				if (DATA.Atoms.atomsList[i].selected == 1) {
					DATA.Atoms.atomsList[i].vel.y = DATA.Atoms.atomsList[i].vel.y - DATA.Settings.movementSpeed;
				}
			}
		}
		if (IsKeyDown(KEY_DOWN)) {
			for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
				if (DATA.Atoms.atomsList[i].selected == 1) {
					DATA.Atoms.atomsList[i].vel.y = DATA.Atoms.atomsList[i].vel.y + DATA.Settings.movementSpeed;
				}
			}
		}
		if (IsKeyDown(KEY_LEFT)) {
			for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
				if (DATA.Atoms.atomsList[i].selected == 1) {
					DATA.Atoms.atomsList[i].vel.x = DATA.Atoms.atomsList[i].vel.x - DATA.Settings.movementSpeed;
				}
			}
		}
		if (IsKeyDown(KEY_RIGHT)) {
			for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
				if (DATA.Atoms.atomsList[i].selected == 1) {
					DATA.Atoms.atomsList[i].vel.x = DATA.Atoms.atomsList[i].vel.x + DATA.Settings.movementSpeed;
				}
			}
		}

		// TODO rotate the selected atoms while using mouse as center

		if (IsKeyDown(KEY_Q)) {
		for(size_t i = 0; i < DATA.Atoms.atomsCount; i++) {
					if(DATA.Atoms.atomsList[i].selected) { DATA.Atoms.atomsList[i].rot = DATA.Atoms.atomsList[i].rot - DATA.Settings.rotationSpeed; }
		}
		}
		if (IsKeyDown(KEY_E)) {
		for(size_t i = 0; i < DATA.Atoms.atomsCount; i++) {
					if(DATA.Atoms.atomsList[i].selected) { DATA.Atoms.atomsList[i].rot = DATA.Atoms.atomsList[i].rot + DATA.Settings.rotationSpeed; }
		}
		}
	}
}

void setVelZero() {
	if (IsKeyDown(KEY_ZERO)) {
		for (size_t i = 0; i < DATA.Atoms.atomsCount; i++) {
			if (DATA.Atoms.atomsList[i].selected == true) {
				DATA.Atoms.atomsList[i].vel = (Vector2){0, 0};
			}
		}
	}
}
void debugActions() {
	if (IsKeyDown(KEY_F1)) {
		for (size_t i = 0; i < DATA.Atoms.atomsCount; i++) {
			DrawPixel(DATA.Atoms.atomsList[i].pos.x, DATA.Atoms.atomsList[i].pos.y,
								GREEN); // press F1 to see the atoms center
		}
	}
}

void deleteSelectedAtoms() {
	if (IsKeyPressed(DATA.Keybinds.deleteKey)) {
		for (size_t i = 0; i < DATA.Atoms.atomsCount; i++) {
			if (DATA.Atoms.atomsList[i].selected == 1) {
				atomDelete(i);
			}
		}
	}
}

void userActions() {
	circleSelect();
	arrowMoving();
	cameraControls();
}

int main() {
	DATA.Atoms.atomsList = NULL;
	DATA.Atoms.atomsCount = 0;

	DATA.Keybinds.addKey = KEY_G;
	DATA.Keybinds.selectKey = KEY_F;
	DATA.Keybinds.deleteKey = KEY_R;
	DATA.Keybinds.camMovementUp = KEY_W;
	DATA.Keybinds.camMovementDown = KEY_S;
	DATA.Keybinds.camMovementLeft = KEY_A;
	DATA.Keybinds.camMovementRight = KEY_D;
	DATA.Keybinds.camZoomOut = KEY_Q;
	DATA.Keybinds.camZoomIn = KEY_E;
	DATA.Keybinds.pauseKey = KEY_P;

	DATA.Settings.newtonianConst = 10000;
	DATA.Settings.paddingTop = 35;
	DATA.Settings.mouseWheelSensivity = 10;
	DATA.Settings.movementSpeed = 10;
	DATA.Settings.rotationSpeed = 2.5;
	DATA.Settings.selectionDuration = 10.0f;
	
	DATA.Info.isPaused = false;
	DATA.Info.halfScreenWidth = 0.0f;
	DATA.Info.halfScreenHeight = 0.0f;

	DATA.Shaders.dtLoc = 0;
	DATA.Shaders.countLoc = 0;
	DATA.Shaders.ssbo = 0;

	const int screenWidth = 800;
	const int screenHeight = 800;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(screenWidth, screenHeight, "raya");

	initComputeShader();
	DATA.Shaders.dtLoc = GetShaderLocation(DATA.Shaders.shaderCompute, "dt");
	(void)DATA.Shaders.dtLoc;
	DATA.Shaders.countLoc = GetShaderLocation(DATA.Shaders.shaderCompute, "particleCount");
	(void)DATA.Shaders.countLoc;

	DATA.Shaders.ssbo = rlLoadShaderBuffer(sizeof(atom) * DATA.Atoms.atomsCount, DATA.Atoms.atomsList, RL_DYNAMIC_COPY);

	double startTime = GetTime();
	double pausedTime = 0.0;
	double totalPausedDuration = 0.0;

	DATA.Settings.scrollActionSpeed = 32;

	SetExitKey(KEY_NULL);

	Texture2D textureProton = LoadTexture("assets/proton.png");
	Texture2D textureNeutron = LoadTexture("assets/neutron.png");
	Texture2D textureElectron = LoadTexture("assets/electron.png");

	initCamera();

	SetTraceLogLevel(LOG_INFO); // some thhing for logs

	SetTargetFPS(60);

	while (!WindowShouldClose()) {
		if (IsKeyPressed(DATA.Keybinds.pauseKey)) {
			if (DATA.Info.isPaused) {
				totalPausedDuration += GetTime() - pausedTime;
				DATA.Info.isPaused = false;
			} else {
				pausedTime = GetTime();
				DATA.Info.isPaused = true;
			}
		}

		DATA.Info.halfScreenWidth = (float){GetScreenWidth()} / 2;
		DATA.Info.halfScreenHeight = (float){GetScreenHeight()} / 2;

		double elapsedTime;
		if (DATA.Info.isPaused) {
			elapsedTime = pausedTime - startTime - totalPausedDuration;
		} else {
			elapsedTime = GetTime() - startTime - totalPausedDuration;
		}

		BeginDrawing();

		ClearBackground(BLACK);

		atomSpawning();

		if (!DATA.Info.isPaused) {
			atomPhysics();
		}
		BeginMode2D(DATA.camera);

		userActions();

		Texture textureToRender;
		int centering = 0;
		for (int i = 0; i < (int)DATA.Atoms.atomsCount; i++) {
			switch (DATA.Atoms.atomsList[i].charge) {
			case -1:
				textureToRender = textureElectron;
				centering = 8;
				break;
			case 0:
				textureToRender = textureNeutron;
				centering = 16;
				break;
			case 1:
				textureToRender = textureProton;
				centering = 16;
				break;
			default:
				TraceLog(LOG_INFO, "ERROR: while selecting texture for particle based "
													 "on charge; invalid charge");
				break;
			}
			DrawTexture(textureToRender, DATA.Atoms.atomsList[i].pos.x - centering + 1,
									DATA.Atoms.atomsList[i].pos.y - centering + 1, WHITE);
			if (DATA.Atoms.atomsList[i].selected == 1) {
				DrawCircleLines(DATA.Atoms.atomsList[i].pos.x + 1, DATA.Atoms.atomsList[i].pos.y + 1,
												centering, YELLOW);
				DrawCircleLines(DATA.Atoms.atomsList[i].pos.x + 1, DATA.Atoms.atomsList[i].pos.y + 1,
												centering + 1, YELLOW);
				DrawCircleLines(DATA.Atoms.atomsList[i].pos.x + 1, DATA.Atoms.atomsList[i].pos.y + 1,
												centering + 2, YELLOW);
			}
		}

		debugActions();

		EndMode2D();

		DrawText(TextFormat("Simulation time %.1f", elapsedTime), 25, DATA.Settings.paddingTop,
						 16, WHITE);
		DrawText(TextFormat("dt: %.3f ms", GetFrameTime() * 1000), 25,
						 (DATA.Settings.paddingTop - 5) * 2, 16, WHITE);
		DrawText(TextFormat("Brush size: %.0f px", DATA.Settings.scrollActionSpeed), 25,
						 (DATA.Settings.paddingTop - 8) * 3, 16, WHITE);

		DrawCircleLinesV((Vector2){GetMousePosition().x * GetWindowScaleDPI().x,
															 GetMousePosition().y * GetWindowScaleDPI().y},
										 DATA.Settings.scrollActionSpeed, WHITE);

		if (!DATA.Info.isPaused) {

		} else {
			DrawText("Paused! Press P to unpause.", 225, 25, 16, RED);
		}

		EndDrawing();
	}

	UnloadTexture(textureProton);
	UnloadTexture(textureElectron);
	UnloadTexture(textureNeutron);
	free(DATA.Atoms.atomsList);
	rlUnloadShaderBuffer(DATA.Shaders.ssbo);
	UnloadShader(DATA.Shaders.shaderCompute);

	CloseWindow();

	return 0;
}
