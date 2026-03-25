#include "universe.h"


void initData(){
	 mouseWheelSensivity = 10;
	 movementSpeed = 10;
	 rotationSpeed = 2.5;
	 selectionDuration = 10.0f;
	 AddKey = KEY_G;
	 selectKey = KEY_F;
	 deleteKey = KEY_R;

	 camMovementUp = KEY_W;
	 camMovementDown = KEY_S;
	 camMovementLeft = KEY_A;
	 camMovementRight = KEY_D;

	 camZoomOut = KEY_Q;
	 camZoomIn = KEY_E;

	 pauseKey = KEY_P;

	 newtonianConst = 25000;

	 paddingTop = 35;

	 atomsList = NULL;
	 atomCount = 0;

	 isPaused = false;
	 scrollActionSpeed = 0;
	 Camera2D camera = { 0 };

	 halfScreenWidth = 20;
	 halfScreenHeight = 20;

}
