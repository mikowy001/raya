#include "universe.h"

void initCamera(){
	camera.target = (Vector2){0, 0};
	camera.offset = (Vector2){GetScreenWidth(), GetScreenHeight()};
	camera.rotation = 0.0f;
	camera.zoom = 1.1f;
}
void cameraControls(){
	if(IsKeyDown(camZoomIn)){
		camera.zoom *= 1.01;
	}
	if(IsKeyDown(camZoomOut)){
		camera.zoom /= 1.01;
	}
	if(IsKeyDown(camMovementUp)){
		camera.target = Vector2Add(camera.target, (Vector2){0, -(scrollActionSpeed / 4 / camera.zoom)});
	}
	if(IsKeyDown(camMovementDown)){
		camera.target = Vector2Add(camera.target, (Vector2){0, scrollActionSpeed / 4 / camera.zoom});
	}
	if(IsKeyDown(camMovementRight)){
		camera.target = Vector2Add(camera.target, (Vector2){scrollActionSpeed / 4 / camera.zoom, 0});
	}	
	if(IsKeyDown(camMovementLeft)){
		camera.target = Vector2Add(camera.target, (Vector2){-(scrollActionSpeed / 4 / camera.zoom), 0});
	}
}
