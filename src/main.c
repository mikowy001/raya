#include <raylib.h>

#include <stdlib.h>
#include <assert.h>

typedef struct {
	Vector2 pos;
	Vector2 force;
	int charge;
    int rot;
	float size;
} atom;

size_t atomCount = 0;

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

    const int screenWidth = 800;
    const int screenHeight = 800;

	float movementSpeed = 1.5;
	float rotationSpeed = 2.5;
	
	atom* atomsList = malloc(++atomCount * sizeof(atom));
	if(!atomsList) {
		assert("ERROR: MALLOC FUCKING EXPLODED");
		exit(EXIT_FAILURE);
	}
	
	atomsList[0].pos = (Vector2){100, 100};
	atomsList[0].size = 25;

	atomsList[1].pos = (Vector2){100, 100};
	atomsList[1].size = 25;

	
	int selectorSize = 50;

	bool isPaused = false;
	double startTime = GetTime();
	double pausedTime = 0.0;
	double totalPausedDuration = 0.0;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raya sim");
	
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
		

		if(!isPaused) {
			if(IsKeyDown(KEY_W)) { atomsList[0].pos.y = atomsList[0].pos.y - movementSpeed; }
			if(IsKeyDown(KEY_S)) { atomsList[0].pos.y = atomsList[0].pos.y + movementSpeed; }
			if(IsKeyDown(KEY_A)) { atomsList[0].pos.x = atomsList[0].pos.x  - movementSpeed; }
			if(IsKeyDown(KEY_D)) { atomsList[0].pos.x  = atomsList[0].pos.x  + movementSpeed; }

			if(IsKeyDown(KEY_Q)) { atomsList[0].rot = atomsList[0].rot - rotationSpeed; }
			if(IsKeyDown(KEY_E)) { atomsList[0].rot = atomsList[0].rot + rotationSpeed; }
		}
	
   	    BeginDrawing();

        ClearBackground(BLACK);

		DrawText(TextFormat("Simulation time %.1f", elapsedTime), 25, 25, 16, WHITE);
		DrawText(TextFormat("dt: %.3f ms", GetFrameTime() * 1000), 25, 50, 16, WHITE);
		
		if(!isPaused) {
			DrawCircleLinesV(GetMousePosition(), selectorSize, WHITE);
		} else {
			DrawText("Paused! Press P to unpause.", 225, 25, 16, RED);
		}


		
		for(int i = 0; i < (int)atomCount; i++){
        	DrawRectanglePro((Rectangle){atomsList[i].pos.x, atomsList[i].pos.y, atomsList[i].size, atomsList[i].size}, 
					(Vector2){atomsList[i].size/2, atomsList[i].size / 2}, 
					atomsList[i].rot, 
					(Color){255, 0, 0, 255 });
 		}
        EndDrawing();
    }


    CloseWindow();        

	free(atomsList);

    return 0;
}
