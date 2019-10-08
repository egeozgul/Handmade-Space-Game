#include "TheGame.h"
#include "Variables.h"

GAME::GAME(CGRAPHICS* gfx, CINPUT* Inpt, double* framedur, unsigned long int* totaltime, int scw, int sch, bool* running) :graphics(gfx), input(Inpt), frameDuration(framedur), totalTime(totaltime), screenHeight(sch), screenWidth(scw), Running(running)
{
	srand(1);

	earth = new PLANET("resources\\planets\\planet4.bmp", 255, 255, 0);

	planets = new PLANET*[maxPlanets];

	planets[0] = new PLANET("resources\\planets\\planet1.bmp", 255, 255, 255);
	planets[1] = new PLANET("resources\\planets\\planet2.bmp", 255, 255, 255);
	planets[2] = new PLANET("resources\\planets\\planet3.bmp", 255, 255, 255);
	planets[3] = new PLANET("resources\\planets\\planet3.bmp", 255, 255, 255);
	planets[4] = new PLANET("resources\\planets\\planet5.bmp", 255, 255, 255);
	planets[5] = new PLANET("resources\\planets\\planet6.bmp", 255, 255, 255);
	planets[6] = new PLANET("resources\\planets\\planet7.bmp", 255, 255, 0);
	planets[7] = new PLANET("resources\\planets\\planet8.bmp", 255, 255, 0);
	planets[8] = new PLANET("resources\\planets\\planet9.bmp", 255, 255, 0);
	planets[9] = new PLANET("resources\\planets\\planet10.bmp", 255, 255, 0);
	planets[10] = new PLANET("resources\\planets\\planet11.bmp", 255, 255, 0);
	planets[11] = new PLANET("resources\\planets\\planet1.bmp", 255, 255, 0);

	planets[12] = new PLANET("resources\\planets\\planet3.bmp", 255, 255, 255);
	planets[13] = new PLANET("resources\\planets\\planet1.bmp", 255, 255, 255);
	planets[14] = new PLANET("resources\\planets\\planet5.bmp", 255, 255, 255);
	planets[15] = new PLANET("resources\\planets\\planet6.bmp", 255, 255, 255);
	planets[16] = new PLANET("resources\\planets\\planet7.bmp", 255, 255, 0);
	planets[17] = new PLANET("resources\\planets\\planet8.bmp", 255, 255, 0);
	planets[18] = new PLANET("resources\\planets\\planet9.bmp", 255, 255, 0);
	planets[19] = new PLANET("resources\\planets\\planet10.bmp", 255, 255, 0);
	planets[20] = new PLANET("resources\\planets\\planet11.bmp", 255, 255, 0);
	planets[21] = new PLANET("resources\\planets\\planet7.bmp", 255, 255, 0);
	planets[22] = new PLANET("resources\\planets\\planet3.bmp", 255, 255, 255);
	planets[23] = new PLANET("resources\\planets\\planet2.bmp", 255, 255, 255);
	planets[24] = new PLANET("resources\\planets\\planet5.bmp", 255, 255, 255);
	planets[25] = new PLANET("resources\\planets\\planet6.bmp", 255, 255, 255);
	planets[26] = new PLANET("resources\\planets\\planet7.bmp", 255, 255, 0);
	planets[27] = new PLANET("resources\\planets\\planet8.bmp", 255, 255, 0);
	planets[28] = new PLANET("resources\\planets\\planet9.bmp", 255, 255, 0);
	planets[29] = new PLANET("resources\\planets\\planet10.bmp", 255, 255, 0);
	planets[30] = new PLANET("resources\\planets\\planet11.bmp", 255, 255, 0);

	shuttle = new SHUTTLE("resources\\shuttle.bmp", "resources\\shuttleFlame.bmp", 255, 255, 0);
	
	menuPlate = new IMAGE("resources\\GUI_menus\\MENU_plate.bmp", 255, 255, 255);
	aimCursor = new IMAGE("resources\\AimCursor.bmp", 255, 255, 255);
	bitmap = new IMAGE("resources\\walpaper2.bmp", 255, 255, 255);
	text = new LETTER("resources\\asci.bmp", 16, 16, 0);

	startButton = new BUTTON(input, "resources\\GUI_menus\\STARTBUTTON", 200, 200, 255, 255, 255);
	nextLevelButton = new BUTTON(input, "resources\\GUI_menus\\NEXTLEVELBUTTON", 200, 200, 255, 255, 255);
	restartButton = new BUTTON(input, "resources\\GUI_menus\\RESTARTBUTTON", 200, 200, 255, 255, 255);
	exitButton = new BUTTON(input, "resources\\GUI_menus\\EXITBUTTON", 200, 200, 255, 255, 255);

	button_initialize();

	levelInitialize();
}

void GAME::mainUpdate()
{
	mousePos = input->Mouse.Pos;
	
	if (gameState == levelBegginingPage)
	{
		startButton->UpdateButtonState();

		if(startButton->isPushed())
			gameState = levelActivePage;
	}
	else if (gameState == levelActivePage)
	{
		//mass update
		

		//spaceshuttle acceleration update
		shuttle->accX = 0.0f;
		shuttle->accY = 0.0f;
		for (int i = 0; i < usednPlanets; i++)
		{			
			float distance = sqrtf(powf((float)(shuttle->posX - planets[i]->posX), 2) + powf((float)(shuttle->posY - planets[i]->posY), 2));

			shuttle->accX += G * planets[i]->mass * (float)(planets[i]->posX - shuttle->posX) /powf(distance,3) ;
			shuttle->accY += G * planets[i]->mass * (float)(planets[i]->posY - shuttle->posY) /powf(distance,3) ;
		}

		float distance = sqrtf(powf((float)(shuttle->posX - earth->posX), 2) + powf((float)(shuttle->posY - earth->posY), 2));

		shuttle->accX += G * earth->mass * (float)(earth->posX - shuttle->posX) / powf(distance, 3);
		shuttle->accY += G * earth->mass * (float)(earth->posY - shuttle->posY) / powf(distance, 3);

		
		if (fuelUsed < totalFuel)
		{
			if (input->Keyboard.ArrowUp)
			{
				shuttle->accX += cosf(shuttle->angle*PI / 180.0f) * thrustForce / (float)shuttle->mass;
				shuttle->accY += sinf(shuttle->angle*PI / 180.0f) * thrustForce / (float)shuttle->mass;
				fuelUsed += (float)((double)mainEngine_FuelUsageRate * *frameDuration);
			}
			
			if (input->Keyboard.ArrowRight)
			{
				shuttle->angleVel += *frameDuration * angleForce*timeScale / (float)shuttle->mass;
				fuelUsed += (double)rotaryEngine_FuelUsageRate * *frameDuration;
			}
			else if (input->Keyboard.ArrowLeft)
			{
				shuttle->angleVel -= *frameDuration * angleForce*timeScale / (float)shuttle->mass;
				fuelUsed += (double)rotaryEngine_FuelUsageRate * *frameDuration;
			}
			shuttle->mass = spacecraft_initialMass + (totalFuel - fuelUsed);
		}

		// velocity update
		shuttle->velocityX += shuttle->accX * (*frameDuration*timeScale);
		shuttle->velocityY += shuttle->accY * (*frameDuration*timeScale);

		//position update
		shuttle->posX += shuttle->velocityX * (*frameDuration*timeScale);
		shuttle->posY += shuttle->velocityY * (*frameDuration*timeScale);

		shuttle->angle += shuttle->angleVel * (*frameDuration*timeScale);

		//collision detection
		if (sqrtf(powf(shuttle->posX - earth->posX, 2) + powf(shuttle->posY - earth->posY, 2)) > maxDistance)
			gameState = lostPage;

		for (int i = 0; i < usednPlanets; i++)
		{
			if (powf(shuttle->posX - planets[i]->posX, 2) + powf(shuttle->posY - planets[i]->posY, 2) <= powf(planets[i]->radius +((shuttle->I->height+ shuttle->I->width )/4.0),2))
				gameState = lostPage;
		}
	
		if (powf((float)(shuttle->posX - earth->posX), 2) + powf((float)(shuttle->posY - earth->posY), 2) <= powf((float)earth->radius, 2))
			gameState = nextLevelPage;

	}
	else if (gameState == nextLevelPage)
	{
		nextLevelButton->UpdateButtonState();

		if(nextLevelButton->isPushed())
		{
			gameState = levelBegginingPage;
			level++;

			levelInitialize();
			Sleep(800);
		}
		
	}
	else if (gameState == wonPage)
	{
		exitButton->UpdateButtonState();

		if(exitButton->isPushed())
			*Running = false;
	}
	else if (gameState == lostPage)
	{
		restartButton->UpdateButtonState();

		if (restartButton->isPushed())
		{
			levelInitialize();
			gameState = levelBegginingPage;
		}
	}
}

void GAME::GraphicsUpdate()
{
	bitmap->DrawToBackground();

	for (int i = 0; i < usednPlanets; i++)
		planets[i]->draw();

	earth->draw();


	shuttle->draw();

	int tempx = shuttle->posX;
	int tempy = shuttle->posY;
	if (tempx < 0) tempx = 2;
	else if (tempx >= screenWidth) tempx = screenWidth-3;
	
	if (tempy < 0) tempy = 2;
	else if (tempy >= screenHeight) tempy = screenHeight - 3;

	
	if (shuttle->posX + shuttle->I->width / 2 < 0)
		graphics->DrawRect(0, tempy,6,12,255,0,0);
	else if (shuttle->posX - shuttle->I->width / 2 > screenWidth)
		graphics->DrawRect(screenWidth-1, tempy, 6, 12, 255, 0, 0);

	if (shuttle->posY + shuttle->I->height / 2 < 0)
		graphics->DrawRect(tempx, 0, 12, 6, 255, 0, 0);
	else if (shuttle->posY - shuttle->I->height / 2 > screenHeight)
		graphics->DrawRect(tempx,screenHeight-1, 12,6, 255, 0, 0);



	if (input->Keyboard.ArrowUp && (fuelUsed < totalFuel))
		shuttle->drawFlames();

	if (totalFuel <= fuelUsed) 
			fuelUsed = totalFuel;

	static float currentFPS = 1.0f / (*frameDuration);

	static int f1 = 0;
	f1++;

	if (f1 == 8)
	{
		f1 = 0;
		currentFPS = 1.0f / (*frameDuration);
	}

		text->putString(10, 450, 255, 255, 0, "     MISSION %d\n\ndistance to Earth (km):%d \nSpeed (km/s):             %d \nAcceleration (km/s^2):      %d \n\nFUEL (kg): %d\n Aircraft Mass (kg): %d \nFPS: %d",
			level + 1,
			(int)sqrtf(powf(shuttle->posX - earth->posX, 2) + powf(shuttle->posY - earth->posY, 2)) * 1000,
			(int)sqrtf(pow(shuttle->velocityX, 2) + pow(shuttle->velocityY, 2)) * 1000,
			(int)(sqrtf(pow(shuttle->accX, 2) + pow(shuttle->accY, 2))*1000.0),
			(int)(totalFuel - fuelUsed),
			(int)shuttle->mass,
			(int)currentFPS);

	if (gameState == levelBegginingPage)
	{
		menuPlate->DrawImage(manuPlate_posX, manuPlate_posY,0, manuPlate_horStretch, manuPlate_vertStretch);
		text->putString(message_posX, message_posY, 255, 255, 0, begin_Message[level]);
		startButton->drawButton();
	}
	else if (gameState == levelActivePage)
	{

	}
	else if (gameState == wonPage)
	{
		menuPlate->DrawImage(manuPlate_posX, manuPlate_posY, 0, manuPlate_horStretch, manuPlate_vertStretch);
		text->putString(message_posX ,message_posY,255,255,0, won_Message);
		exitButton->drawButton();
	}
	else if(gameState == nextLevelPage)
	{
		menuPlate->DrawImage(manuPlate_posX, manuPlate_posY, 0, manuPlate_horStretch, manuPlate_vertStretch);
		text->putString(message_posX, message_posY, 255, 255, 0, nextLevel_Message);
		nextLevelButton->drawButton();
	}
	else if (gameState == lostPage)
	{
		menuPlate->DrawImage(manuPlate_posX, manuPlate_posY, 0, manuPlate_horStretch, manuPlate_vertStretch);
		
		if (sqrtf(powf(shuttle->posX - earth->posX, 2) + powf(shuttle->posY - earth->posY, 2)) > maxDistance)
			text->putString(message_posX, message_posY, 255, 255, 0, rangeExceeded_Message );
		else 
			text->putString(message_posX, message_posY, 255, 255, 0, lost_Message);

		restartButton->drawButton();
	}

	if(gameState != levelActivePage)
		aimCursor->DrawImage(mousePos.x,  mousePos.y);
}

GAME::~GAME()
{	
	delete text ;
	delete bitmap ;

	delete aimCursor;
	delete  exitButton ;
	delete startButton ;
	delete  nextLevelButton ;
	delete restartButton ;

	for(int i = 0 ; i<maxPlanets; i++)
		delete planets[i];

	delete  shuttle ;
	delete earth;

}
