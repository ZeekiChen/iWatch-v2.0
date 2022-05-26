#ifndef _ELLIPSOID_FITTING_H
#define _ELLIPSOID_FITTING_H

typedef struct{
	int X0;
	int Y0;
	int Z0;
	float A;
	float B;
	float C;
}tsCaliData;

void ResetMatrix(void);
void CalcData_Input(float x, float y, float z);
unsigned char Ellipsoid_fitting_Process(tsCaliData *dat);

#endif