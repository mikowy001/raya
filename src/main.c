#include "./raya.h"
#include <raylib.h>


int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

    const int screenWidth = 800;
    const int screenHeight = 800;

	float movementSpeed = 1.5;
	float rotationSpeed = 2.5;
	
	atom* atomsList;
	CreateAtomArray(&atomsList, 1);
	
	
	atomsList[1].pos = (Vector2){100, 100};
	atomsList[1].size = 1;
	
	int selectorSize = 50;

	bool isPaused = false;
	double startTime = GetTime();
	double pausedTime = 0.0;
	double totalPausedDuration = 0.0;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "raya sim");
	

    SetTargetFPS(60);
	
    while (!WindowShouldClose()) {	
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
		
		int x = atomsList[1].pos.x;
		int y = atomsList[1].pos.y;
		int r = atomsList[1].rot;

		if(!isPaused) {
			if(IsKeyDown(KEY_W)) { y = y - movementSpeed; }
			if(IsKeyDown(KEY_S)) { y = y + movementSpeed; }
			if(IsKeyDown(KEY_A)) { x = x - movementSpeed; }
			if(IsKeyDown(KEY_D)) { x = x + movementSpeed; }

			if(IsKeyDown(KEY_Q)) { r = r - rotationSpeed; }
			if(IsKeyDown(KEY_E)) { r = r + rotationSpeed; }
		}
	



   	    BeginDrawing();

        ClearBackground(BLACK);

		DrawText(TextFormat("Simulation time %.1f", elapsedTime), 25, 25, 16, WHITE);
		DrawText(TextFormat("dt: %.3f ms", GetFrameTime() * 1000), 25, 50, 16, WHITE);
		
		if(isPaused){
			DrawText("Paused!        Press P to unpause.", 225, 25, 16, RED);
		} else {
			DrawCircleLinesV(GetMousePosition(), selectorSize, WHITE);
		}


		
		for(int i = 0; i< sizeof(atomsList); i++){
        	DrawRectanglePro((Rectangle){atomsList[i].pos.x, atomsList[i].pos.y, atomsList[i].size, atomsList[i].size}, 
					(Vector2){atomsList[i].size/2, atomsList[i].size / 2}, 
					atomsList[i].rot, 
					(Color){255, 0, 0, 255 });
 		}
        EndDrawing();
    }


    CloseWindow();        

	DestroyAtomArray(&atomsList);

    return 0;
}
