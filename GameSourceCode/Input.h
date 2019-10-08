#pragma once

#include<windows.h>
#include<Windowsx.h>
#include<math.h>

#define KeyDown true
#define KeyUp false

struct VECTOR2
{
	int x;
	int y;
};

class CINPUT
{
private:
	bool inputBuffer[219] = {};
	
	struct MOUSE
	{
		bool isMoved = false;
		VECTOR2 Pos = {0,0};
		bool MiddleButton = false;
		bool LeftButton = false;
		bool RightButton = false;
		bool IsInWindow = true;
		int Wheel_Delta_Pos = 0;
	};

	struct KEYBOARD
	{
		bool IsPressed(char key);
		bool Enter = false;
		bool Shift = false;
		bool Ctrl = false;
		bool Space = false;
		bool Esc = false;

		bool ArrowLeft = false;
		bool ArrowRight = false;
		bool ArrowDown = false;
		bool ArrowUp = false;
	};

public:
	char lastPressed = 0;
	bool IsKbHit = false;
	MOUSE Mouse;
	KEYBOARD Keyboard;

	void GetMessages(UINT data, WPARAM wParam, LPARAM LParam)
	{
		Mouse.isMoved = false;
		switch (data)
		{

			case WM_KEYDOWN:
			{
				IsKbHit = true;
				inputBuffer[wParam] = KeyDown;
				lastPressed = wParam;
				switch (wParam)
				{
					case VK_RETURN:
					{
						Keyboard.Enter = true;
					}break;
					case VK_SHIFT:
					{
						Keyboard.Shift = true;
					}break;
					case VK_CONTROL:
					{
						Keyboard.Ctrl = true;
					}break;
					
					case VK_ESCAPE:
					{
						Keyboard.Esc = true;
					}break;
					case VK_SPACE:
					{
						Keyboard.Space = true;
					}break;
					case VK_LEFT:
					{
						Keyboard.ArrowLeft = true;
					}break;
					case VK_RIGHT:
					{
						Keyboard.ArrowRight = true;
					}break;
					case VK_UP:
					{
						Keyboard.ArrowUp = true;
					}break;
					case VK_DOWN:
					{
						Keyboard.ArrowDown = true;
					}break;
				}
			}break;

			case WM_KEYUP:
			{IsKbHit = false;
				inputBuffer[wParam] = KeyUp;

				switch (wParam)
				{
					case VK_RETURN:
					{
						Keyboard.Enter = false;
					}break;
					case VK_SHIFT:
					{
						Keyboard.Shift = false;
					}break;
					case VK_CONTROL:
					{
						Keyboard.Ctrl = false;
					}break;
					
					case VK_ESCAPE:
					{
						Keyboard.Esc = false;
					}break;
					case VK_SPACE:
					{
						Keyboard.Space = false;
					}break;
					case VK_LEFT:
					{
						Keyboard.ArrowLeft = false;
					}break;
					case VK_RIGHT:
					{
						Keyboard.ArrowRight = false;
					}break;
					case VK_UP:
					{
						Keyboard.ArrowUp = false;
					}break;
					case VK_DOWN:
					{
						Keyboard.ArrowDown = false;
					}break;
				}
			}

			case WM_MOUSEMOVE:
			{
				Mouse.isMoved = true;
				Mouse.Pos.x = GET_X_LPARAM(LParam);
				Mouse.Pos.y = GET_Y_LPARAM(LParam);
			}break;

			case WM_MOUSEHWHEEL:
			{
				Mouse.Wheel_Delta_Pos = GET_WHEEL_DELTA_WPARAM(wParam);			
			}break;

			case WM_MOUSELEAVE:
			{
				Mouse.IsInWindow = false;
			}break;

			case WM_MOUSEACTIVATE:
			{
				Mouse.IsInWindow = true;
			}break;

			case	WM_LBUTTONDOWN://	Left button down
			{
				Mouse.LeftButton = true;
			}break;
			case	WM_LBUTTONUP:	//Left button up
			{
				Mouse.LeftButton = false;
			}break;
			case	WM_MBUTTONDOWN://	Middle button down
			{
				Mouse.MiddleButton = true;
			}break;
			case	WM_MBUTTONUP:	//Middle button up
			{
				Mouse.MiddleButton = false;
			}break;
			case	WM_RBUTTONDOWN:	//Right button down
			{
				Mouse.RightButton = true;
			}break;
			case	WM_RBUTTONUP:	//Right button up
			{
				Mouse.RightButton = false;
			}break;

			default:
			{

			}
		}
	}

	bool IsPressed(char key)
	{
		if (inputBuffer[key - ' '] == true)
			return true;

		return false;
	}
};