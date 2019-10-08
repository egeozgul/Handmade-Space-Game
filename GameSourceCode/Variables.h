#pragma once


#define manuPlate_posX screenWidth / 5.0
#define manuPlate_posY screenHeight*12 / 16.0
#define manuPlate_horStretch 1
#define manuPlate_vertStretch 1

//********mission message************
#define message_posX screenWidth *1.0/ 32.0
#define message_posY screenHeight*9.5 / 16.0


#define lost_Message			"You'd better stay in Earth"
#define won_Message				"well done!! next step is to buy a\n secondhand space shuttle :) "
#define nextLevel_Message		"MISSION ACCOPMLISHED!"
#define rangeExceeded_Message	"you spaceship is lost"

#define button_initialize()							\
{													\
	startButton->xStretch = 0.7f;					\
	startButton->yStretch = 0.7f;					\
	startButton->posX = screenWidth *1.8/ 7;		\
	startButton->posY = screenHeight *13.5/ 16;		\
													\
	nextLevelButton->xStretch = 0.7f;				\
	nextLevelButton->yStretch =  0.7f;				\
	nextLevelButton->posX = screenWidth  *1.8/ 7;	\
	nextLevelButton->posY = screenHeight*13.5/ 16;	\
													\
													\
	restartButton->xStretch =  0.7f;				\
	restartButton->yStretch =  0.7f;				\
	restartButton->posX = screenWidth  *1.8/ 7;		\
	restartButton->posY = screenHeight*13.5/ 16;	\
													\
	exitButton->xStretch =  0.7f;					\
	exitButton->yStretch =  0.7f;					\
	exitButton->posX = screenWidth *1.8/ 7;			\
	exitButton->posY = screenHeight*13.5/ 16;		\
}													\



float timeScale = 1;

const float G = 350.0f;
float thrustForce =  2000000.0;
float  angleForce =  1500000.0;

#define maxDistance 1000 //distance from earth that spaceship is lost if exceeded


#define mainEngine_FuelUsageRate 1000.0f
#define rotaryEngine_FuelUsageRate 10.0f
#define spacecraft_initialMass 22700

float fuelUsed = 0.0f;
float totalFuel = 729006.0f; 

char *begin_Message[10];

void GAME::levelInitialize()
{
		shuttle->angle = 0.0;
		shuttle->angleVel = 0.0;

		shuttle->velocityX = 0.0;
		shuttle->velocityY = 0.0;

		shuttle->posX = screenWidth *1.5 / 32.0;
		shuttle->posY = screenHeight *7.0 / 16.0;

		if (level == 0)
		{
			begin_Message[0] = "MISSION 1: Take the spaceship to Earth!\n\nThe mission is completed and spaceships's \njob is done, but the samples have to be \ndelivered to Earth for further study. \nHowever, the spacecraft is not equiped for \nsafe landing to Earth, thus your job is to \nbring the spaceship back to Earth. \nThe spaceship will collapse int the \nathmosphere, but the sample module will \nland safely, and \ncollected.";
			begin_Message[1] = "MISSION 2: The communicaiton with the spaceship \nis so weak due to miculculated blocking \nplanets. The mission is cancelled and your\n duty is to get back to earth immediately \nwith the limited fuel.";
			begin_Message[2] = "MISSION 3: There is a subsystem failure in the \nECLSS pressurization control system. The The Backup \nFlight System is activated, however the \nmain problem is not found. Get back to Earth.\n The mission is cancelled!";
			begin_Message[3] = " ";


			fuelUsed = 720006.0f;
			
			//*********earth**********
			earth->posX = screenWidth *8 / 12;
			earth->posY = screenHeight * 1 / 6;
			earth->radius = 50;


			//******* planets*********
			usednPlanets = 3;

			planets[0]->posX = screenWidth * 1.5 / 5;
			planets[0]->posY = screenHeight * 1 / 5;
			planets[0]->radius = 32;

			planets[1]->posX = screenWidth * 11 / 12;
			planets[1]->posY = screenHeight * 3 / 6;
			planets[1]->radius = 23;

			planets[2]->posX = screenWidth * 8 / 12;
			planets[2]->posY = screenHeight * 5 / 6;
			planets[2]->radius = 30;
		}
		else if (level == 1)
		{

			fuelUsed = 723006.0f;

			//*********earth**********
			earth->posX = screenWidth *6/ 8;
			earth->posY = screenHeight *7/8;
			earth->radius = 50;


			//******* planets*********
			usednPlanets = 5;

			planets[0]->posX = screenWidth * 7 / 8;
			planets[0]->posY = screenHeight * 1 / 9;
			planets[0]->radius = 32;

			planets[1]->posX = screenWidth * 4 / 8;
			planets[1]->posY = screenHeight * 1 / 8;
			planets[1]->radius = 32;

			planets[2]->posX = screenWidth * 6 / 8;
			planets[2]->posY = screenHeight * 3 / 8;
			planets[2]->radius = 32;

			planets[3]->posX = screenWidth * 4 / 8;
			planets[3]->posY = screenHeight * 6 / 8;
			planets[3]->radius = 32;

			planets[4]->posX = screenWidth * 2 / 8;
			planets[4]->posY = screenHeight * 6 / 8;
			planets[4]->radius = 32;
		}
		else if (level == 2)
		{
			fuelUsed = 720006.0f;

			//*********earth**********
			earth->posX = screenWidth * 6 / 8;
			earth->posY = screenHeight * 7 / 8;
			earth->radius = 50;


			thrustForce = 4500000.0;
			angleForce = 3000000.0;



			//******* planets*********
			usednPlanets = 5;

			planets[0]->posX = screenWidth * 2 / 8;
			planets[0]->posY = screenHeight * 1 / 9;
			planets[0]->radius = 12;

			planets[1]->posX = screenWidth * 1 / 16;
			planets[1]->posY = screenHeight * 7 / 8;
			planets[1]->radius = 32;

			planets[2]->posX = screenWidth * 2 / 8;
			planets[2]->posY = screenHeight * 5 / 8;
			planets[2]->radius = 32;

			planets[3]->posX = screenWidth * 3 / 8;
			planets[3]->posY = screenHeight * 1 / 8;
			planets[3]->radius = 32;

			planets[4]->posX = screenWidth * 7.5 / 8;
			planets[4]->posY = screenHeight * 3 / 8;
			planets[4]->radius = 24;
		
		}
		else if (level == 3)
		{
			fuelUsed = 720006.0f;

			//*********earth**********
			earth->posX = screenWidth * 6 / 8;
			earth->posY = screenHeight * 7 / 8;
			earth->radius = 50;


			thrustForce = 4500000.0;
			angleForce = 3000000.0;



			//******* planets*********
			usednPlanets = 5;

			planets[0]->posX = screenWidth * 2 / 8;
			planets[0]->posY = screenHeight * 1 / 9;
			planets[0]->radius = 12;

			planets[1]->posX = screenWidth * 1 / 8;
			planets[1]->posY = screenHeight * 8 / 8;
			planets[1]->radius = 27;

			planets[2]->posX = screenWidth * 2 / 8;
			planets[2]->posY = screenHeight * 5 / 8;
			planets[2]->radius = 32;

			planets[3]->posX = screenWidth * 3 / 8;
			planets[3]->posY = screenHeight * 1 / 8;
			planets[3]->radius = 32;

			planets[4]->posX = screenWidth * 7 / 16;
			planets[4]->posY = screenHeight * 1.5 / 8;
			planets[4]->radius = 26;
		}
		else
		{
			gameState = wonPage;
		}

		shuttle->mass = spacecraft_initialMass + (totalFuel - fuelUsed);

		for (int i = 0; i <maxPlanets; i++)
			planets[i]->mass = pow(planets[i]->radius,2)*PI;

		earth->mass = earth->radius*earth->radius*PI;

}





