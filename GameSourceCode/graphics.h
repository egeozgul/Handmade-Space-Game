#pragma once
#include<windows.h>
#include<Windowsx.h>
#include<stdint.h>
#include <math.h>
#include<stdio.h>
#include <assert.h>
#include <algorithm>
#include <string>
#include <cstdarg>
#include <ctime>
#include <sstream>
#include <fstream>
#include <vector>

#include "Input.h"

#define fromTo(var,f,l) for(int var = f;var<=l; var++)
#define fromToBy(var,f,l,by) for(int var = f;var<=l; var+= by)

#define debugPrint(...) {			\
	char buffer[256];				\
	wsprintf(buffer, __VA_ARGS__ );	\
	OutputDebugStringA(buffer);		\
}									\


class CGRAPHICS
{
private:
	HDC		   *DeviceContext;
	const int  *BytesPerPixel;
	const BITMAPINFO *BitmapInfo;

public:
	struct Pixel24
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
	};

	struct Pixel32
	{
		unsigned char blue;
		unsigned char green;
		unsigned char red;
		unsigned char alpha = 255;
	};

	union mem
	{
		uint32_t* Int = NULL;
		void* Void;
	};
	static  mem PixelMemory;

	static int* screenWidth;
	static int *screenHeight;
	static double *frameDuration;
	static unsigned long int *totalTime;

	CGRAPHICS(int *sWidth, int* sHeight, HDC *DeviceContx, const int* bytesPerPixel, const BITMAPINFO* bitmapInfo , double* frameDuration_ss, unsigned long int* totalTime_ss)
	{
		//PixelMemory.Void = pixMemory;
		frameDuration = frameDuration_ss;
		totalTime = totalTime_ss;
		screenHeight = sHeight;
		screenWidth  = sWidth;

		DeviceContext =  DeviceContx;
		BytesPerPixel = bytesPerPixel;
		BitmapInfo = bitmapInfo;
	}

	void allocateBuffer()
	{
		PixelMemory.Void = VirtualAlloc(0, *screenWidth * *screenHeight * *BytesPerPixel, MEM_COMMIT, PAGE_READWRITE);
	}

	void copyBufferToClientArea()
	{
		StretchDIBits(*DeviceContext,
			0, 0, *screenWidth, *screenHeight,
			0, 0, *screenWidth, *screenHeight,
			PixelMemory.Int, BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		//VirtualFree(PixelMemory.Void, 0, MEM_RELEASE);
	}

	~CGRAPHICS()
	{
		VirtualFree(PixelMemory.Void, 0, MEM_RELEASE);
	}

private:
	
	bool chechPixelBoundary = false;
public:

	static void PutPixel(int x, int y, int r = 255, int g = 255, int b = 255);

	void DrawRect(int x1, int y1, int width, int height, int r = 255, int g = 255, int b = 255);
	
	class IMAGE_S;

	class LETTER_S;

	class BUTTON_S;
};


#define IMAGE CGRAPHICS::IMAGE_S
#define LETTER CGRAPHICS::LETTER_S
#define BUTTON CGRAPHICS::BUTTON_S


class CGRAPHICS::IMAGE_S
{
private:

	struct BitmapFileHeader
	{
		unsigned int fileSize;
		unsigned short reserved1, reserved2;
		unsigned int offsetToPixelData;
	};

	struct BitmapInfoHeader
	{
		unsigned int headerSize;
		int width, height;
		unsigned short planes;
		unsigned short bits;
		unsigned int compression;
		unsigned int imageSize;
		int xResolution, yResolution;
		unsigned int nColors;
		unsigned int importantColours;
	};

	
public:
	Pixel32* imageBuffer = NULL;
	int height;
	int width;
	int exR, exB, exG;
	float valAngle = 0.0f;
	float valStretch_X = 1.0f;
	float valStretch_Y = 1.0f;
	int pos_X = 0;
	int pos_Y = 0;
	int rotationPivotX;
	int rotationPivotY;


	IMAGE_S(const char *filename,int excludedR, int excludedG, int excludedB);

	void DrawImage(int x, int y);

	void DrawImage(int x, int y, float angel, volatile float stretchx, volatile float stretchy);

	void DrawImageFast(int x, int y);

	void DrawToBackground();

	~IMAGE_S();
};

class CGRAPHICS::LETTER_S
{
private:
	IMAGE_S* fontmap = NULL;
	char firstChar = 0;
	int nCharacters = 0;
	int fontSize = 12;

	class CHARMAP
	{
	public:
		Pixel32 **bitmap;
		int  sizeX = 0, sizeY = 0;

		void alloc(int sizeXs, int sizeYs)
		{
			sizeX = sizeXs;
			sizeY = sizeYs;

			bitmap = new Pixel32*[sizeX];

			fromTo(i1,0,sizeX-1)
				bitmap[i1] = new Pixel32[sizeY];
		}

		~CHARMAP()
		{
			fromTo(i,0, sizeX-1)
				delete[] bitmap[i];

			delete []bitmap;			
		}

	}*CharMap = NULL;

	void putChar(int c, int posX, int posY, int r, int g, int b);

public:
	int xAdjustmentOffset = -6;
	int yAdjustmentOffset = 0;

	LETTER_S(const char* path, int nCharHorizontal, int nCharVertical, char firstChar);	

	void putString(int posX, int posY, int r, int g, int b, const char* line, ...);

	~LETTER_S();
};

class CGRAPHICS::BUTTON_S
{
private:
	std::vector<IMAGE_S*> image;
	
	CINPUT *Binput = NULL;

	int nImage = 0;
	int buttonState = 0;

	void  loadButtons(const char* file)
	{
		nImage = 0;
		std::string path;
		//"dir resources\\balls /B /A:-D | find /v \"\" /c"
		path = "dir ";
		path.append(file);
		path.append("/B /A:-D | find /v \"\" /c");

		//get the number of files in the folder 
		FILE *fpipe = _popen(path.c_str(), "r");

		for (char c = 0; fread(&c, sizeof(c), 1, fpipe);)
		{
			if (c == 10)break;

			nImage *= 10;
			nImage += c - '0';
		}
		_pclose(fpipe);

		//get the names of all files

		path = "dir ";
		path.append(file);
		path.append(" /B /A:-D");

		std::string *folder = new std::string[nImage];

		fpipe = _popen(path.c_str(), "r");//"dir resources\\balls /B /A:-D"
		char c = 0;
		for (int i = 0; i < nImage; i++)
		{
			while (fread(&c, sizeof(c), 1, fpipe))
			{

				if (c == '\n')break;

				folder[i].push_back(c);
			}
		}
		_pclose(fpipe);

		image.resize(nImage);

		std::string s = file;
		s.push_back('\\');
		for (int i = 0; i < nImage; i++)
		{
			folder[i] = s + folder[i];
			//BitmapFile t;
			//debugPrint(folder[i].c_str());
			//t.Load(folder[i].c_str());
			//((image[i].Load(folder[i].c_str());
			image[i] = new CGRAPHICS::IMAGE_S(folder[i].c_str(), exR, exG, exB);

			//image.push_back(t);
		}

	}

public:
	int exR ,exG ,exB;

	int posX;
	int posY;
	int width;
	int height;
	float xStretch = 1;
	float yStretch = 1;


	BUTTON_S(CINPUT *inpt,const char* FilePath, int posx, int posy, int exr = -1, int exg = -1, int exb = -1)
	{
		Binput = inpt;
		exR = exr;
		exG = exg;
		exB = exb;

		posX = posx;
		posY = posy;
		buttonState = 0;

		loadButtons(FilePath);

		width = image[0]->width;
		height = image[0]->height;
	}

	~BUTTON_S()
	{
		image.clear();
	}

	int UpdateButtonState()
	{
		if (Binput->Mouse.Pos.x >= posX - width * xStretch / 2 && Binput->Mouse.Pos.x <= posX + width* xStretch / 2 &&
			Binput->Mouse.Pos.y >= posY - height * yStretch / 2 && Binput->Mouse.Pos.y <= posY + height* yStretch / 2)
		{
			if (Binput->Mouse.LeftButton)
				buttonState = 2;
			else
				buttonState = 1;
		}
		else
			buttonState = 0;
		return buttonState;
	}
	
	void drawButton()
	{
		if (buttonState == 0)
			image[0]->DrawImage(posX, posY , 0, xStretch, yStretch);

		else if (buttonState == 1)
			image[1]->DrawImage(posX, posY, 0, xStretch, yStretch);
		
		else if (nImage >= 3 && buttonState == 2)
			image[2]->DrawImage(posX, posY, 0, xStretch, yStretch);
	}

	bool isPushed()
	{
		if (buttonState == 2) return true;
		return false;
	}

	bool isOnHowered()
	{
		if (buttonState == 1) return true;
		return false;
	}
};