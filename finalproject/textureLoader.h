#pragma once
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

#define NUMOFBMPFILE 1

GLbyte* gltReadBMPBits(const char *szFileName, int *nWidth, int *nHeight);
bool LoadTextures(GLuint *texName);