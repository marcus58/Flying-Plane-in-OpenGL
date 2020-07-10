#define FREEGLUT_STATIC
#define _CRT_SECURE_NO_WARNINGS
#include <freeglut.h>
#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))

#include <iostream>
#include "textureLoader.h"



GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight)
{
	HANDLE hFileHandle;
	BITMAPINFO *pBitmapInfo = NULL;
	unsigned long lInfoSize = 0;
	unsigned long lBitSize = 0;
	GLbyte *pBits = NULL; // Bitmaps bits
	BITMAPFILEHEADER bitmapHeader;
	DWORD dwBytes;

	// Open the Bitmap file
	hFileHandle = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	// Check for open failure (most likely file does not exist).
	if (hFileHandle == INVALID_HANDLE_VALUE)
		return NULL;

	// File is Open. Read in bitmap header information
	ReadFile(hFileHandle, &bitmapHeader, sizeof(BITMAPFILEHEADER),
		&dwBytes, NULL);

	// Check for a couple of simple errors
	if (dwBytes != sizeof(BITMAPFILEHEADER))
		return FALSE;

	// Check format of bitmap file
	if (bitmapHeader.bfType != 'MB')
		return FALSE;

	// Read in bitmap information structure
	lInfoSize = bitmapHeader.bfOffBits - sizeof(BITMAPFILEHEADER);
	pBitmapInfo = (BITMAPINFO *)malloc(sizeof(GLbyte)*lInfoSize);
	ReadFile(hFileHandle, pBitmapInfo, lInfoSize, &dwBytes, NULL);

	if (dwBytes != lInfoSize)
	{
		free(pBitmapInfo);
		CloseHandle(hFileHandle);
		return FALSE;
	}

	// Save the size and dimensions of the bitmap
	*nWidth = pBitmapInfo->bmiHeader.biWidth;
	*nHeight = pBitmapInfo->bmiHeader.biHeight;
	lBitSize = pBitmapInfo->bmiHeader.biSizeImage;

	// If the size isn't specified, calculate it anyway
	if (pBitmapInfo->bmiHeader.biBitCount != 24)
	{
		free(pBitmapInfo);
		return FALSE;
	}

	if (lBitSize == 0)
		lBitSize = (*nWidth *
			pBitmapInfo->bmiHeader.biBitCount + 7) / 8 *
		abs(*nHeight);

	// Allocate space for the actual bitmap
	free(pBitmapInfo);
	pBits = (GLbyte *)malloc(sizeof(GLbyte)*lBitSize);

	// Read in the bitmap bits, check for corruption
	if (!ReadFile(hFileHandle, pBits, lBitSize, &dwBytes, NULL) ||
		dwBytes != (sizeof(GLbyte)*lBitSize))
		pBits = NULL;

	// Close the bitmap file now that we have all the data we need
	CloseHandle(hFileHandle);

	return pBits;
}

bool LoadTextures(GLuint *texName)
{
	GLbyte *pBits[NUMOFBMPFILE];
	int nWidth, nHeight;

	char* fileName[] = { "flare.bmp" };

	for (int i = 0; i < NUMOFBMPFILE; i++)
	{
		pBits[i] = gltReadBMPBits(fileName[i], &nWidth, &nHeight);
		if (pBits[i] == NULL)
		{
			printf("bmp load failed");
			exit(-1);
		}

		glGenTextures(1, &texName[i]);

		/** 创建纹理对象 */
		glBindTexture(GL_TEXTURE_2D, texName[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, nWidth, nHeight,
			GL_RGB, GL_UNSIGNED_BYTE, pBits[i]);
	}
	return true;
}