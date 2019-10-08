#pragma once

#include "graphics.h"
#include "Input.h"
#include <math.h>
#define PI 3.14159265359
#define randomNumber(a,b) rand()%(b-a) +a;

enum GAMESTATE
{
	levelBegginingPage, levelActivePage, nextLevelPage, wonPage, lostPage
};

class SHUTTLE
{
	IMAGE* flame = NULL;
public:
	IMAGE* I = NULL;
	int radius;
	double mass;
	float angle = 0.0;
	float angleVel = 0.0;

	float posX, posY;
	float velocityX = 0, velocityY = 0;
	float accX = 0, accY = 0;

	int vectorPointX, vectorPointY;
	int pointRadius = 5;

	SHUTTLE(const char* path, const char* path2, int exr, int exg, int exb)
	{
		I = new IMAGE(path, exr, exg, exb);
		flame = new IMAGE(path2, exr, exg, exb);

		radius = 0.5f + I->height / 2;

		mass = radius*radius*PI;
	}

	void draw()
	{
		I->DrawImage(posX, posY, angle + 90, 1, 1);
	}

	void drawFlames()
	{
		flame->DrawImage(posX ,posY ,angle + 90.0f, 1, 1);
	}

	~SHUTTLE()
	{
		delete I;
	}
};

class PLANET
{

public:
	IMAGE* I = NULL;

	int radius;
	double mass;
	int angle = 0;
	int posX = 0, posY = 0;

	PLANET()
	{

	}

	PLANET(const char* path, int exr, int exg, int exb)
	{
		I = new IMAGE(path, exr, exg, exb);
		radius = 0.5f + I->height / 2;

		mass = radius*radius*PI;
	}

	void draw()
	{
		I->DrawImage(posX, posY, angle, (float)radius*2.0f / (float)I->height, (float)radius*2.0f / (float)I->width);
	}

	~PLANET()
	{
		I->~IMAGE_S();
	}
};

class GAME
{
private://window variables
	CGRAPHICS* graphics = NULL;
	CINPUT* input = NULL;
	bool* Running = NULL;

	double* frameDuration = 0;
	unsigned long int* totalTime = 0;
	int screenHeight, screenWidth;

public:
	GAME(CGRAPHICS* gfx, CINPUT* Inpt, double* framedur, unsigned long int* totaltime, int scw, int sch, bool* running);
	void levelInitialize();
	void mainUpdate();
	void GraphicsUpdate();
	~GAME();
	
private: // game variables
	GAMESTATE gameState;

	LETTER * text = NULL;

	IMAGE* aimCursor = NULL;
	IMAGE*	menuPlate = NULL;
	IMAGE *bitmap = NULL;
	
	BUTTON* exitButton = NULL;
	BUTTON *startButton = NULL;
	BUTTON* nextLevelButton = NULL;
	BUTTON* restartButton = NULL;

	int maxPlanets = 30;
	int usednPlanets = 0;
	PLANET **planets = NULL;
	SHUTTLE *shuttle = NULL;
	PLANET *earth = NULL;

	int level = 0;
	VECTOR2 mousePos;
};
