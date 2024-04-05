

#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <cmath>
#include <math.h>
#define PI 3.14159265

void Render(double delta_time)
{    
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

	double Az[] = { 0, 0, 5 };
	double Bz[] = { 3, 2, 5 };
	double Cz[] = { 2,9, 5 };
	double Dz[] = { -3,5, 5 };

	double B1z[] = { 2, -1, 5 };
	double C1z[] = { 8,0, 5 };
	double D1z[] = { 3,2, 5 };

	double B2z[] = { -4, -7, 5 };
	double C2z[] = { 1,-9, 5 };
	double D2z[] = { 2,-1, 5 };

	glBegin(GL_QUADS);
	//низ
		glColor3d(1, 0.5, 0.7);
		/*glVertex3dv(A);
		glVertex3dv(B);
		glVertex3dv(C);
		glVertex3dv(D);*/

		glVertex3dv(A);
		glVertex3dv(B1);
		glVertex3dv(C1);
		glVertex3dv(D1);

		/*glVertex3dv(A);
		glVertex3dv(B2);
		glVertex3dv(C2);
		glVertex3dv(D2);*/
	//вверх
		glColor3d(0, 0.7, 1);
		/*glVertex3dv(Az);
		glVertex3dv(Bz);
		glVertex3dv(Cz);
		glVertex3dv(Dz);*/

		glVertex3dv(Az);
		glVertex3dv(B1z);
		glVertex3dv(C1z);
		glVertex3dv(D1z);

		/*glVertex3dv(Az);
		glVertex3dv(B2z);
		glVertex3dv(C2z);
		glVertex3dv(D2z);*/

		//1 wall
		glColor3d(0.7, 1, 0.35);
		glVertex3dv(A);
		glVertex3dv(Az);
		glVertex3dv(Dz);
		glVertex3dv(D);
		////2 wall
		//glVertex3dv(C);
		//glVertex3dv(Cz);
		//glVertex3dv(Dz);
		//glVertex3dv(D);
		//3 wall
		/*glVertex3dv(C);
		glVertex3dv(Cz);
		glVertex3dv(Bz);
		glVertex3dv(B);*/
		//4 wall
		glVertex3dv(C1);
		glVertex3dv(C1z);
		glVertex3dv(Bz);
		glVertex3dv(B);
		//5 wall
		glVertex3dv(C1);
		glVertex3dv(C1z);
		glVertex3dv(D2z);
		glVertex3dv(D2);
		//6 wall
		glVertex3dv(C2);
		glVertex3dv(C2z);
		glVertex3dv(D2z);
		glVertex3dv(D2);
		//7 wall
		glVertex3dv(C2);
		glVertex3dv(C2z);
		glVertex3dv(B2z);
		glVertex3dv(B2);
		//8 wall
		glVertex3dv(A);
		glVertex3dv(Az);
		glVertex3dv(B2z);
		glVertex3dv(B2);

	glEnd();

	//Выпуклость
	double x, y, m1[1000][3], m2[1000][3];
	int k = 0;
	double r = sqrt(29) / 2;
	for (double i = 0; i <= 360; i += 0.5) {
		x = r * cos(i * PI / 180) -1.5;
		y = r * sin(i * PI / 180) -8;
		if (y <=-0.4*x-8.6) {
			m1[k][0] = x;
			m1[k][1] = y;
			m1[k][2] = 0;

			m2[k][0] = x;
			m2[k][1] = y;
			m2[k][2] = 5;
			k++;
		}
	}	


	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.8, 0.7, 0.7,1.0);
		glVertex3dv(m1[i]);
		glVertex3dv(m1[i + 1]);
		glVertex3dv(m2[i + 1]);
		glVertex3dv(m2[i]);
		glEnd();
	}

	//верх и низ выпуклости
	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.8, 0.7, 0.7,1.0);
		glVertex3dv(m1[i]);
		glVertex3dv(B2);
		glVertex3dv(A);
		glVertex3dv(B1);
		glEnd();
	}

	for (int i = 0; i < k; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.8, 0.7, 0.7,1.0);
		glVertex3dv(m2[i]);
		glVertex3dv(B2z);
		glVertex3dv(Az);
		glVertex3dv(B1z);
		glEnd();
	}
	
	
	//Вогнутость
	double xg, yg, m3[1000][3], m4[1000][3];
	int kg = 0;
	//was 41
	float rg = sqrt(41) / 2;
	for (double i = 0; i <= 360; i += 0.5) {
		//-0.5
		xg = rg * cos(i * PI / 180) -0.5;
		//7
		yg = rg * sin(i * PI / 180) +7;
		//0.8x +7.4
		if (yg <= 0.8*xg + 7.4) {
			if (yg <= -7 * xg + 23) {


				m3[kg][0] = xg;
				m3[kg][1] = yg;
				m3[kg][2] = 0;

				m4[kg][0] = xg;
				m4[kg][1] = yg;
				m4[kg][2] = 5;
				kg++;
			}
		}
	}

	for (int i = 0; i < kg; i++) {
		glBegin(GL_QUADS);
		glColor4f(0.5, 0.6, 0.3, 1.0);
		glVertex3dv(m3[i]);
		glVertex3dv(m3[i + 1]);
		glVertex3dv(m4[i + 1]);
		glVertex3dv(m4[i]);
		glEnd();

	}

	//низ
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glVertex3dv(A);
	for (int i = 0; i < kg; i++) {
		if (m3[i][0] <= 0) {
			glVertex3dv(m3[i]);
		}
	}
	glVertex3dv(B);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glVertex3dv(A);
	for (int i = 0; i < kg; i++) {
		if (m3[i][0] >= 0) {
			glVertex3dv(m3[i]);
		}
	}
	glVertex3dv(B);
	glEnd();
	int numb = 0;
	//вверх
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glVertex3dv(Az);
	for (int i = 0; i < kg; i++) {
		if (m4[i][0] <= 0) {
			glVertex3dv(m4[i]);
		}
	}
	glVertex3dv(Bz);
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.5, 0.8, 0.3, 1.0);
	glVertex3dv(Az);
	for (int i = 0; i < kg; i++) {
		if (m4[i][0] >= 0) {
			glVertex3dv(m4[i]);
		}
		numb = i;
	}
	glVertex3dv(Bz);
	glEnd();

	glBegin(GL_QUADS);
	glVertex3dv(m3[numb]);
	glVertex3dv(m4[numb]);
	glVertex3dv(Bz);
	glVertex3dv(B);
	glEnd();
}


