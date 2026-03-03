#include "raylib.h"

#include <stdlib.h>         
#include <math.h>           
#include <vector>

struct atom {
	Vector2 pos;
	Vector2 force;
	int charge;
    int rot;
	int size;
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
	
	
	//float size = 25;
	float movementSpeed = 1.5;
	float rotationSpeed = 2.5;
	
	//atom atomsList[] = HERE DO AN RESIZABLE ARRAY OF "atom" 

	bool isPaused = false;
	double startTime = GetTime();
	double pausedTime = 0.0;
	double totalPausedDuration = 0.0;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "swietlista biblioteka");
	
	Texture2D pauseIcon = LoadTexture("resources/pauseIcon.png");  // i must load it after initwindow cuz it needs openGl contdxt

    SetTargetFPS(60);
	
    while (!WindowShouldClose())   
    {	


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
		if(isPaused){
			elapsedTime = pausedTime - startTime - totalPausedDuration;
		}
		else {
			elapsedTime = GetTime() - startTime - totalPausedDuration;
		}
		

		if(!isPaused) {

			if (IsKeyDown(KEY_W)) y = y - movementSpeed;
			if (IsKeyDown(KEY_S)) y = y + movementSpeed;
			if (IsKeyDown(KEY_A)) x = x - movementSpeed;
			if (IsKeyDown(KEY_D)) x = x + movementSpeed;

			if (IsKeyDown(KEY_Q)) r = r - rotationSpeed;
			if (IsKeyDown(KEY_E)) r = r + rotationSpeed;
		}

   	    BeginDrawing();

        ClearBackground(BLACK);

		DrawText(TextFormat("Simulation time %.1f", elapsedTime), 25, 25, 16, WHITE);
		DrawText(TextFormat("dt: %.3f ms", GetFrameTime() * 1000), 25, 50, 16, WHITE);
		
		if(isPaused){
			DrawText("Paused!        Press P to unpause.", 225, 25, 16, RED);
		}

        DrawRectanglePro((Rectangle){x, y, size, size}, (Vector2){size/2, size / 2}, r, (Color){255, 0, 0, 255 });
 
        EndDrawing();
		
		
    }

    UnloadTexture(pauseIcon);

    CloseWindow();        

    return 0;
}
