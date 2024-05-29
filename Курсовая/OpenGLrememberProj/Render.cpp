#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

bool textureMode = true;
bool lightMode = true;

//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����




//������ ���������� ����
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint texId;

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{
	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE *texarray;

	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//���������� �� ��� ��������
	glGenTextures(1, &texId);
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, texId);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//�������� ������
	free(texCharArray);
	free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}





void Render(OpenGL *ogl)
{



	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);


	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;


	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
		//������ �����
		glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  


	
	double A[] = { 0, 0, 0 };
	double B[] = { 3, 2, 0 };
	double C[] = { 2,9, 0 };
	double D[] = { -3,5, 0 };

	double B1[] = { 2, -1, 0 };
	double C1[] = { 8,0, 0 };
	double D1[] = { 3,2, 0 };

	double B2[] = { -4, -7, 0 };
	double C2[] = { 1,-9, 0 };
	double D2[] = { 2,-1, 0 };

	double Az[] = { 0, 0, 2 };
	double Bz[] = { 3, 2, 2 };
	double Cz[] = { 2,9, 2 };
	double Dz[] = { -3,5, 2 };

	double B1z[] = { 2, -1, 2 };
	double C1z[] = { 8,0, 2 };
	double D1z[] = { 3,2, 2 };

	double B2z[] = { -4, -7, 2 };
	double C2z[] = { 1,-9, 2 };
	double D2z[] = { 2,-1, 2 };

	double Ap[] = { 0,0,0 };
	double Bp[] = { -3,5,0 };
	double Cp[] = { 2,9,0 };
	double Dp[] = { 3,2,0 };
	double Ep[] = { 8,0,0 };
	double Fp[] = { 2,-1,0 };
	double Gp[] = { 1,-9,0 };
	double Hp[] = { -4,-7,0 };

	double Apz[] = { 0,0,2 };
	double Bpz[] = { -3,5,2 };
	double Cpz[] = { 2,9,2 };
	double Dpz[] = { 3,2,2 };
	double Epz[] = { 8,0,2 };
	double Fpz[] = { 2,-1,2 };
	double Gpz[] = { 1,-9,2 };
	double Hpz[] = { -4,-7,2 };

	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_POLYGON);
	//���
	glColor3d(1, 0.5, 0.7);
	glTexCoord2d(0, 0);
	glVertex3dv(Ap);
	glTexCoord2d(2, -1);
	glVertex3dv(Fp);
	glTexCoord2d(8, 0);
	glVertex3dv(Ep);
	glTexCoord2d(3, 2);
	glVertex3dv(Dp);
	glTexCoord2d(2, 9);
	glVertex3dv(Cp);
	glTexCoord2d(-3, 5);
	glVertex3dv(Bp);
	glEnd();

	//glNormal3d(0, 0, 1);
	
	//�����
	/*glBegin(GL_POLYGON);
	glTexCoord2d(0, 0);
	glVertex3dv(Apz);
	glTexCoord2d(2, -1);
	glVertex3dv(Fpz);
	glTexCoord2d(8, 0);
	glVertex3dv(Epz);
	glTexCoord2d(3, 2);
	glVertex3dv(Dpz);
	glTexCoord2d(2, 9);
	glVertex3dv(Cpz);
	glTexCoord2d(-3, 5);
	glVertex3dv(Bpz);

	glEnd();*/


	glBegin(GL_QUADS);

	//1 wall
	glColor3d(0.7, 1, 0.35);
	glTexCoord2d(0, 0);
	glVertex3dv(Ap);
	glTexCoord2d(0, 1);
	glVertex3dv(Apz);
	glTexCoord2d(1, 1);
	glVertex3dv(Bpz);
	glTexCoord2d(1, 0);
	glVertex3dv(Bp);
	//2 wall
	glTexCoord2d(0, 0);
	glVertex3dv(Bp);
	glTexCoord2d(0, 1);
	glVertex3dv(Bpz);
	glTexCoord2d(1, 1);
	glVertex3dv(Cpz);
	glTexCoord2d(1, 0);
	glVertex3dv(Cp);
	//3 wall
	glTexCoord2d(0, 0);
	glVertex3dv(Cp);
	glTexCoord2d(0, 1);
	glVertex3dv(Cpz);
	glTexCoord2d(1, 1);
	glVertex3dv(Dpz);
	glTexCoord2d(1, 0);
	glVertex3dv(Dp);
	//4 wall
	glTexCoord2d(0, 0);
	glVertex3dv(Dp);
	glTexCoord2d(0, 1);
	glVertex3dv(Dpz);
	glTexCoord2d(1, 1);
	glVertex3dv(Epz);
	glTexCoord2d(1, 0);
	glVertex3dv(Ep);
	//5 wall
	glTexCoord2d(0, 0);
	glVertex3dv(Ep);
	glTexCoord2d(0, 1);
	glVertex3dv(Epz);
	glTexCoord2d(1, 1);
	glVertex3dv(Fpz);
	glTexCoord2d(1, 0);
	glVertex3dv(Fp);
	//6 wall
	glTexCoord2d(0, 0);
	glVertex3dv(Fp);
	glTexCoord2d(0, 1);
	glVertex3dv(Fpz);
	glTexCoord2d(1, 1);
	glVertex3dv(Gpz);
	glTexCoord2d(1, 0);
	glVertex3dv(Gp);
	////7 wall
	//glVertex3dv(Gp);
	//glVertex3dv(Gpz);
	//glVertex3dv(Hpz);
	//glVertex3dv(Hp);
	//8 wall
	glTexCoord2d(0, 0);
	glVertex3dv(Hp);
	glTexCoord2d(0, 1);
	glVertex3dv(Hpz);
	glTexCoord2d(1, 1);
	glVertex3dv(Apz);
	glTexCoord2d(1, 0);
	glVertex3dv(Ap);

	glEnd();





	//����������
	double x, y, m1[1000][3], m2[1000][3];
	int k = 0;
	double r = sqrt(29) / 2;
	for (double i = 0; i <= 360; i += 0.5) {
		x = r * cos(i * PI / 180) - 1.5;
		y = r * sin(i * PI / 180) - 8;
		if (y <= -0.4 * x - 8.6) {
			m1[k][0] = x;
			m1[k][1] = y;
			m1[k][2] = 0;

			m2[k][0] = x;
			m2[k][1] = y;
			m2[k][2] = 2;
			k++;
		}
	}


	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.8, 0.7, 0.7, 1.0);
		glTexCoord2d(*m1[i], *m1[i]);
		glVertex3dv(m1[i]);
		glTexCoord2d(*m1[i+1], *m1[i+1]);
		glVertex3dv(m1[i + 1]);
		glTexCoord2d(*m2[i + 1], *m2[i + 1]);
		glVertex3dv(m2[i + 1]);
		glTexCoord2d(*m2[i], *m2[i]);
		glVertex3dv(m2[i]);
		glEnd();
	}

	//���� � ��� ����������
	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.8, 0.7, 0.7, 1.0);
		glTexCoord2d(0, 0);
		glVertex3dv(m1[i]);
		glTexCoord2d(1, 0);
		glVertex3dv(Hp);
		glTexCoord2d(1, 1);
		glVertex3dv(Ap);
		glTexCoord2d(0, 1);
		glVertex3dv(Fp);
		glEnd();
	}

	//glBegin(GL_QUADS);
	//glColor4f(0.8, 0.7, 0.7, 1.0);
	//glTexCoord2d(*m1[i], *m1[i]);
	//glVertex3dv(m1[i]);
	//glTexCoord2d(-4, -7);
	//glVertex3dv(Hp);
	//glTexCoord2d(0, 0);
	//glVertex3dv(Ap);
	//glTexCoord2d(2, -1);
	//glVertex3dv(Fp);
	//glEnd();

	

	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.8, 0.7, 0.7, 1.0);
		glTexCoord2d(0, 0);
		glVertex3dv(m2[i]);
		glTexCoord2d(0, 1);
		glVertex3dv(Hpz);
		glTexCoord2d(1, 1);
		glVertex3dv(Apz);
		glTexCoord2d(1, -1);
		glVertex3dv(Fpz);
		glEnd();
	}
	/*glBegin(GL_QUADS);
	glColor4f(0.8, 0.7, 0.7, 1.0);
	glTexCoord2d(*m2[i], *m2[i]);
	glVertex3dv(m2[i]);
	glTexCoord2d(-4, -7);
	glVertex3dv(Hpz);
	glTexCoord2d(0, 0);
	glVertex3dv(Apz);
	glTexCoord2d(2, -1);
	glVertex3dv(Fpz);
	glEnd();*/



	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//���������������� ����
	double xkrug, ykryg;
	int j = 0;
	glBegin(GL_TRIANGLES);
	for (double i = 0; i <= 360; i += 0.5) {
		if (!j) {
			x = r * cos(i * PI / 180) ;
			y = r * sin(i * PI / 180) ;
			
		}
		xkrug = x; ykryg = y;
		x = r * cos(i * PI / 180)+6;
		y = r * sin(i * PI / 180)+6;
		if (j) {
			glTexCoord2d(6, 6);
			glVertex2d(6, 6);
			glTexCoord2d(x, y);
			glVertex2d(x, y);
			glTexCoord2d(xkrug, ykryg);
			glVertex2d(xkrug, ykryg);
		}
		j++;
	}
	glEnd();

	//�����
	glEnable(GL_COLOR_MATERIAL);
	//glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	glBegin(GL_POLYGON);
	glNormal3d(0, 0, 1);
	glColor4d(1, 0.7, 0.5, 1);
	glTexCoord2d(0, 0);
	glVertex3dv(Apz);
	glTexCoord2d(0, 1);
	glVertex3dv(Fpz);
	glTexCoord2d(0, 2);
	glVertex3dv(Epz);
	glTexCoord2d(1, 2);
	glVertex3dv(Dpz);
	glTexCoord2d(1, 1);
	glVertex3dv(Cpz);
	glTexCoord2d(1, 0);
	glVertex3dv(Bpz);

	glEnd();
	/*glTexCoord2d(0, 0);
	glVertex3dv(Apz);
	glTexCoord2d(2, -1);
	glVertex3dv(Fpz);
	glTexCoord2d(8, 0);
	glVertex3dv(Epz);
	glTexCoord2d(3, 2);
	glVertex3dv(Dpz);
	glTexCoord2d(2, 9);
	glVertex3dv(Cpz);
	glTexCoord2d(-3, 5);
	glVertex3dv(Bpz);*/


	


	//������ ��������� ���������� ��������

	/*glBindTexture(GL_TEXTURE_2D, texId);

	glColor3d(0.6, 0.6, 0.6);
	glBegin(GL_QUADS);

	glNormal3d(0, 0, 1);
	glTexCoord2d(0 ,0);
	glVertex3d(-4,-7,2);
	glTexCoord2d(1, 0);
	glVertex3d(1, -9, 2);
	glTexCoord2d(1, 1);
	glVertex3d(2, -1, 2);
	glTexCoord2d(0, 1);
	glVertex3d(0, 0, 2);

	glTexCoord2d(0, 0);
	glVertex3d(0, 0, 2);
	glTexCoord2d(1, 0);
	glVertex3d(2, -1, 2);
	glTexCoord2d(1, 1);
	glVertex3d(8, 0, 2);
	glTexCoord2d(0, 1);
	glVertex3d(3, 2, 2);

	glTexCoord2d(0, 0);
	glVertex3d(0, 0, 2);
	glTexCoord2d(1, 0);
	glVertex3d(3, 2, 2);
	glTexCoord2d(1, 1);
	glVertex3d(2, 9, 2);
	glTexCoord2d(0, 1);
	glVertex3d(-3, 5, 2);
	glEnd();*/
	//����� ��������� ���������� ��������

	//======================================================================
   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}