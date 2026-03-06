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
} multiSpawningInfo;

//GLOBAL SETTINGS !!!
int mouseWheelSensivity = 10;
//arrow moving
float movementSpeed = 1.5;
float rotationSpeed = 2.5;
//spawningUI SETTINGS
#define MAX_INPUT_CHARS = 50;


atom* atomsList = NULL;
size_t atomCount = 0;

bool isPaused = false;
float scrollActionSpeed;



void atomSpawn(Vector2 pos, int charge, int rotation) {
	atomsList = realloc(atomsList, (atomCount + 1) * sizeof(atom));
	if(!atomsList) {
		assert("ERROR: MALLOC EXPLODED: check atom array setup in atomSpawn");
		exit(EXIT_FAILURE);
	}
	atomsList[atomCount].pos = pos;
	atomsList[atomCount].charge = charge;
	atomsList[atomCount].rot = rotation;

	switch(charge){
		case 1:
			atomsList[atomCount].size = 25;
			break;
		case 0:
			atomsList[atomCount].size = 25;
			break;
		case -1:
			atomsList[atomCount].size = 10;
			break;
		default: {
			assert("ERROR: INVALID CHARGE AT SPAWNING ATOM");
			exit(EXIT_FAILURE);
		}
	}

	atomsList[atomCount].selected = false;
	atomsList[atomCount].force = (Vector2){0, 0};

	atomCount++;

	//TraceLog(LOG_INFO, "debug: %d", scrollActionSpeed);  debug
}



void circleSelect(){
		if(!isPaused){
			if(scrollActionSpeed >= 2 ){
				scrollActionSpeed += GetMouseWheelMove() * mouseWheelSensivity;
			} else {
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
	DrawText(TextFormat("Brush size: %.0f px", scrollActionSpeed), 25, 75, 16, WHITE);
	
}

multiSpawningInfo spawningUI(){
	bool typing = true;
	Rectangle textBox = (Rectangle){25, 50, GetScreenWidth(), 75};
	static int frameCounter = 0;

	DrawRectangleRoundedLines(textBox, 10.0f, 2, WHITE);
}

void atomSpawning() {
	
	// "A" key for spawning an atom
	
	static double selectionStartTime = 0.0f;  
    static const float selectionDuration = 10.0f;  
	
	short spawningCharge = 1;
	static bool AwasPressed;
	static bool chosen;
	bool shiftWasPressed;
	
	shiftWasPressed = IsKeyDown(KEY_LEFT_SHIFT);

	if(IsKeyPressed(KEY_A)){
		AwasPressed = true;
		selectionStartTime = GetTime();
	}
	if(AwasPressed) {
		
		if(shiftWasPressed) {
			TraceLog(LOG_INFO, "a + shift was pressed");
			shiftWasPressed = false;
			spawningUI();
			//if(){}
		}

		double elapsed = GetTime() - selectionStartTime;
		if(elapsed >= selectionDuration) {
			AwasPressed = false;
			chosen = false;
		}

		if(IsKeyPressed(KEY_ONE)){
			spawningCharge = 1;
			chosen = true;
		}
		if(IsKeyPressed(KEY_TWO)){
			spawningCharge = 0;
			chosen = true;
		}
		if(IsKeyPressed(KEY_THREE)){
			spawningCharge = -1;
			chosen = true;
		}
	}  
	if (chosen == true) {
		AwasPressed = false;
		atomSpawn(GetMousePosition(), spawningCharge, 0);
		chosen = false;
		TraceLog(LOG_INFO, "t:%.1f    Spawned %d atoms, charge: %d", GetTime(), 1, spawningCharge);
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

	
	atomSpawn((Vector2){250, 250}, 1, 0);
	
	atomsList[0].pos = (Vector2){100, 100};
	atomsList[0].size = 25;

	char inputString[MAX_INPUT_CHARS + 1] = "\0";
	int currentLetterCount = 0;
	Rectangle textBox;

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

		DrawText(TextFormat("Simulation time %.1f", elapsedTime), 25, 25, 16, WHITE);
		DrawText(TextFormat("dt: %.3f ms", GetFrameTime() * 1000), 25, 50, 16, WHITE);
		
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
