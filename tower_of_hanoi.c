#include <stdlib.h>
#include <stdio.h>
#include "glut.h"
#include "gl.h"
#include "glu.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
int status = 0;
int pile = 0;
double me_x = 0, me_y = -30, me_z = 10;	//���_�̍��W
/*
double ring_position[10] = { 1,1,1,1,1,1,1,1,1,1 };	//ring�̈ʒu(�ԍ���ring�̑傫���ɑΉ�����)
double ring_height[10] = { 10,9,8,7,6,5,4,3,2,1 };	//ring�̍���(�ԍ���ring�̑傫���ɑΉ�����)
double ring_radius[10] = { 1,2,3,4,5,6,7,8,9,10 };	//ring�̃T�C�Y���a(�ԍ���ring�̑傫���ɑΉ�����)
*/
double pointing = -1;
//double field_position[11][3] = 						//[10-ring_height][ring_position-1]�����ɒu�����ring�̃T�C�Y
//{ {1,0,0},{2,0,0},{3,0,0},{4,0,0},{5,0,0},{6,0,0},{7,0,0},{8,0,0},{9,0,0},{10,0,0}, {11,11,11} };//11�͒n��
double held_ring = 0;						//�����Ă���ring�̃T�C�Y;
double holding = 0;
int Count=0;			//����������
int Difficulty=0;//��Փx��ݒ�ł���悤�ɂ���

double ring_position[10] = { 0,0,0,0,0,0,0,0,0,0 };	//ring�̈ʒu(�ԍ���ring�̑傫���ɑΉ�����)
double ring_height[10] = { 0,0,0,0,0,0,0,0,0,0 };	//ring�̍���(�ԍ���ring�̑傫���ɑΉ�����)
double ring_radius[10] = { 0,0,0,0,0,0,0,0,0,0 };	//ring�̃T�C�Y���a(�ԍ���ring�̑傫���ɑΉ�����)

double field_position[11][3] = { {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {0,0,0}, {11,11,11} };// field_position�̉��ǔ�

//�����̐ݒ�
void SetAmbient(float a, float b, float c)
{
	float ambient[] = { a,b,c,1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
}

//�g�U���ˌ��̐ݒ�
void SetDiffuse(float a, float b, float c)
{
	float diffuse[] = { a / 255, b / 255, c / 255, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
}

//���ʔ��ˌ��̐ݒ�
void SetSpecular(float a, float b, float c)
{
	float specular[] = { a / 255,b / 255,c / 255,1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}

//���ʔ��ˌ��̉s���̐ݒ�
void SetShineness(float a)
{
	glMaterialf(GL_FRONT, GL_SHININESS, a);
}

//�|���S���̐F��F�ʂ�3�v�f�Őݒ�
void SetDiffuse_HSV(float H, float S, float V)
{
	// H�@0-360
	// S , V 0-100
	float MAX = V * 2.25;
	float MIN = MAX - S * MAX / 100;
	float R = 0, G = 0, B = 0;

	if (0 <= H && H < 60)
	{
		R = MAX;
		G = H * (MAX - MIN) / 60 + MIN;
		B = MIN;
	}
	else if (60 <= H && H < 120)
	{
		R = (120 - H) * (MAX - MIN) / 60 + MIN;
		G = MAX;
		B = MIN;
	}
	else if (120 <= H && H < 180)
	{
		R = MIN;
		G = MAX;
		B = (H - 120) * (MAX - MIN) / 60 + MIN;
	}
	else if (180 <= H && H < 240)
	{
		R = MIN;
		G = (240 - H) * (MAX - MIN) / 60 + MIN;
		B = MAX;
	}
	else if (240 <= H && H < 300)
	{
		R = (120 - H) * (MAX - MIN) / 60 + MIN;
		G = MIN;
		B = MAX;
	}
	else if (300 <= H && H <= 360)
	{
		R = MAX;
		G = MIN;
		B = (360 - H) * (MAX - MIN) / 60 + MIN;
	}
	SetDiffuse(R, G, B);
}
/************************************************/
void place()//�u���ꏊ�̕`��
{
	double i = 0;
	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0.3);
	for (i = 0; i < 2; i += 0.1)
	{
		glVertex2f(cos(M_PI * i), sin(M_PI * i));
	}
	glEnd();
}
/************************************************/
void ring(int x, double z, double r)//�g�[���X�쐬
/*
�q ��P���� x �r
�P�F ��  �ɔz�u
�Q�F���� �ɔz�u
�R�F �E  �ɔz�u

�q ��Q���� z �r
�g�[���X��`�悷�鍂��
double�^�����C10�i�K�̐����l1�`10�Ŏw��
0�̂Ƃ��͕`����Ȃ�

�q ��R���� r �r
�g�[���X�̑傫��
double�^�����C10�i�K�̐����l1�`10�Ŏw��
*/
{
	if (r == 10) SetDiffuse_HSV(324, 60, 100);
	else SetDiffuse_HSV((r - 1) * 30, 60, 100);
	glPushMatrix();
	switch (x)
	{
	case 1:
		glTranslated(-8, 0, z / 2);
		break;
	case 2:
		glTranslated(0, 0, z / 2);
		break;
	case 3:
		glTranslated(8, 0, z / 2);
		break;
	}
	if (r > 0)glutSolidTorus(0.25, 1 + 2 * (r - 1) / 9, 60, 60);
	else;
	glPopMatrix();
}
/************************************************/
void Data_Set(){}
void Set(int Dif)
{
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		ring_position[i] = 0;	//ring�̈ʒu(�ԍ���ring�̑傫���ɑΉ�����)
		ring_height[i] = 0;	//ring�̍���(�ԍ���ring�̑傫���ɑΉ�����)
		ring_radius[i] = 0;	//ring�̃T�C�Y���a(�ԍ���ring�̑傫���ɑΉ�����)
		field_position[i][0] = 0;
		field_position[i][1] = 0;
		field_position[i][2] = 0;
	}

	for (i = 0; i < Dif; i++)
	{
		ring_position[i] = 1;	//ring�̈ʒu(�ԍ���ring�̑傫���ɑΉ�����) 0�̏ꍇ�͑��݂��Ȃ�
		ring_height[i] = Dif - i;	//ring�̍���(�ԍ���ring�̑傫���ɑΉ�����) 0�̏ꍇ�͑��݂��Ȃ�
		ring_radius[i] = i + 1;	//ring�̃T�C�Y���a(�ԍ���ring�̑傫���ɑΉ�����) 0�̏ꍇ�͑��݂��Ȃ�
	}

	for (pile = 9, i = 0; i < Dif; pile--, i++)
	{
		field_position[pile][0] = Dif - i;
		field_position[pile][1] = 0;
		field_position[pile][2] = 0;
	}
	Count = 0;
	holding = 0;
	held_ring = 0;
	pointing = -1;
}
/************************************************/
void Pointing(double r)//ring�̑I��
{
	SetDiffuse(255, 0, 0);
	glPushMatrix();
	glTranslatef(8 * pointing, 0, 10);
	glRotatef(180, 1, 0, 0);
	glutSolidCone(0.5, 1, 50, 50);
	glPopMatrix();

	ring(pointing + 2, 15, r);
}

void C(int RED,int GREEN, int BLUE)
{
	float i=0;
	float fine = 20;
	glBegin(GL_QUAD_STRIP);
	glColor3f((float)RED/255, (float)GREEN/255, (float)BLUE/255);
	for (i = 0; i <= fine; i++)
	{
		glVertex3f(cos(M_PI/ fine *i),0, sin(M_PI / fine * i)+0.5);
		glVertex3f(2*cos(M_PI/ fine *i),0, 2*sin(M_PI / fine * i)+0.5);
	}
	for (i = fine; i <= fine*2; i++)
	{
		glVertex3f(cos(M_PI/ fine *i),0, sin(M_PI / fine * i)-0.5);
		glVertex3f(2*cos(M_PI/ fine *i),0, 2*sin(M_PI / fine * i)-0.5);
	}
	glEnd();
}

void L(int RED,int GREEN, int BLUE)
{
	glBegin(GL_QUAD_STRIP);
	glColor3f((float)RED / 255, (float)GREEN / 255, (float)BLUE / 255);
	glVertex3f(-2,0,2.5);
	glVertex3f(-1,0,2.5);
	glVertex3f(-2,0,-2.5);
	glVertex3f(-1,0,-1.5);
	glVertex3f(1.5,0,-2.5);
	glVertex3f(1.5,0,-1.5);
	glEnd();
}

void E(int RED, int GREEN, int BLUE)
{
	glBegin(GL_QUADS);
	glColor3f((float)RED / 255, (float)GREEN / 255, (float)BLUE / 255);
	glVertex3f(-2, 0, 2.5);
	glVertex3f(-1, 0, 2.5);
	glVertex3f(-1, 0, -2.5);
	glVertex3f(-2, 0, -2.5);

	glVertex3f(-2, 0, 2.5);
	glVertex3f(-2, 0, 1.5);
	glVertex3f(1.5,0,1.5);
	glVertex3f(1.5, 0, 2.5);

	glVertex3f(-2, 0, 0.5);
	glVertex3f(-2, 0, -0.5);
	glVertex3f(1.5, 0, -0.5);
	glVertex3f(1.5, 0, 0.5);

	glVertex3f(-2, 0, -1.5);
	glVertex3f(-2, 0, -2.5);
	glVertex3f(1.5, 0, -2.5);
	glVertex3f(1.5, 0, -1.5);

	glEnd();
}

void A(int RED, int GREEN, int BLUE)
{
	glBegin(GL_QUAD_STRIP);
	glColor3f((float)RED / 255, (float)GREEN / 255, (float)BLUE / 255);

	glVertex3f(-2, 0, -2.5);
	glVertex3f(-1, 0, -2.5);

	glVertex3f(-0.5, 0, 2.5);
	glVertex3f(0.5, 0, 2.5);

	glVertex3f(1, 0, -2.5);
	glVertex3f(2, 0, -2.5);
	
	glEnd();
	glBegin(GL_QUADS);

	glVertex3f(1,0,-0.5);
	glVertex3f(1,0,-1.5);
	glVertex3f(-1,0,-1.5);
	glVertex3f(-1,0,-0.5);

	glEnd();
}

void R(int RED, int GREEN, int BLUE)
{

	float i = 0;
	float fine = 20;	
	glBegin(GL_QUAD_STRIP);
	glColor3f((float)RED / 255, (float)GREEN / 255, (float)BLUE / 255);


	glVertex3f(-2,0,-2.5);
	glVertex3f(-1,0,-2.5);
	glVertex3f(-2,0,2.5);
	glVertex3f(-1,0,1.5);

	for (i=0;i<=20;i++)
	{
		glVertex3f(1.5*sin(M_PI / fine * i), 0, 1.5*cos(M_PI / fine * i) + 1);
		glVertex3f(0.5*sin(M_PI / fine * i), 0, 0.5*cos(M_PI / fine * i) + 1);
	}

	glVertex3f(-1, 0, -0.5);
	glVertex3f(-1, 0, 0.5);

	for (i = 0; i <= 10; i++)
	{
		glVertex3f(0.5*sin(M_PI / fine * i), 0, 0.5*cos(M_PI / fine * i) - 1);
		glVertex3f(1.5*sin(M_PI / fine * i), 0, 1.5*cos(M_PI / fine * i) - 1);
	}

	glVertex3f(0.5, 0, -2.5);
	glVertex3f(1.5, 0, -2.5);

	glEnd();

}

/*******************************************************///display
void display(void)
{
	double m = 0;
	int i = 0;
	//float light[] = { me_x,me_y,me_z,1 }; //�����̂��߂̏��
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gluLookAt(me_x, me_y, me_z, 0, 0, 2, 0, 0, 1);
	//�����ɕ`��
	glEnable(GL_DEPTH_TEST);

	/****************************************///�X�e�[�W�`��
	glBegin(GL_POLYGON);
	glColor3f(0.7, 0.8, 0.3);
	glVertex2f(-12, -5);
	glVertex2f(12, -5);
	glVertex2f(12, 5);
	glVertex2f(-12, 5);
	glEnd();

//�N���A���̏���
	if (field_position[9][0] == 0 && field_position[9][1] == 0 && held_ring == 0)
	{
	glPushMatrix();
	glTranslatef(-8.5,0.5,10);
	C(255,255,255);
	glTranslatef(4.5, 0, 0);
	L(255,255,255);
	glTranslated(4, 0, 0);
	E(255,255,255);
	glTranslated(4, 0, 0);
	A(255,255,255);
	glTranslatef(4.5, 0, 0);
	R(255,255,255);
	glPopMatrix();
	}

	for (i = -8; i <= 8; i += 8)		//�u���ꏊ��x���W�͂��ꂼ�� -H,0,H
	{
		glPushMatrix();
		glTranslatef(i, 0, 0.1);
		place();
		glPopMatrix();
	}
	/****************************************/
	glEnable(GL_LIGHTING);//�A�e��t�������ꍇ�͂����ɕ`��(�����Ƀ|���S����`�悷��ƃV���G�b�g�ɂȂ�)
	/*
	double ring_position[10] = { 1,1,1,1,1,1,1,1,1,1 };	//1,2,3������(�ԍ���ring�̑傫���ɑΉ�����)
	double ring_height[10] = { 10,9,8,7,6,5,4,3,2,1 };	//�ԍ���ring�̑傫���ɑΉ�����
	double field_position[i][0] [ring_height][ring_position]�����ɒu�����ring�̃T�C�Y���w�肷��
	*/

	for (i = 0; i < Difficulty; i++)
	{
		ring(ring_position[i], ring_height[i], ring_radius[i]);
		// ring_position ��ڂ� ���� ring_height �� �傫�� ring_radius �� ring ��`�悷��
	}

	//	for (i = 0; i < 10; i++)ring((int)ring_position[i], (int)ring_height[i], (int)field_position[i][(int)pointing+1]);

	Pointing(held_ring);


	/****************************************/
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();
	glutSwapBuffers();
	glFlush();
}

//------------------------------------
//����L�[�ɑ΂���R�[���o�b�N�֐�
//------------------------------------
void TouchKey_S(int key, int x, int y)
{

	switch (key) {
	case GLUT_KEY_LEFT:		//�I��
		if (pointing != -1)
		{
			pointing -= 1;
			if (held_ring != 0) 
			{
				ring_position[(int)held_ring - 1] = pointing + 2;
			}
			status = 1;//field_position�̏�Ԃ�����
		}
		if (pointing == -1) pointing -= 0;
		break;
	case GLUT_KEY_RIGHT:	//�I��


		if (pointing != 1)
		{
			pointing += 1;
			if (held_ring != 0)
			{
				ring_position[(int)held_ring - 1] = pointing + 2;
			}
			status = 1;//field_position�̏�Ԃ�����
		}
		if (pointing == 1) pointing += 0;
		break;
	case GLUT_KEY_DOWN:		//ring��ς݂���
		pile = 0;//�ŏ�i���画��
		if (held_ring != 0)//ring�������Ă��邱�Ƃ�O��
		{
			for (pile = 0; pile < 10; pile++)//��ɂ���P�ԏ��ring�܂��͒n�ʂɓ�����܂�pile��1�����₷
			{
				if (field_position[pile + 1][(int)pointing + 1] != 0) break;
			}
			if (held_ring < field_position[pile + 1][(int)pointing + 1])
			{
				ring_height[(int)held_ring - 1] = 10 - pile;//�����O��ςݏd�˂鍂����ݒ肷�鏈��
				ring_radius[(int)held_ring - 1] = held_ring;//�ςݏd�˂�ʒu�ɁA�����Ă��郊���O�̑傫������
				field_position[pile][(int)pointing + 1] = held_ring;//�ςݏd�˂�ʒu�̐���ݒ肷��
				held_ring = 0;//�����グ�Ă��郊���O�̑傫����0�ɂ��Č����Ȃ����鏈��
				Count++;
				status = 1;//field_position�̏�Ԃ�����
			}

		}
		break;
	case GLUT_KEY_UP:		//ring����������
		pile = 0;	//�ŏ�i���画��
		if (held_ring == 0)	//ring�������Ă��Ȃ�����O��
		{
			for (pile = 0; pile < 10; pile++)
			{
				if (field_position[pile][(int)pointing + 1] != 0)
				{
					held_ring = field_position[pile][(int)pointing + 1];//�����グ�Ă��郊���O�̑傫���������ĕ`�悷��
					field_position[pile][(int)pointing + 1] = 0;//���X�����O���������ʒu�̐���0�ɂ���
					ring_height[(int)held_ring - 1] = 0;//�����O�̍�����0�i�����グ����ԁj�ɂ��鏈��
					ring_radius[(int)held_ring - 1] = 0;//���Ƃ��Ƃ����������O�̕`����A�傫��0�ɂ��Č����Ȃ����鏈��
				}
				if (held_ring != 0)break;
			}
		status = 1;//field_position�̏�Ԃ�����
		}
		break;
	case GLUT_KEY_F1:
		Difficulty = 1;
		Set(1); status = 1;break;
	case GLUT_KEY_F2:
		Difficulty = 2;
		Set(2); status = 1;break;
	case GLUT_KEY_F3:
		Difficulty = 3;
		Set(3); status = 1;break;
	case GLUT_KEY_F4:
		Difficulty = 4;
		Set(4); status = 1;break;
	case GLUT_KEY_F5:
		Difficulty = 5;
		Set(5); status = 1;break;
	case GLUT_KEY_F6:
		Difficulty = 6;
		Set(6); status = 1;break;
	case GLUT_KEY_F7:
		Difficulty = 7;
		Set(7); status = 1;break;
	case GLUT_KEY_F8:
		Difficulty = 8;
		Set(8); status = 1;break;
	case GLUT_KEY_F9:
		Difficulty = 9;
		Set(9); status = 1;break;
	case GLUT_KEY_F10:
		Difficulty = 10;
		Set(10); status = 1;break;
	}
	glutPostRedisplay();
}

//------------------------------------
//�ʏ�̃L�[�ɑ΂���R�[���o�b�N�֐�
//------------------------------------

void TouchKey_N(unsigned char key, int x, int y)
{
	switch (key) {
	case 'z':
		break;
	case 'x':
		break;
	}
	glutPostRedisplay();
}

void myInit(char* progname)
{
	int width = 700, height = 700;
	float aspect = (float)width / (float)height;

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutCreateWindow(progname);

	glClearColor(0.0, 0.5, 1.0, 1.0);
	glutSpecialFunc(TouchKey_S);
	glutKeyboardFunc(TouchKey_N);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 40.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_LIGHT0);
	//Data_Set();
}

void idle(void)
{
	int  b = 0;
	/*�m�F�p******************************************************************/
	if (status == 1)
	{
		printf("----------STATUS----------\n");
		for (b = 0; b < 11; b++)
			printf("%2.0f,%2.0f,%2.0f\n", field_position[b][0], field_position[b][1], field_position[b][2]);
		printf("\n");
		for (b = 0; b < 10; b++)
			printf("RING %d ��� POSITION�F%.0f�@HEIGHT�F%.0f\n", b+1, ring_position[b], ring_height[b]);
		printf("\n");
			printf("HOLDING �F %d, POINTING �F %d, MOVE COUNT �F %d\n",(int)held_ring,(int)pointing+2,Count);
		printf("--------------------------\n\n");
		status = 0;
	}
	/*************************************************************************/
	glutPostRedisplay();
}



int main(int argc, char** argv)
{
	int i=0;
	int message;
	srand((unsigned int) time(NULL));
	message = rand() % 3;
	while (Difficulty > 10 || Difficulty <= 0)
	{
		printf("1�`10�܂ł̓�Փx����͂��ĉ������F");
		scanf("%d", &Difficulty);
		if (Difficulty > 10 || Difficulty <= 0) 
				printf("\n���s�ł��܂���\n\n");
	}

	Set(Difficulty);

/*
	for(i=0;i<Difficulty;i++)
	{
	ring_position[i] = 1;	//ring�̈ʒu(�ԍ���ring�̑傫���ɑΉ�����) 0�̏ꍇ�͑��݂��Ȃ�
	ring_height[i] = Difficulty-i;	//ring�̍���(�ԍ���ring�̑傫���ɑΉ�����) 0�̏ꍇ�͑��݂��Ȃ�
	ring_radius[i] = i+1;	//ring�̃T�C�Y���a(�ԍ���ring�̑傫���ɑΉ�����) 0�̏ꍇ�͑��݂��Ȃ�
	}
	
	for (pile = 9, i = 0; i < Difficulty; pile--, i++)
	{
		field_position[pile][0] = Difficulty-i;
	}
*/
	glutInit(&argc, argv);
	myInit(argv[0]);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}
