#include"graphics.h"
using namespace std;

CGRAPHICS::mem CGRAPHICS::PixelMemory;
int* CGRAPHICS:: screenWidth;
int* CGRAPHICS:: screenHeight;
double* CGRAPHICS::frameDuration = NULL;
unsigned long int *CGRAPHICS::totalTime = NULL;

void CGRAPHICS::PutPixel(int x, int y, int r, int g , int b)
{
	PixelMemory.Int[x + *screenWidth * (*screenHeight - y - 1)] = (b | (g << 8) | (r << 16));
}

void CGRAPHICS::DrawRect(int x1, int y1, int width, int height, int r , int g , int b )
{
	width = (int)(width / 2 + 0.5f);
	height = (int)(height / 2 + 0.5f);

	for (int ty = y1 - height; ty <= y1 + height; ty++)
	{
		if (ty < 0 || ty >= *screenHeight) continue;
		for (int tx = x1 - width; tx <= x1 + width; tx++)
		{
			if (tx < 0 || tx >= *screenWidth) continue;
			PutPixel(tx, ty, r, g, b);
		}
	}
}

#define INDEX(x, y, width) (((float)y)*((float)width)+(float)x)

CGRAPHICS::IMAGE_S::IMAGE_S(const char *filename,int excludedR = -1, int excludedG = -1, int excludedB = -1)
{
	FILE* bmpFile;
	fopen_s(&bmpFile, filename, "rb");

	char signature[2];
	fread(signature, sizeof(char), 2, bmpFile);
	assert(signature[0] == 'B' && signature[1] == 'M');

	BitmapFileHeader fileHeader;
	fread(&fileHeader, sizeof(fileHeader), 1, bmpFile);

	BitmapInfoHeader infoHeader;
	fread(&infoHeader, sizeof(infoHeader), 1, bmpFile);

	fseek(bmpFile, fileHeader.offsetToPixelData, SEEK_SET);

	height = infoHeader.height;
	width = infoHeader.width;


	imageBuffer = new Pixel32[height*width];

	int nPaddingBytesPerRow = (4 - ((width * 3) % 4)) % 4;

	
	for (int y = 0 ; y < height; y++)
	{
		Pixel32 pixel;
		for (int x = 0; x < width; x++)
		{
			fread(&pixel, sizeof(Pixel24), 1, bmpFile);
			imageBuffer[y*(width)+x] = pixel;
		}
		fseek(bmpFile, nPaddingBytesPerRow, SEEK_CUR);
	}

	fclose(bmpFile);

	exR = excludedR;
	exG = excludedG;
	exB = excludedB;

	rotationPivotX = 0.5f + width / 2;
	rotationPivotY = 0.5f + height / 2;
	return;
}

//#define m2 (int)((height - 1.0f - (float)ty/((float)stretchy) )*width/stretchx + (float)tx /( (float)stretchx))
#define m (int)((height - ty - 1)*(width) + tx )

void CGRAPHICS::IMAGE_S::DrawImage(int x, int y)
{
	x = (int)((float)x - 0.5f - (float)width / 2.0f);
	y = (int)((float)y - 0.5f - (float)height / 2.0f);

	if(exR == -1 || exG == -1 || exB == -1)
		for (int ty = 0; ty <height ;ty++)
		{
			if (y < -ty || y + ty >= *screenHeight) continue;

			for (int tx = 0; tx < width; tx++)
			{
				PutPixel(x + tx, y + ty, imageBuffer[m].red, imageBuffer[m].green, imageBuffer[m].blue);
			}
		}
	else
		for (int ty = 0; ty <height; ty++)
		{
			if (y < -ty || y + ty >= *screenHeight ) continue;

			for (int tx = 0; tx < width; tx++)
			{
				if (x < -tx || x + tx >= *screenWidth) continue;

				int r = imageBuffer[m].red;
				int g = imageBuffer[m].green;
				int b = imageBuffer[m].blue;

				if (r == exR && g == exG && b == exB)
					continue;

				PutPixel(x + tx, y + ty, r, g, b);
			}
		}
}



#define index(x,y) ((height - y- 1)*width + x)
void CGRAPHICS::IMAGE_S::DrawImage(int x, int y, float angel, volatile float stretchx, volatile float stretchy)
{
	 x = (int)((float)x - (0.5f + (float)width * stretchx / 2.0f));
	y = (int)((float)y - (0.5f + (float)height *stretchy / 2.0f));

	float cosine = cos(angel * 3.1415926 / 180.0);
	float sine = sin(angel * 3.1415926 / 180.0);

	if (exR == -1 || exG == -1 || exB == -1)
	{
		if (angel == 0)
		for (int ty = 0; ty < height * stretchy; ty++)
		{

			if (y < -ty || y + ty >= *screenHeight) continue;
			for (int tx = 0; tx < width*stretchx; tx++)
			{

				if (x < -tx || x + tx >= *screenWidth) continue;

				volatile int index3 = index((int)((float)tx / stretchx), (int)((float)ty / stretchy));

				PutPixel(x + tx, y + ty, imageBuffer[index3].red, imageBuffer[index3].green, imageBuffer[index3].blue);
			}
		}
		else
			for (int ty = 0; ty < height * stretchy; ty++)
			{
				for (int tx = 0; tx < width*stretchx; tx++)
				{
					volatile int index4 = index((int)((float)tx / stretchx), (int)((float)ty / stretchy));

					int r = imageBuffer[index4].red;
					int g = imageBuffer[index4].green;
					int b = imageBuffer[index4].blue;

					PutPixel(x + rotationPivotX + tx*cosine - sine*ty - cosine*rotationPivotX + rotationPivotY*sine, y + rotationPivotY + tx*sine + cosine*ty - sine*rotationPivotX - rotationPivotY*cosine, r, g, b);
				}
			}
	}
	else
	{
		if(angel == 0)
		for (volatile int ty = 0; (float)ty < (float)height * stretchy; ty++)
		{
			if (y < -ty || y + ty >= *screenHeight) continue;
			for (volatile int tx = 0; (float)tx < (float)width*stretchx; tx++)
			{
				if (x < -tx || x + tx >= *screenWidth) continue;

				volatile int index1 = index( (int)((float)tx / stretchx), (int)((float) ty / stretchy));
				

				volatile int r = imageBuffer[index1].red;
				volatile int g = imageBuffer[index1].green;
				volatile int b = imageBuffer[index1].blue;

				if (r == exR && g == exG && b == exB)
					continue;

				PutPixel(x+tx,y+ty, r, g, b);
			}
		}
		else
			for (int ty = 0; ty < height*stretchy; ty++)
			{
				for (int tx = 0; tx < width*stretchx; tx++)
				{
					volatile int index2 = index((int)((float)tx / stretchx), (int)((float)ty / stretchy));

					volatile int r = imageBuffer[index2].red;
					volatile int g = imageBuffer[index2].green;
					volatile int b = imageBuffer[index2].blue;

					if (r == exR && g == exG && b == exB)
						continue;
					
					volatile int X = x + rotationPivotX + tx*cosine - sine*ty - cosine*rotationPivotX + rotationPivotY*sine;
					volatile int Y = y + rotationPivotY + tx*sine + cosine*ty - sine*rotationPivotX - rotationPivotY*cosine;
					
					if (X>0 && X<*screenWidth && Y>0 && Y<*screenHeight)
						PutPixel(X,Y, r, g, b);
				}
			}
	}
}

void CGRAPHICS::IMAGE_S::DrawImageFast(int x, int y)
{
	x = (int)((float)x - 0.5f - (float)width / 2.0f);
	y = (int)((float)y - 0.5f - (float)height / 2.0f);

	for (int ty = height; ty--;)
	{
		if (y < -ty || y + ty >= *screenHeight) continue;

		for (int tx = width; tx--;)
		{
			if (x < -tx || x + tx >= *screenWidth) continue;

			PutPixel(x + tx, y + ty , imageBuffer[m].blue, imageBuffer[m].green, imageBuffer[m].red);
		}
	}
}

void CGRAPHICS::IMAGE_S::DrawToBackground()
{
	memcpy_s(PixelMemory.Int, *screenHeight * *screenWidth * sizeof(Pixel32), imageBuffer, width*height * sizeof(Pixel32));
}

CGRAPHICS::IMAGE_S::~IMAGE_S()
{
	delete []imageBuffer;
}




CGRAPHICS::LETTER_S::LETTER_S(const char* path, int nCharHorizontal, int nCharVertical, char firstChar_s)
{
	fontmap = new IMAGE_S(path);

	firstChar = firstChar_s;
	nCharacters = nCharHorizontal*nCharHorizontal;

	CharMap = new CHARMAP[nCharacters];
	
	fromTo(i,0, nCharacters)
		CharMap[i].alloc(fontmap->width/ nCharHorizontal, fontmap->height/ nCharVertical);

	fromTo(iy, 0, nCharVertical -1)
	{
		fromTo(ix, 0, nCharHorizontal -1)
		{
			
			fromTo(iy2,0, fontmap->height/ nCharVertical -1)
			{
				fromTo(ix2, 0, fontmap->width / nCharHorizontal - 1)
				{
					int a = ix + (iy) * nCharHorizontal;
					int b = (fontmap->width * (fontmap->height - 1-( iy * CharMap[a].sizeY + iy2)) + (ix * CharMap[a].sizeX + ix2));

					CharMap[a].bitmap[ix2][iy2] = fontmap->imageBuffer[b];
				}
			}

		}
	}
}

void CGRAPHICS::LETTER_S::putChar(int c, int posX, int posY,int r,int g, int b)
{
	int t = abs((c-firstChar) % 255);
#define co CharMap[t].bitmap[ix][iy]

	fromTo(iy, 0, CharMap[t].sizeY - 1)
		fromTo(ix, 0, CharMap[t].sizeX - 1)
		{
			if (co.blue == 255 && co.green == 255 && co.red == 255) continue;
			
			PutPixel(posX + ix, posY + iy, r,g,b);
		}	
}

void CGRAPHICS::LETTER_S::putString(int posX, int posY, int r, int g, int b, const char* line,...)
{
	char buffer[1000];

	int length = 0;
	for (; line[length] != '\0'; length++);

	va_list args;
	va_start(args, line);
	length = vsnprintf(buffer, 1000,line,args);
	va_end(args);

	int charWidth = CharMap[0].sizeX;
	int charHeight = CharMap[0].sizeY; 

	int xOffset = 0;
	int yOffset = 0;
	fromTo(i, 0, length - 1)
	{
		xOffset += charWidth + xAdjustmentOffset;

		if (buffer[i] == '\n')
		{
			xOffset = 0;
			yOffset += charHeight + yAdjustmentOffset;
		}
		
		putChar(buffer[i], posX + xOffset, posY + yOffset, r,g,b);
	}
}

CGRAPHICS::LETTER_S::~LETTER_S()
{
	delete fontmap;
}