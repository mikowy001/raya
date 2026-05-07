#include "./functions.h"
#include "./coredata.h"

void initCamera(){
	DATA.camera.target = (Vector2){0, 0};
	DATA.camera.offset = (Vector2){GetScreenWidth(), GetScreenHeight()};
	DATA.camera.rotation = 0.0f;
	DATA.camera.zoom = 1.1f;
}
void cameraControls(){
	if(IsKeyDown(DATA.Keybinds.camZoomIn)){
		DATA.camera.zoom *= 1.01;
	}
	if(IsKeyDown(DATA.Keybinds.camZoomOut)){
		DATA.camera.zoom /= 1.01;
	}
	if(IsKeyDown(DATA.Keybinds.camMovementUp)){
		DATA.camera.target = Vector2Add(DATA.camera.target, (Vector2){0, -(DATA.Settings.scrollActionSpeed / 4 / DATA.camera.zoom)});
	}
	if(IsKeyDown(DATA.Keybinds.camMovementDown)){
		DATA.camera.target = Vector2Add(DATA.camera.target, (Vector2){0, DATA.Settings.scrollActionSpeed / 4 / DATA.camera.zoom});
	}
	if(IsKeyDown(DATA.Keybinds.camMovementRight)){
		DATA.camera.target = Vector2Add(DATA.camera.target, (Vector2){DATA.Settings.scrollActionSpeed / 4 / DATA.camera.zoom, 0});
	}	
	if(IsKeyDown(DATA.Keybinds.camMovementLeft)){
		DATA.camera.target = Vector2Add(DATA.camera.target, (Vector2){-(DATA.Settings.scrollActionSpeed / 4 / DATA.camera.zoom), 0});
	}
}
