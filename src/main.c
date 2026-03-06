#include <math.h>
#include <raylib.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef struct {
	Vector2 pos;
	Vector2 force;
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
float movementSpeed = 1.5;
float rotationSpeed = 2.5;
//spawningUI SETTINGS
#define MAXINPUTCHARS  50
const float selectionDuration = 10.0f;

//info texts SETTINGS
int paddingTop = 35;


atom* atomsList = NULL;
size_t atomCount = 0;

bool isPaused = false;
float scrollActionSpeed;

Vector2 randomInCircle(Vector2 center, float radius){
	float theta = (float)GetRandomValue(0, 360);
	float r = sqrtf(GetRandomValue(0, radius * radius));
	Vector2 toReturn = (Vector2){(float)center.x + r * cosf(theta), (float)center.y + r * sinf(theta)};
	return toReturn;
}

void atomSpawn(Vector2 pos, int charge, int rotation, int count, bool random) {
	
	atomsList = realloc(atomsList, (atomCount + count) * sizeof(atom));
	
	if(!atomsList) {
		assert("ERROR: MALLOC EXPLODED: check atom array setup in atomSpawn");
		exit(EXIT_FAILURE);
	}
	

	for(unsigned int i = 0; i < count; i++){
		
		if(random){
			atomsList[atomCount + i].pos = randomInCircle(GetMousePosition(), scrollActionSpeed);
		} 
		if(!random){
			atomsList[atomCount + i].pos = pos;
		}

		atomsList[atomCount + i].charge = charge;
		atomsList[atomCount + i].rot = rotation;
	
		switch(charge){
			case 1:
				atomsList[atomCount + i].size = 25;
				break;
			case 0:
				atomsList[atomCount + i].size = 25;
				break;
			case -1:
				atomsList[atomCount + i].size = 10;
				break;
			default: {
				assert("ERROR: INVALID CHARGE AT SPAWNING ATOM");
				exit(EXIT_FAILURE);
			}
		}
		atomsList[atomCount + i].selected = false;
		atomsList[atomCount + i].force = (Vector2){0, 0};

	}

	atomCount = atomCount + count;
	//TraceLog(LOG_INFO, "debug: %d", scrollActionSpeed);  debug
	TraceLog(LOG_INFO, "DZIALA TUTAJ");
	
}



void circleSelect(){
		if(!isPaused){
			if(scrollActionSpeed >= 2 ){
				scrollActionSpeed += GetMouseWheelMove() * mouseWheelSensivity;
			} else 	{
			scrollActionSpeed = 2;
			}
		Vector2 circleCenter = GetMousePosition();
		DrawCircleLinesV(circleCenter, scrollActionSpeed, WHITE);
		//TraceLog(LOG_INFO, "mousewheel: %d", selectorSize);   debug for printing out the selection circle size
		
		int selected = 0;
			if(IsKeyDown(KEY_LEFT_SHIFT)){

				if(IsKeyPressed(KEY_S)){
					for(int i = 0; i < (int)atomCount; i++){
						if(CheckCollisionPointCircle(atomsList[i].pos, circleCenter, scrollActionSpeed)){
							atomsList[i].selected = true;
							selected++;
						}
					}
				TraceLog(LOG_INFO, "t:%.1f    Added to selection %d atoms", GetTime(), selected);
				}
		
			}
			if(!IsKeyDown(KEY_LEFT_SHIFT)){

				if(IsKeyPressed(KEY_S)){
					for(int i = 0; i < (int)atomCount; i++){
						atomsList[i].selected = false;
						if(CheckCollisionPointCircle(atomsList[i].pos, circleCenter, scrollActionSpeed)){
							atomsList[i].selected = true;
							selected++;
						}
					}
				TraceLog(LOG_INFO, "t:%.1f    Selected %d atoms", GetTime(), selected);
				}
		
			}
		}
	DrawText(TextFormat("Brush size: %.0f px", scrollActionSpeed), 25, (paddingTop - 8) * 3, 16, WHITE);
	 
}

multiSpawningInfo spawningUI(double elapsed, bool reset){
	static char inputString[MAXINPUTCHARS + 1] = "\0";
	static int currentDigitCount = 0;
	if(reset){
		for(int i = 0; i < MAXINPUTCHARS + 1; i++) {
			inputString[i] = ' ';
			currentDigitCount = 0;
		}
		TraceLog(LOG_INFO, "RESETED STRING");
	}
	bool typing = true;
	Rectangle textBox = (Rectangle){5, 5, GetScreenWidth() - 10, 25};
	static int frameCounter = 0;
	
	
	int key = GetCharPressed();
	while (key > 0) {
		if ((key >= 48) && (key <= 57) && (currentDigitCount < MAXINPUTCHARS)){
        	inputString[currentDigitCount] = (char)key;
            inputString[currentDigitCount + 1] = '\0';                     
			currentDigitCount++;
        }

		key = GetCharPressed(); 
	}
	
    if (IsKeyPressed(KEY_BACKSPACE)){
		currentDigitCount--;
		if (currentDigitCount < 0) currentDigitCount = 0;
        inputString[currentDigitCount] = '\0';
	}
	
	DrawText(inputString, 10, 14, 16 ,WHITE);
	DrawRectangleRoundedLines(textBox, 0.4f, 1, WHITE);
	DrawLineEx((Vector2){10, 30}, (Vector2){(GetScreenWidth() - 10) - (elapsed/selectionDuration * (GetScreenWidth() - 20)), 30}, 2.0f, RED);
	int countToReturn = atoi(inputString);
	multiSpawningInfo returnThing = (multiSpawningInfo){countToReturn, 0};
	return returnThing;
}

short selectingChargeUI(double elapsed, bool reset) {
		short selectedCharge = 1;
		bool chosen = false;
		if(IsKeyPressed(KEY_ONE)){
			selectedCharge = 1;
			chosen = true;
		}
		if(IsKeyPressed(KEY_TWO)){
			selectedCharge = 0;
			chosen = true;
		}
		if(IsKeyPressed(KEY_THREE)){
			selectedCharge = -1;
			chosen = true;
		}
		Rectangle textBox = (Rectangle){5, 5, GetScreenWidth() - 10, 25};
		DrawRectangleRoundedLines(textBox, 0.4f, 1, WHITE);
		DrawLineEx((Vector2){10, 30}, (Vector2){(GetScreenWidth() - 10) - (elapsed/selectionDuration * (GetScreenWidth() - 20)), 30}, 2.0f, RED);

		if(chosen){
			return selectedCharge;
		}
		else {
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
	
	if(IsKeyDown(KEY_LEFT_SHIFT)){
		shiftWasPressed = true;
	}

	if(IsKeyPressed(KEY_A)){
		selectionStartTime = GetTime();
		if(shiftWasPressed){
			enteringNumbers = true;
			selectingCharge = false;
		} else {
			enteringNumbers = false;
			selectingCharge = true;
		}
	}
	static unsigned int spawningCount;
	if(enteringNumbers){
		double elapsed = GetTime() - selectionStartTime;
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
		multiSpawningInfo spawningInfo = spawningUI(elapsed, false);
		spawningCount = spawningInfo.count; // how many atoms to spawn
		
		if(elapsed >= selectionDuration) { // if timer ends
			enteringNumbers = false;
			shiftWasPressed = false;
			selectingCharge = true;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			spawningUI(elapsed, true);
			if(spawningCount == 0){
				spawningCount = 1;
			}
			selectionStartTime = GetTime();
		} 
		if(IsKeyPressed(KEY_ENTER)) { // if user selects number to spawn
			enteringNumbers = false;
			shiftWasPressed = false;
			selectingCharge = true;
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			spawningUI(elapsed, true);
			TraceLog(LOG_INFO, "enter: %d", spawningCount);
			selectionStartTime = GetTime();
			if(spawningCount == 0){
				spawningCount = 1;
			}
		}

}
	short chargeToSpawn = -10;
	if(selectingCharge) {
		
		double elapsed = GetTime() - selectionStartTime;
		chargeToSpawn = selectingChargeUI(elapsed, false);
		if(elapsed >= selectionDuration) {
			selectingCharge = false;
			chargeToSpawn = 0;
			chosen = false;
		}
		if(chargeToSpawn > -10){
			chosen = true;
		}

	}  
	if (chosen == true) {
		selectingCharge = false;
		
		atomSpawn(randomInCircle(GetMousePosition(), scrollActionSpeed), chargeToSpawn, 0, spawningCount, true);
		
		chosen = false;
		
		TraceLog(LOG_INFO, "t:%.1f    Spawned %d atoms, charge: %d", GetTime(), spawningCount, spawningCharge);
		spawningCount = 0;
	}
}

void arrowMoving(){
		if(!isPaused) {
			
			movementSpeed = scrollActionSpeed / 4;
			if(IsKeyDown(KEY_UP)) {
				for(int i = 0; i < (int)atomCount; i++){
						if(atomsList[i].selected == true) {
							atomsList[i].pos.y = atomsList[i].pos.y - movementSpeed;
						}
				}
			}
			if(IsKeyDown(KEY_DOWN)) {
				for(int i = 0; i < (int)atomCount; i++){
						if(atomsList[i].selected == true) {
							atomsList[i].pos.y = atomsList[i].pos.y + movementSpeed;
						}
				}
			}
			if(IsKeyDown(KEY_LEFT)) {
				for(int i = 0; i < (int)atomCount; i++){
						if(atomsList[i].selected == true) {
							atomsList[i].pos.x = atomsList[i].pos.x - movementSpeed;
						}
				}
			}
			if(IsKeyDown(KEY_RIGHT)) {
				for(int i = 0; i < (int)atomCount; i++){
						if(atomsList[i].selected == true) {
							atomsList[i].pos.x = atomsList[i].pos.x + movementSpeed;
						}
				}
			}



			if(IsKeyDown(KEY_Q)) { 
				atomsList[0].rot = atomsList[0].rot - rotationSpeed; 
			}
			if(IsKeyDown(KEY_E)) { 
				atomsList[0].rot = atomsList[0].rot + rotationSpeed; 
			}

		}
}

void userActions(){

	atomSpawning();
	circleSelect();
	arrowMoving();

}




int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

    const int screenWidth = 800;
    const int screenHeight = 800;

	
	atomSpawn((Vector2){250, 250}, 1, 0, 1, false);
	
	atomsList[0].pos = (Vector2){100, 100};
	atomsList[0].size = 25;


	double startTime = GetTime();
	double pausedTime = 0.0;
	double totalPausedDuration = 0.0;

	scrollActionSpeed = 16;
	

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raya sim");
	
	SetTraceLogLevel(LOG_INFO);  // some thhing for logs

    SetTargetFPS(60);
	
    while(!WindowShouldClose()) {	
		if(IsKeyPressed(KEY_P)){
			if(isPaused) {
				totalPausedDuration += GetTime() - pausedTime;
				isPaused = false;
			}
			else {
				pausedTime = GetTime();
				isPaused = true;
			}
		}
		
		

		double elapsedTime;
		if(isPaused) {
			elapsedTime = pausedTime - startTime - totalPausedDuration;
		}
		else {
			elapsedTime = GetTime() - startTime - totalPausedDuration;
		}
		
		


	
   	    BeginDrawing();

        ClearBackground(BLACK);



		
		for(int i = 0; i < (int)atomCount; i++){
			Color color = (Color){255, 255, 255, 255};
			switch(atomsList[i].charge){
				case -1: 
					color = (Color){255, 255, 255, 255};
					break;
				case 0:
					color = (Color){0, 255, 255, 255};
					break;
				case 1:
					color = (Color){255, 0, 0, 255};
					break;
			}
			Rectangle rectangle = (Rectangle){atomsList[i].pos.x, atomsList[i].pos.y, atomsList[i].size, atomsList[i].size};

			Vector2 origin = (Vector2){atomsList[i].size/2, atomsList[i].size / 2};

        	DrawRectanglePro(
					rectangle,
					origin,
					atomsList[i].rot, 
					color);

			if(atomsList[i].selected == true) {
				DrawRectangleLinesEx((Rectangle){atomsList[i].pos.x - atomsList[i].size/2,
										atomsList[i].pos.y - atomsList[i].size/2,
										atomsList[i].size, atomsList[i].size}, 2.0f, YELLOW);
			}

 		}
		
		userActions();

		DrawText(TextFormat("Simulation time %.1f", elapsedTime), 25, paddingTop, 16, WHITE);
		DrawText(TextFormat("dt: %.3f ms", GetFrameTime() * 1000), 25, (paddingTop - 5) * 2, 16, WHITE);
		
		if(!isPaused) {

		} else {
			DrawText("Paused! Press P to unpause.", 225, 25, 16, RED);
		}

        EndDrawing();
    }


    CloseWindow();        

	free(atomsList);

    return 0;
}
