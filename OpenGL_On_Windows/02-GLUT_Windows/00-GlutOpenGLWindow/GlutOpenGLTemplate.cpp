#include<gl/freeglut.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
bool bFullscreen = false;

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
	glutCreateWindow("OpenGL First Window: Hello World!!");
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
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void initialize(void)
{
	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
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

}

void uninitialize(void)
{

}