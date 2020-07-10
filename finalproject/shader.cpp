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

//�ļ���ȡ(������shader�ļ���ȡΪ�ַ�����ʽ����)
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

	//������ɫ������(����C�еĺ��������Ը���)
	v = glCreateShader(GL_VERTEX_SHADER);//������ɫ��
	f = glCreateShader(GL_FRAGMENT_SHADER);//Ƭ����ɫ��

	vs = textFileRead("minimal.vert");
	fs = textFileRead("minimal.frag");

	const char *vv = vs;
	const char *ff = fs;
	//����ɫ��������ӵ���������ɫ��������
	glShaderSource(v, 1, &vv, NULL);
	glShaderSource(f, 1, &ff, NULL);

	free(vs);
	free(fs);
	//������ɫ��Դ����
	glCompileShader(v);
	glCompileShader(f);


	p = glCreateProgram();//������ɫ������
	glAttachShader(p, v);//����ɫ����������������ɫ������
	glAttachShader(p, f);

	glLinkProgram(p); //������ɫ������

	glUseProgram(p);//ʹ����ɫ��

	//�õ���ɫ��������unifrom���Ե�time������ַ���Ա������޸���
	loc = glGetUniformLocation(p, "time");
}