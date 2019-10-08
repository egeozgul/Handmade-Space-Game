#include "game.h"

HANDLER *gameHandler = NULL;

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode)
{
	gameHandler = new HANDLER(&Instance, &PrevInstance, &CommandLine, &ShowCode);
	
	gameHandler->createWindow("Ege's window",1300, 650);

	gameHandler->fixedFPSInit();
	gameHandler->Graphics->allocateBuffer();

	while (gameHandler->Running)
	{
		gameHandler->fixedFPS(70);

		gameHandler->checkMessage();

		//game update
		gameHandler->TheGame->mainUpdate();

		//display stuff
		gameHandler->TheGame->GraphicsUpdate();
		gameHandler->Graphics->copyBufferToClientArea();
	}

	delete gameHandler;
	return 0;
}

