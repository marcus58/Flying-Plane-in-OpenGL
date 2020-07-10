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
#include <cmath> 
#include <ctime>
//#include <vector>

#include "ChargeObject.h"
#include "Particle.h"
#include "shader.h"
#include "textureLoader.h"
#include "tgaloader.h"
#include "backtexture.h"


//#include <Windows.h>
//#include <string>
//#include <fstream>

using namespace std;

#define PI 3.1415926535
#define FRAME_MSEC (17)
#define PARTICLE_AMOUNT (10000)
#define COLLISION_AMOUNT (30000)

bool mouseLeftDown;
bool mouseRightDown;
GLfloat mouseX, mouseY;
GLfloat cameraDistance;
GLfloat cameraAngleX;
GLfloat cameraAngleY;
GLfloat xoffset;
GLfloat yoffset;
GLfloat movespeed = 3.0;
GLfloat xstep = 0;
GLfloat ystep = 0;
GLfloat zstep = 0;
GLfloat spinround = 50;
GLint roundcount = 0;
GLint turningcount = 0;
GLint explosioncount = 10;
bool trail_1 = true;
bool trail_2, trail_3, trail_4, trail_5;
bool collisionoccur;

ChargeObject obj1 = "F16C_US_LOD1_v25.obj";


GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.2,0.2,0.2,1.0 };

/** 创建粒子类对象 */
CParticle jet;
CParticle explosion;

/** 用来设置粒子的属性值 */
float x, y, z, vx, vy, vz, ax, ay, az, particlesize, lifetime, decrease;
int r, g, b;
float gravity = 0.8;

GLuint  texName[NUMOFBMPFILE];

bool Initexplosion()
{
	for (int i = 0; i < explosion.GetNumOfParticle(); ++i)
	{
		//初始化颜色，五彩斑斓的爆炸色
		r = 226;
		g = 17;
		b = 12;
		/*r = 50;
		g = 50;
		b = 100;*/

		explosion.SetColor(i, r, g, b);

		/** 初始化位置 */
		x = 0.05f * (rand() % 200 - 100);
		y = 0.05f * (rand() % 200 - 100);
		z = 0.05f * (rand() % 200 - 100);
		explosion.SetPosition(i, x, y, z);

		//初始化速度
		vx = 0.05f * (rand() % 200 - 100);
		vy = 0.05f * (rand() % 200 - 100);
		vz = 0.05f * (rand() % 200 - 100);

		explosion.SetVelocity(i, vx, vy, vz);

		//初始化加速度
		if (vx < 0)
		{
			ax = 0.2*vx;
		}
		else
		{
			ax = -0.2*vx;
		}

		if (vy < 0)
		{
			ay = 0.2*vy - gravity;
		}
		else
		{
			ay = -0.2*vy - gravity;
		}

		if (vz < 0)
		{
			az = 0.2*vz;
		}
		else
		{
			az = -0.2*vz;
		}

		explosion.SetAcceleration(i, ax, ay, az);
		//初始化生命周期
		lifetime = 100;
		explosion.SetLifeTime(i, lifetime);

		//消失速度
		decrease = 0.1*(rand() % 50);
		explosion.SetDec(i, decrease);

		//初始化大小
		explosion.SetSize(i, 0.1f);
	}
	return true;
}

void Updateexplosion()
{
	/** 更新位置 */
	x += vx;
	y += vy;
	z += vz;

	/** 更新速度 */
	vx += ax;
	vy += ay;
	vz += az;

	/** 更新生存时间 */
	lifetime -= decrease;

	/** 如果粒子消失或生命结束 */
	if (lifetime <= 0)
	{
		/** 初始化位置 */
		x = 0.05f * (rand() % 200 - 100);
		y = 0.05f * (rand() % 200 - 100);
		z = 0.05f * (rand() % 200 - 100);

		/** 初始化速度 */
		vx = 0.05f * (rand() % 200 - 100);
		vy = 0.05f * (rand() % 200 - 100);
		vz = 0.05f * (rand() % 200 - 100);

		//初始化加速度
		if (vx < 0)
		{
			ax = 0.2*vx;
		}
		else
		{
			ax = -0.2*vx;
		}

		if (vy < 0)
		{
			ay = 0.2*vy-gravity;
		}
		else
		{
			ay = -0.2*vy-gravity;
		}

		if (vz < 0)
		{
			az = 0.2*vz;
		}
		else
		{
			az = -0.2*vz;
		}

		lifetime = 100;
		decrease = 0.1 * (rand() % 50);
	}
}

void Drawexplosion()
{
	/** 绑定纹理 */
	glBindTexture(GL_TEXTURE_2D, texName[0]);

	for (int i = 0; i < explosion.GetNumOfParticle(); ++i)
	{
		/** 获得粒子的所有属性 */
		explosion.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, particlesize, lifetime, decrease);
		//glLoadIdentity();//恢复所有弹出的栈（恢复初始坐标系）
		//glTranslatef(0.0f, 0.0f, -6.0f);
		glColor4ub(r, g, b, 255);
		//glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
		/** 画出粒子 */
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z - particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z + particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x - particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x - particlesize, y + particlesize, z + particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z + particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z - particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z - particlesize);
		glEnd();

		/** 更新粒子属性 */
		Updateexplosion();
		explosion.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, particlesize, lifetime, decrease);
	}
	//glutPostRedisplay();//发送渲染请求
}

bool Initjet()
{
	for (int i = 0; i < jet.GetNumOfParticle(); ++i)
	{
		//初始化颜色
		/*r = rand()%255;
		g = rand()%255;
		b = rand()%255;*/
		r = 200;
		g = 200;
		b = 200;

		jet.SetColor(i, r, g, b);

		//初始化坐标
		x = 0.05f * (rand() % 9);
		y = -0.05f * (rand() % 9) - 6.0;
		z = 0.05f * (rand() % 9) + 2.0;
		jet.SetPosition(i, x, y, z);

		//初始化速度
		vx = 0.000005f * (rand() % 9000 - rand() % 9000);
		vy = -0.00005f * (rand() % 9000);
		vz = 0.000001f * (rand() % 9000);

		jet.SetVelocity(i, vx, vy, vz);

		//初始化加速度
		ax = 0;
		ay = 0.002;
		az = 0;

		jet.SetAcceleration(i, ax, ay, az);
		//初始化生命周期
		lifetime = 100;
		jet.SetLifeTime(i, lifetime);

		//消失速度
		decrease = 0.05*(rand() % 50);
		jet.SetDec(i, decrease);

		//初始化大小
		jet.SetSize(i, 0.01f);
	}
	return true;
}

void Updatejet()
{
	/** 更新位置 */
	x += vx;
	y += vy;
	z += vz;

	/** 更新速度 */
	vy += ay;

	/** 更新生存时间 */
	lifetime -= decrease;

	/** 如果粒子消失或生命结束 */
	if (y >= - 6.0 || lifetime <= 0)
	{
		/** 初始化位置 */
		x = 0.05f * (rand() % 9);
		y = -0.05f * (rand() % 9) - 6.0;
		z = 0.05f * (rand() % 9) + 2.0;

		/** 初始化速度 */
		vx = 0.000005f * (rand() % 9000 - rand() % 9000);
		vy = -0.00005f * (rand() % 9000);
		vz = 0.000001f * (rand() % 90000);

		lifetime = 100;
		decrease = 0.05 * (rand() % 50);
	}
}

void Drawjet()
{
	/** 绑定纹理 */
	glBindTexture(GL_TEXTURE_2D, texName[0]);

	for (int i = 0; i < jet.GetNumOfParticle(); ++i)
	{
		/** 获得粒子的所有属性 */
		jet.GetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, particlesize, lifetime, decrease);
		//glLoadIdentity();//恢复所有弹出的栈（恢复初始坐标系）
		//glTranslatef(0.0f, 0.0f, -6.0f);
		glColor4ub(r, g, b, 255);
		//glNormal3f(0.0f, 0.0f, 1.0f);   /**< 定义法线方向 */
		/** 画出粒子 */
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z - particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z + particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x - particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x - particlesize, y + particlesize, z + particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z + particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y - particlesize, z - particlesize);

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z - particlesize);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x - particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z + particlesize);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + particlesize, y + particlesize, z - particlesize);
		glEnd();

		/** 更新粒子属性 */
		Updatejet();
		jet.SetAll(i, r, g, b, x, y, z, vx, vy, vz, ax, ay, az, particlesize, lifetime, decrease);
	}
	//glutPostRedisplay();//发送渲染请求
}


void Init()
{
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClearDepth(1.0f);//指定深度缓冲区的清除值(即将深度缓冲区里的值设置为1)
	glDepthFunc(GL_LEQUAL);//指定用于深度缓冲比较值(即新进像素深度值与原来的1比较，<=则通过，否则丢弃)
	glShadeModel(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_TEXTURE_2D);             /**< 开启纹理映射 */
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);    /**< 设置混合因子获得半透明效果 */
	glEnable(GL_BLEND);

	glEnable(GL_NORMALIZE);

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);	//设置环境光源
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);	//设置漫反射光源
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);	//设置镜面光源
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);		//设置灯光位置

	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);						//启用材质的颜色跟踪
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);	//指定材料着色的面
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, light_specular);	//指定材料对镜面光的反应
	glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 100);

	time_t t;
	srand((unsigned)time(&t));//选取种子文件
	load();
	if (!LoadTextures(texName))
	{
		printf("bmp load failed");
		exit(-1);
	}
	/** 创建PARTICLE_AMOUNT个粒子 */
	jet.Create(PARTICLE_AMOUNT);
	explosion.Create(COLLISION_AMOUNT);
	/** 初始化粒子 */
	Initjet();
	Initexplosion();

	glewInit();
	setShaders();
}

void myDisplay(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glTranslatef(xoffset, 0, 0);//a,d move
	glTranslatef(0, 0, yoffset);//w,s move
	glTranslatef(0, cameraDistance, 0);//mouse click right up and down
	glRotatef(cameraAngleX, 1, 0, 0);//mouse click left 
	glRotatef(cameraAngleY, 0, 1, 0);//mouse click left
	//glTranslatef(x, y, z);
	skybox();
	glPushMatrix();
	glTranslatef(xstep, ystep, zstep);
	if (trail_2)
	{
		glRotatef((-1.0)*turningcount, 0, 0, 1);
	}
	if (trail_3)
	{
		glRotatef(-90.0 - 360 * roundcount / 300, 0, 0, 1);
	}
	if (trail_4)
	{
		glRotatef(turningcount - 180.0, 0, 0, 1);
	}

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	
	glPushMatrix();

	//insert test code HERE!!!
	//Drawexplosion();
	//粒子系统展示
	if (!collisionoccur)
	{
		obj1.display(); //模型展示
		Drawjet();
	}
	else
	{
		if (explosioncount > 0)
		{
			obj1.display(); //模型展示
			Drawexplosion();
		}		
	}

	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
	
	glutSwapBuffers();

}

//animation maker

void myTime(int id)
{
	if (ystep < spinround && trail_1)
	{
		ystep += 1.0;
	}
	else
	{
		trail_1 = false;
		if (turningcount < 90)
		{
			trail_2 = true;
			turningcount += 1;
		}
		else
		{
			trail_2 = false;
			if (roundcount < 600)
			{
				trail_3 = true;
				ystep = spinround * cos(2 * PI*roundcount / 300);
				xstep = spinround * sin(2 * PI*roundcount / 300);
				zstep += 0.1;
				roundcount++;
			}
			else
			{
				trail_3 = false;
				if (turningcount < 180)
				{
					trail_4 = true;
					turningcount += 1;
				}
				else
				{
					trail_4 = false;
					if (ystep < 100)
					{
						trail_5 = true;
						ystep += 1.0;
					}
					else
					{
						trail_5 = false;
						collisionoccur = true;
						explosioncount--;
					}
				}	
			}
		}		
	}

	glutPostRedisplay();
	glutTimerFunc(FRAME_MSEC, myTime, 1);
}

void myReshape(GLsizei w, GLsizei h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, w / h, 1, 1000);
	gluLookAt(0, 0, 120, 0, 0, 0, 0, 1.0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myMouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
	{
		movespeed += 0.1;
	}
	else
	{
		if (dir < 0 && movespeed > 0.1)
		{
			movespeed -= 0.1;
		}
	}
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		xoffset += movespeed;
		break;
	case 'd':
		xoffset -= movespeed;
		break;
	case 's':
		yoffset -= movespeed;
		break;
	case 'w':
		yoffset += movespeed;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else
		{
			mouseLeftDown = false;
		}
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else
		{
			mouseRightDown = false;
		}
	}
}

void myMotion(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX)*0.1;
		cameraAngleX += (y - mouseY)*0.1;
	}

	if (mouseRightDown)
	{
		cameraDistance += (y - mouseY) * 0.1;
	}

	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple Fighter Simulator");
	Init();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutMouseWheelFunc(myMouseWheel);
	glutTimerFunc(FRAME_MSEC, myTime, 1);
	glutMainLoop();
	return 0;
}

