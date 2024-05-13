

#include "Render.h"

#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <cmath>
#include <math.h>
#define PI 3.14159265

double f(double a, double b, double c, double d, double t)
{
	return (1 - t) * (1 - t) * (1 - t) * a + 3 * t * (1 - t) * (1 - t) * b + 3 * t * t * (1 - t) * c + t * t * t * d;
}
double f2(double a, double b, double c, double d, double t) {
	return(a * (2 * t * t * t - 3 * t * t + 1) + b * (-2 * t * t * t + 3 * t * t) + c * (t * t * t - 2 * t * t + t) + d * (t * t * t - t * t));
}
double* beze(double* P1, double* P2, double* P3, double* P4, double t) {
	double p[3];
	p[0] = f(P1[0], P2[0], P3[0], P4[0], t);
	p[1] = f(P1[1], P2[1], P3[1], P4[1], t);
	p[2] = f(P1[2], P2[2], P3[2], P4[2], t);
	return p;
}
double* ermyt(double* P1, double* P2, double* P3, double* P4, double t) {
	double R1[3] = { (P1[0] - P2[0]),(P1[1] - P2[1]) ,(P1[2] - P2[2]) };
	double R4[3] = { (P3[0] - P4[0]),(P3[1] - P4[1]) ,(P3[2] - P4[2]) };
	double p[3];
	p[0] = f2(P1[0], P3[0], R1[0], R4[0], t);
	p[1] = f2(P1[1], P3[1], R1[1], R4[1], t);
	p[2] = f2(P1[2], P3[2], R1[2], R4[2], t);
	return p;
}
double t_max = 0;
double ttime = 0.01;
void Render(double delta_time)
{
	t_max += ttime;
	if (t_max > 1) {
		ttime *= -1;
	} 
	if (t_max < 0) {
		ttime *= -1;
	} 
	double P1[] = { 0,0,0 }; 
	double P2[] = { -8,-6,1 };
	double P3[] = { 10,-10,0 };
	double P4[] = { 1,8,4 };

	double A[] = { 1, 1, 0 };
	double B[] = { -1, 1, 0 };
	double C[] = { -1,-1, 0 };
	double D[] = { 1,-1, 0 };

	double lastend[] = { 0,0,0 };

	glBegin(GL_LINES); 
	glVertex3dv(P1);
	glVertex3dv(P2);
	glVertex3dv(P2);
	glVertex3dv(P3);
	glVertex3dv(P3);
	glVertex3dv(P4);

	glEnd();
	glLineWidth(3); 
	glColor3d(0, 1, 0);
	glBegin(GL_LINE_STRIP);
	for (double t = 0; t <= 1.0001; t += 0.01) {
		glVertex3dv(beze(P1, P2, P3, P4, t));
	}
	glEnd();
	double p[] = { beze(P1, P2, P3, P4, t_max)[0], beze(P1, P2, P3, P4, t_max)[1],beze(P1, P2, P3, P4, t_max)[2] };
	glTranslated(p[0], p[1], p[2]);
	glBegin(GL_QUADS);
	glColor3d(0.2, 0.7, 0.7);
	glVertex3dv(A);
	glVertex3dv(B);
	glVertex3dv(C);
	glVertex3dv(D);

	glEnd();
	glColor3d(1, 0, 1);
	glLineWidth(1); 
	glBegin(GL_LINES); 
	glEnd();
}
