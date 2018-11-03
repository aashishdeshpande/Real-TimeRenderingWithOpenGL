#include<gl/freeglut.h>
#include<math.h>
#define WIN_WIDTH 600
#define WIN_HEIGHT 600
bool bFullscreen = false;
const float PI= 3.1415926535898;
int main(int argc, char **argv)
{
	void display(void);
	void resize(int, int);
	void keyboard(unsigned char, int, int);
	void mouse(int, int, int, int);
	void initialize(void);
	void uninitialize(void);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutInitWindowPosition((x / 2) - (WIN_WIDTH / 2), (y / 2) - (WIN_HEIGHT / 2));
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Glut:Horizontal Line");
	initialize();
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutCloseFunc(uninitialize);
	glutMainLoop();
	return(0);

}

void display(void)
{
	void drawGrid(void);
	void drawCircle(void);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawGrid();
	glLineWidth(3.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();

	glLineWidth(3.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();
	drawCircle();
	glutSwapBuffers();
}

void drawGrid(void)
{
	glLineWidth(1.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	for (float a = 0.05; a <= 1.0f; a = a + 0.05f)
	{
		glVertex3f(a, 1.0f, 0.0f);
		glVertex3f(a, -1.0f, 0.0f);
	}
	glEnd();


	glBegin(GL_LINES);
	for (float b = -0.05; b >= -1.0f; b = b - 0.05)
	{
		glVertex3f(b, 1.0f, 0.0f);
		glVertex3f(b, -1.0f, 0.0f);
	}
	glEnd();

	glBegin(GL_LINES);
	for (float c = 0.05f; c <= 1.0f; c = c + 0.05f)
	{
		glVertex3f(1.0f, c, 0.0f);
		glVertex3f(-1.0f, c, 0.0f);
	}
	glEnd();

	glBegin(GL_LINES);
	for (float d = -0.05; d >= -1.0f; d = d - 0.05f)
	{
		glVertex3f(1.0f, d, 0.0f);
		glVertex3f(-1.0f, d, 0.0f);
	}
	glEnd();
}
void drawCircle(void)
{
	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 0.0f);

	for (float angle = 0.0f; angle < 2.0f*PI; angle = angle + 0.01f)
	{
		glVertex3f(cos(angle), sin(angle), 0.0f);
	}
	glEnd();
}
void initialize(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glutLeaveMainLoop();
		break;

	case 'F':
	case 'f':
		if (bFullscreen == false)
		{
			glutFullScreen();
			bFullscreen = true;
		}
		else
		{
			glutLeaveFullScreen();
			bFullscreen = false;
		}
		break;

	default:
		break;
	}
}

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		break;
	default:
		break;
	}
}

void resize(int width, int height)
{
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void uninitialize(void)
{

}
