#pragma once
#define FREEGLUT_STATIC
#include <freeglut.h>
#include "tgaloader.h"
#define BOXSIDE 200  


GLfloat contrlPoints[6][4][5] = {       //矩形6个面，每个面4个点，每个点空间中XYZ+图片上XY
{{-BOXSIDE, -BOXSIDE, -BOXSIDE, 0.25, 0.334}, {BOXSIDE, -BOXSIDE, -BOXSIDE, 0.5, 0.334},
{BOXSIDE, BOXSIDE, -BOXSIDE, 0.5, 0.666}, {-BOXSIDE, BOXSIDE, -BOXSIDE, 0.25, 0.666}},    //前面
{{BOXSIDE, -BOXSIDE, BOXSIDE, 0.75, 0.334}, {-BOXSIDE, -BOXSIDE, BOXSIDE, 1, 0.334},
{-BOXSIDE, BOXSIDE, BOXSIDE, 1, 0.666}, {BOXSIDE, BOXSIDE, BOXSIDE, 0.75, 0.666}},        //后面
{{-BOXSIDE, -BOXSIDE, BOXSIDE, 0, 0.334}, {-BOXSIDE, -BOXSIDE, -BOXSIDE, 0.25, 0.334},
{-BOXSIDE, BOXSIDE, -BOXSIDE, 0.25, 0.666}, {-BOXSIDE, BOXSIDE, BOXSIDE, 0, 0.666}},      //左面
{{BOXSIDE, -BOXSIDE, -BOXSIDE, 0.5, 0.334}, {BOXSIDE, -BOXSIDE, BOXSIDE, 0.75, 0.334},
{BOXSIDE, BOXSIDE, BOXSIDE, 0.75, 0.666}, {BOXSIDE, BOXSIDE, -BOXSIDE, 0.5, 0.666}},        //右面
{{-BOXSIDE, +BOXSIDE, -BOXSIDE, 0.25, 0.666}, {BOXSIDE, BOXSIDE, -BOXSIDE, 0.5, 0.666},
{BOXSIDE, BOXSIDE, BOXSIDE, 0.5, 1}, {-BOXSIDE, BOXSIDE, BOXSIDE, 0.25, 1}},            //上面
{{-BOXSIDE, -BOXSIDE, BOXSIDE, 0.25, 0}, {BOXSIDE, -BOXSIDE, BOXSIDE, 0.5, 0},
{BOXSIDE, -BOXSIDE, -BOXSIDE, 0.5, 0.334}, {-BOXSIDE, -BOXSIDE, -BOXSIDE, 0.25, 0.334}}    //下面
};


GLfloat normals[6][3] = {
	{0, 0, 1}, {0, 0, -1}, {1, 0, 0}, {-1, 0, 0}, {0, -1, 0}, {0, 1, 0}
};

GLuint textureId;

void load()
{
	glGenTextures(1, &textureId); 				// Create a Texture object
	glBindTexture(GL_TEXTURE_2D, textureId);		//Use the texture
	const char *path = "test5.tga";
	loadTGA(path);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void skybox()
{
	glEnable(GL_TEXTURE_2D);

	glColor4f(1.0, 1.0, 1.0, 1.0);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glBegin(GL_QUADS);

	for (int i = 0; i < 6; i++) {
		glNormal3f(normals[i][0], normals[i][1], normals[i][2]); 
		for (int j = 0; j < 4; j++) {
			glTexCoord2f(contrlPoints[i][j][3], contrlPoints[i][j][4]); 
			glVertex3f(contrlPoints[i][j][0], contrlPoints[i][j][1], contrlPoints[i][j][2]);
		}

	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}