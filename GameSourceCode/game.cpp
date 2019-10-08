#include"game.h"
using namespace std;

//external definitions of the variables that are previously defined as static : this is needed
bool HANDLER::Running;
const int HANDLER::BytesPerPixel = 4;
int HANDLER::screenHeight;
int HANDLER::screenWidth;
BITMAPINFO HANDLER::BitmapInfo;

CINPUT *HANDLER::Input = NULL;
GAME *HANDLER::TheGame = NULL;

//external definitions end

LRESULT CALLBACK HANDLER::Win32MainWindowCallback(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
	case WM_SIZE:
	{
		BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
		BitmapInfo.bmiHeader.biWidth = screenWidth;
		BitmapInfo.bmiHeader.biHeight = screenHeight;
		BitmapInfo.bmiHeader.biPlanes = 1;
		BitmapInfo.bmiHeader.biBitCount = 32;
		BitmapInfo.bmiHeader.biCompression = BI_RGB;

		int BitmapMemorySize = screenWidth*screenHeight* BytesPerPixel;
	}break;

	case WM_CLOSE:
	{
		Running = false;
	}break;

	case WM_DESTROY:
	{
		Running = false;
	}break;

	default:
	{
		 Input->GetMessages(Message, WParam, LParam);
		
		Result = DefWindowProc(Window, Message, WParam, LParam);
	}
	}
	return (Result);
}


void HANDLER::createWindow(const char* title, int width, int height, int posX, int posY)
{
	screenWidth = width;
	screenHeight = height;

	//window class creation
	Window = CreateWindowExA(0, WindowClass.lpszClassName, title,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		posX, posY, width + 16, height + 39, 0, 0, *instance, 0);

	ShowCursor(false);

	//get the window information
	DeviceContext = GetDC(Window);

	return;
}

void HANDLER::checkMessage()
{
	while (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
	{
		if (Message.message == WM_QUIT || Message.message == WM_DESTROY || Message.message == WM_CLOSE)
		{
			Running = false;
			break;
		}

		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
}

void HANDLER::fixedFPS(double targetFPS)
{
	QueryPerformanceCounter(&frameEndTime);

	frameDur.QuadPart = frameEndTime.QuadPart - frameBeginTime.QuadPart;

	frameDur.QuadPart *= 1000000;
	frameDur.QuadPart /= frequency.QuadPart;

	frameDuration = (double)(frameDur.QuadPart) / 1000000.0;

	//begin
	QueryPerformanceCounter(&frameBeginTime);

	totalTime = frameBeginTime.QuadPart * 1000000;
	totalTime /= frequency.QuadPart;

	long delayDuration = (1000.0 / targetFPS - frameDur.QuadPart / 1000.0);

	if (delayDuration > 0)
		Sleep(delayDuration);
}

void HANDLER::fixedFPSInit()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&frameBeginTime);
}

HANDLER::HANDLER(HINSTANCE* T_Instance, HINSTANCE* T_PrevInstance, LPSTR* T_CommandLine, int* T_ShowCode) : instance(T_Instance), prevInstance(T_PrevInstance), commandLine(T_CommandLine), showCode(T_ShowCode)
{
	screenWidth = 1300;
	screenHeight = 650;

	Running = true;

	//window class information filling before creation
	WindowClass.lpfnWndProc = Win32MainWindowCallback;
	WindowClass.hInstance = *instance;
	WindowClass.lpszClassName = "My Class";

	RegisterClassA(&WindowClass);

	Input = new CINPUT();
	Graphics = new CGRAPHICS(&screenWidth, &screenHeight, &DeviceContext, &BytesPerPixel, &BitmapInfo, &frameDuration, &totalTime);

	TheGame = new GAME(Graphics, Input, &frameDuration, &totalTime, screenWidth, screenHeight, &Running);
}


HANDLER::~HANDLER()
{
	ReleaseDC(Window, DeviceContext);
	delete TheGame;
	delete Input;
	delete Graphics;
}
