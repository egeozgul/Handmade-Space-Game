#pragma once

#include<windows.h>
#include<Windowsx.h>
#include <stdio.h>

#include "Input.h"
#include"graphics.h"
#include "TheGame.h"

#define debugPrint(...) {			\
	char buffer[256];				\
	wsprintf(buffer, __VA_ARGS__ );	\
	OutputDebugStringA(buffer);		\
}									\

class HANDLER
{
private:
	HINSTANCE *instance;
	HINSTANCE *prevInstance; 
	LPSTR     *commandLine; 
	int       *showCode;

	HWND	   Window;
	WNDCLASS   WindowClass = {};
	HDC		   DeviceContext;
	MSG		   Message;

	static const int  BytesPerPixel;
	static BITMAPINFO BitmapInfo;
	
	static LRESULT CALLBACK Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam);
	
	void friend thread_funct1();

public:
	static GAME *TheGame;
	static CINPUT *Input;
	CGRAPHICS * Graphics;

	static int screenHeight;
	static int screenWidth;
	static bool Running;

	HANDLER(HINSTANCE* T_Instance, HINSTANCE* T_PrevInstance, LPSTR* T_CommandLine, int* T_ShowCode);
	void createWindow(const char* title, int width, int height, int posX = 10, int posY = 10);
	void checkMessage();
	~HANDLER();

	//FPS Limiter
private:
	double frameDuration;
	unsigned long int totalTime;

	LARGE_INTEGER  frameEndTime;
	LARGE_INTEGER  frameBeginTime;

	LARGE_INTEGER  frameDur;
	LARGE_INTEGER  frequency;
public:
	unsigned long int time_s;
	double duration_s;

	void fixedFPSInit();
	void fixedFPS(double targetFPS);
};
