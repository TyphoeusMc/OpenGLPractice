#include "stdafx.h"
#include <Windows.h>
#include <gl/glut.h>
#include <GL/GLAUX.H>

#define windowWidth 600
#define windowHeight 600
#define planets 3

//1.完成基本模型绘制：圆形轨道，单色球体 done
//2.完成光照效果  done
//3.轨道角度  
//4.抗锯齿
//5.添加月亮与其他行星
//6.完成3d贴图  done
//7.自转  done
//8.视角变换

double cnt=0; //用于计时函数的计数器，并用于计算各行星旋转的角度
// 材质标记数组，暂时为2，太阳与地球
GLuint Texture[planets];
GLUquadricObj *Surface[planets];

void universeTimer(int a);
void updateSun();
void updateEarth();
void updateMoon();
void drawSolarSystem();

//计时函数
void universeTimer(int a)
{
	//假设以天为单位，每0.1秒一天
	//Sleep(100);
	cnt++;
	if (cnt >= 10000000)
		cnt = 0;    //防止溢出
	drawSolarSystem();
	glutTimerFunc(15, universeTimer, 0);
}

/*
太阳 1392000 km
水星直径 4878 km
金星直径 12103.6 km
地球直径 12756.3 km
火星直径 6794 km
木星直径 142984 km
土星直径 120536 km
天王星直径 51118 km
海王星直径 49,528 km
*/

//更新太阳位置
void updateSun()
{
	Surface[0] = gluNewQuadric(); //使用曲面创建球
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	gluQuadricTexture(Surface[0], GL_TRUE);//纹理函数

	//glColor3f(1.0, 0.0, 0.0);
	//glColor3d(244.0 / 255.0, 116.0 / 255.0, 7.0 / 255.0);
	//太阳自转周期25天
	glRotated((25.0 / 360.0)*cnt, 0.0, 0.0, -1.0);
	glDisable(GL_LIGHTING);
	//太阳不设置光照，自身最亮
	/*
	GLfloat sunAmbientDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sunAmbientDiffuse); //太阳不反射
	GLfloat sunEmission[4] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SHININESS, sunEmission); //太阳本身发光
	*/
	gluSphere(Surface[0],18000, 100, 100);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

//更新地球位置
void updateEarth()
{
	Surface[1] = gluNewQuadric(); //使用曲面创建球
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	gluQuadricTexture(Surface[1], GL_TRUE);//纹理函数

	//glColor3d(33.0/255.0, 41.0/255.0, 78.0/255.0);//蓝色
	glRotatef((365.0 / 360.0)*cnt, 0.0, 0.0, -1.0);
	glTranslatef(100000, 0.0, 0.0);
	glRotatef(cnt*10, 0.0, 0.0, 1.0); //地球自转

	GLfloat earthAmbientDiffuse[4] = { 0.0, 0.0, 0.0, 1.0 }; //没有漫射光以及反射的环境光
	GLfloat earthSpecular[4] = { 0.4, 0.4, 0.4, 0.5 };
	GLfloat earthShiness = 0.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, earthAmbientDiffuse); 
	glMaterialfv(GL_FRONT, GL_SPECULAR, earthSpecular); //主要反射光的成分为镜面反射
	glMaterialf(GL_FRONT, GL_SHININESS, earthShiness);  //材质设置为最粗糙

	gluSphere(Surface[1],10000, 100, 100);
	glPopMatrix();
}

//月亮
void updateMoon()
{
	Surface[2] = gluNewQuadric(); //使用曲面创建球
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	gluQuadricTexture(Surface[2], GL_TRUE);//纹理函数

	glRotatef((365.0 / 360.0)*cnt, 0.0, 0.0, -1.0);
	glTranslatef(100000, 0.0, 0.0);
	glRotatef(cnt * 10, 0.0, 0.0, 1.0); //月亮围绕地球
	glTranslatef(30000, 0.0, 0.0);

	GLfloat earthAmbientDiffuse[4] = { 0.0, 0.0, 0.0, 1.0 }; //没有漫射光以及反射的环境光
	GLfloat earthSpecular[4] = { 0.4, 0.4, 0.4, 0.5 };
	GLfloat earthShiness = 0.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, earthAmbientDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earthSpecular); //主要反射光的成分为镜面反射
	glMaterialf(GL_FRONT, GL_SHININESS, earthShiness);  //材质设置为最粗糙

	gluSphere(Surface[2], 6000, 100, 100);
	glPopMatrix();
}

void loadTexture()
{
	AUX_RGBImageRec *Image[planets];
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //这是干嘛用的
	glGenTextures(planets, Texture);
	//绑定并设置太阳材质
	glBindTexture(GL_TEXTURE_2D, Texture[0]);
	//设定缩放模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (Image[0] = auxDIBImageLoadA("sun.bmp")) 
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Image[0]->sizeX, Image[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image[0]->data);
	
	//绑定地球材质
	glBindTexture(GL_TEXTURE_2D, Texture[1]);
	//设定缩放模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (Image[1] = auxDIBImageLoadA("earth.bmp"))
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Image[1]->sizeX, Image[1]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image[1]->data);

	//绑定月球材质
	glBindTexture(GL_TEXTURE_2D, Texture[2]);
	//设定缩放模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	if (Image[2] = auxDIBImageLoadA("moon.bmp"))
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Image[2]->sizeX, Image[2]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image[2]->data);

}

//整体绘制更新
void drawSolarSystem()
{
	glEnable(GL_COLOR_MATERIAL); //使得球体保持原有颜色

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清空原有显示数据
	//启用抗锯齿
	//glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	glMatrixMode(GL_PROJECTION); //操作投影矩阵
	glLoadIdentity(); //载入单位矩阵（将当前坐标重置为初始坐标）
	gluPerspective(70, windowWidth / windowHeight, 1, 1000000);//设置视角
	glMatrixMode(GL_MODELVIEW);//操作模型矩阵
	glLoadIdentity();
	gluLookAt(0, 180000, 40000, 0, 0, 0, 0, 0, 1);//设置观察位置

	//设置太阳处的光源
	GLfloat lightPosition[4] = { 0.0,0.0,0.0,1.0 }; //处于太阳处
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	GLfloat lightAmbient[4] = { 0.4,0.4,0.4,0.5 }; //为了使得各个球体有一定亮度以方便观察，使其有环境光属性
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	
	//光属性使用默认值
	glEnable(GL_TEXTURE_2D);
	updateSun();
	updateEarth();
	updateMoon();
	glDisable(GL_TEXTURE_2D);
	//使用光照
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//交换缓冲区
	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	//使用双缓冲模式
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("solar system demo");
	glutDisplayFunc(&drawSolarSystem);
	loadTexture();
	glutTimerFunc(15, universeTimer, 0);
	glutMainLoop();
	return 0;
}






