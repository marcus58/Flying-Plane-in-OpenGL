#pragma once
#include <iostream>
#include <fstream>
#include <freeglut.h>
#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif
#pragma comment(lib, GL_Lib("freeglut_static"))
using namespace std;

unsigned int loadTGA(const char* filename)
{
	char id, cmap, imgtype, bpp, c_garb;
	char* imageData, temp;
	short int s_garb, wid, hgt;
	int nbytes, size, indx;
	ifstream file(filename, ios::in | ios::binary);
	if (!file)
	{
		cout << "*** Error opening image file: " << filename << endl;
		exit(1);
	}
	file.read(&id, 1);
	file.read(&cmap, 1);
	file.read(&imgtype, 1);
	if (imgtype != 2 && imgtype != 3)   //2= colour (uncompressed),  3 = greyscale (uncompressed)
	{
		cout << "*** Incompatible image type: " << (int)imgtype << endl;
		exit(1);
	}
	//Color map specification
	file.read((char*)&s_garb, 2);
	file.read((char*)&s_garb, 2);
	file.read(&c_garb, 1);
	//Image specification
	file.read((char*)&s_garb, 2);  //x origin
	file.read((char*)&s_garb, 2);  //y origin
	file.read((char*)&wid, 2);     //image width
	file.read((char*)&hgt, 2);     //image height
	file.read(&bpp, 1);     //bits per pixel
	file.read(&c_garb, 1);  //img descriptor
	nbytes = bpp / 8;           //No. of bytes per pixels
	size = wid * hgt * nbytes;  //Total number of bytes to be read
	imageData = new char[size];
	file.read(imageData, size);
	if (nbytes > 2)   //swap R and B
	{
		for (int i = 0; i < wid*hgt; i++)
		{
			indx = i * nbytes;
			temp = imageData[indx];
			imageData[indx] = imageData[indx + 2];
			imageData[indx + 2] = temp;
		}
	}

	switch (nbytes)
	{
	case 1:
		glTexImage2D(GL_TEXTURE_2D, 0, 1, wid, hgt, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, imageData);
		break;
	case 3:
		glTexImage2D(GL_TEXTURE_2D, 0, 3, wid, hgt, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		break;
	case 4:
		glTexImage2D(GL_TEXTURE_2D, 0, 4, wid, hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		break;
	}
	delete imageData;
}