#define GLEW_STATIC
#include <glew.h>

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
#include "shader.h"

GLuint v, f, p;
GLint loc;

//文件读取(将本地shader文件读取为字符串形式加载)
char *textFileRead(char *fn)
{
	FILE *fp;
	char *content = NULL;

	int count = 0;

	if (fn != NULL)
	{
		fp = fopen(fn, "rt");

		if (fp != NULL)
		{
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0)
			{
				content = (char *)malloc(sizeof(char) * (count + 1));
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void setShaders()
{
	char *vs = NULL, *fs = NULL;

	//创建着色器对象(类似C中的函数，可以复用)
	v = glCreateShader(GL_VERTEX_SHADER);//顶点着色器
	f = glCreateShader(GL_FRAGMENT_SHADER);//片段着色器

	vs = textFileRead("minimal.vert");
	fs = textFileRead("minimal.frag");

	const char *vv = vs;
	const char *ff = fs;
	//将着色器代码添加到创建的着色器容器中
	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);
	//编译着色器源代码
	glCompileShader(v);
	glCompileShader(f);


	p = glCreateProgram();//创建着色器程序
	glAttachShader(p, v);//将着色器对象关联到这个着色器程序
	glAttachShader(p, f);

	glLinkProgram(p); //链接着色器程序

	glUseProgram(p);//使用着色器

	//得到着色器程序中unifrom属性的time变量地址，以便外面修改它
	loc = glGetUniformLocation(p, "time");
}