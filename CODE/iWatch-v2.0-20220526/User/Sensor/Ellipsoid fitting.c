#include "Ellipsoid fitting.h"
#include "stdio.h"
#include "string.h"
#include "math.h"

#define MATRIX_SIZE 6
float m_matrix[MATRIX_SIZE][MATRIX_SIZE + 1];	//ϵ������
float final_solve[MATRIX_SIZE] = {0};				//������Ľ��Ӧ��С������������еģ�a��b��c��d��e��f��
float m_result[MATRIX_SIZE];
int N = 0;//�����ۼƵĲ����������

//ȡ����ֵ
float Abs(float a)
{
	return a < 0 ? -a : a;
}

//�Ѿ���ϵ��ȫ�����Ϊ0
void ResetMatrix(void)
{
	unsigned char row, column;
	for (row = 0; row < MATRIX_SIZE; row++)
	{
		for (column = 0; column < MATRIX_SIZE + 1; column++)
			m_matrix[row][column] = 0.0f;
	}
	N = 0;
}

//����������������ɾ����Ԫ�ص��ܺ�
void CalcData_Input(float x, float y, float z)
{
	float V[MATRIX_SIZE + 1];
	unsigned char row, column;
	N++;
	V[0] = y*y;
	V[1] = z*z;
	V[2] = x;
	V[3] = y;
	V[4] = z;
	V[5] = 1.0;
	V[6] = -x*x;
	//����ϵ�����󣬲������ۼ�
	for (row = 0; row < MATRIX_SIZE; row++)
	{
		for (column = 0; column < MATRIX_SIZE + 1; column++)
		{
			m_matrix[row][column] += V[row] * V[column];
		}
	}
	//b������m_matrix[row][6]
}

//����ϵ�����󣬰ѳ���N����
void CalcData_Input_average()
{
	unsigned char row, column;
	for (row = 0; row < MATRIX_SIZE; row++)
		for (column = 0; column < MATRIX_SIZE + 1; column++)
			m_matrix[row][column] /= N;
	//b������m_matrix[row][6]
}
//��ʾ����ϵ��������������[A|b]
/*
void DispMatrix(void)
{
	unsigned char row, column;
	for (row = 0; row < MATRIX_SIZE; row++)
	{
		for (column = 0; column < MATRIX_SIZE + 1; column++)
		{
			printf("%23f ", m_matrix[row][column]);
			if (column == MATRIX_SIZE - 1)
				printf("|");
		}
		printf("\r\n");
	}
	printf("\r\n\r\n");
}
*/
//��������Ԫ��λ��
void Row2_swop_Row1(int row1, int row2)
{
	float tmp = 0;
	unsigned char column;
	for (column = 0; column < MATRIX_SIZE + 1; column++)
	{
		tmp = m_matrix[row1][column];
		m_matrix[row1][column] = m_matrix[row2][column];
		m_matrix[row2][column] = tmp;
	}
}

//�ð�row�е�Ԫ�س���һ��ϵ��k
void k_muiltply_Row(float k, int row)
{
	unsigned char column;
	for (column = 0; column < MATRIX_SIZE + 1; column++)
		m_matrix[row][column] *= k;
}

//��һ��������row1�мӵ�row2����ȥ
void Row2_add_kRow1(float k, int row1, int row2)
{
	unsigned char column;
	for (column = 0; column < MATRIX_SIZE + 1; column++)
		m_matrix[row2][column] += k*m_matrix[row1][column];
}


//����Ԫ����k����Ԫ֮ǰ����k�е�MATRIX_SIZE�������н���ð���ų���������������k�е�Ԫ�صĴ�С
void MoveBiggestElement_to_Top(int k)
{
	int row = 0, column = 0;
	for (row = k + 1; row < MATRIX_SIZE; row++)
	{
		if (Abs(m_matrix[k][k]) < Abs(m_matrix[row][k]))
		{
			Row2_swop_Row1(k, row);
		}
	}
}

//��˹��Ԫ�������н����;���
unsigned char Matrix_GaussElimination(void)
{
	float k = 0;
	unsigned char cnt,row;
	for (cnt = 0; cnt < MATRIX_SIZE; cnt++)//���е�k�ε����㣬��Ҫ�����k�����µ�������k�е�Ԫ�ض����0
	{
		//��k������k�е�Ԫ�ش�С����������
		MoveBiggestElement_to_Top(cnt);
		if (m_matrix[cnt][cnt] == 0)
			return(1);			//����ֵ��ʾ����
		//��k���������Ԫ��ȫ������0�����б仯
		for (row = cnt + 1; row < MATRIX_SIZE; row++)
		{
			k = -m_matrix[row][cnt] / m_matrix[cnt][cnt];
			Row2_add_kRow1(k, cnt, row);
		}
		//DispMatrix();
	}
	return 0;
}

//��������;��󣬼��ѶԽ��ߵ�Ԫ��ȫ������1
void Matrix_RowSimplify(void)
{
	float k = 0;
	unsigned char row;
	for (row = 0; row < MATRIX_SIZE; row++)
	{
		k = 1 / m_matrix[row][row];
		k_muiltply_Row(k, row);
	}
	//DispMatrix();
}

//���������Է�����Ľ�
void Matrix_Solve(float *solve)
{
	long row;
	unsigned char column;
	for (row = MATRIX_SIZE - 1; row >= 0; row--)
	{
		solve[row] = m_matrix[row][MATRIX_SIZE];
		for (column = MATRIX_SIZE - 1; column >= row + 1; column--)
			solve[row] -= m_matrix[row][column] * solve[column];
	}
	//printf("  a = %.3f| b = %.3f| c = %.3f| d = %.3f| e = %.3f| f = %.3f ", solve[0], solve[1], solve[2], solve[3], solve[4], solve[5]);
	//printf("\r\n");
	//printf("\r\n");
}

//��������У׼�Ĺ���
unsigned char Ellipsoid_fitting_Process(tsCaliData *dat)
{
	float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
	float X0 = 0, Y0 = 0, Z0 = 0, A = 0, B = 0, C = 0;
	//ResetMatrix();
	//�����������������ٶȲ����������������Ͼ��ȷֲ�
	/*
	CalcData_Input(87, -52, -4454);
	CalcData_Input(301, -45, 3859);
	CalcData_Input(274, 4088, -303);
	CalcData_Input(312, -4109, -305);
	CalcData_Input(-3805, -24, -390);
	CalcData_Input(4389, 6, -228);
	CalcData_Input(261, 2106, -3848);
	CalcData_Input(327, -2047, -3880);
	CalcData_Input(-1963, -13, -3797);
	CalcData_Input(3024, 18, -3449);
	*/
	CalcData_Input_average();				//����������ݵ�����Ԫ�ؽ��й�һ��
	//DispMatrix();										//��ʾԭʼ���������
	if (Matrix_GaussElimination())	//����н����ξ���
	{
		printf("the marix could not be solved\r\n");
		return 0;
	}
	else
	{
		Matrix_RowSimplify();				//���������̬
		Matrix_Solve(final_solve);	//���a,b,c,d,e,f
		a = final_solve[0];
		b = final_solve[1];
		c = final_solve[2];
		d = final_solve[3];
		e = final_solve[4];
		f = final_solve[5];

		X0 = -c / 2;
		Y0 = -d / (2 * a);
		Z0 = -e / (2 * b);
		A = sqrt(X0*X0 + a*Y0*Y0 + b*Z0*Z0 - f);
		B = A / sqrt(a);
		C = A / sqrt(b);
		
		dat->X0 = X0;
		dat->Y0 = Y0;
		dat->Z0 = Z0;
		dat->A = A;
		dat->B = B;
		dat->C = C;
		/*
		printf("((x - x0) / A) ^ 2 + ((y - y0) / B) ^ 2 + ((z - z0) / C) ^ 2 = 1 Ellipsoid result as below��\r\n");
		printf("\r\n");
		printf("X0 = %.3f| Y0 = %.3f| Z0 = %.3f| A = %.3f| B = %.3f| C = %.3f \r\n", X0, Y0, Z0, A, B, C);
		*/
		return 1;
	}
}