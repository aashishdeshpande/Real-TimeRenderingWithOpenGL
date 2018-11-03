#include <windows.h>
#include <gl/GL.h>
#include<GL/glu.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
//Prototype Of WndProc() declared Globally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullscreen = false;

//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype
	void initialize(void);
	void uninitialize(void);
	void display(void);
	//variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("RTROGL");
	bool bDone = false;

	//code
	//initializing members of struct WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;

	//Registering Class
	RegisterClassEx(&wndclass);
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	//Create Window
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szClassName,
		TEXT("OpenGL Fixed Function Pipeline Using Native Windowing : SHAPES"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(x / 2) - (WIN_WIDTH / 2),
		(y / 2) - (WIN_HEIGHT / 2),
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	//initialize
	initialize();

	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//Message Loop
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				bDone = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true)
					bDone = true;
				display();
			}
		}
	}

	uninitialize();
	return((int)msg.wParam);
}

//WndProc()
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//function prototype

	void resize(int, int);
	void ToggleFullscreen(void);
	void uninitialize(void);

	//code
	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbActiveWindow = true;
		else
			gbActiveWindow = false;
		break;

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			gbEscapeKeyIsPressed = true;
			break;

		case 0x46: //for 'f' or 'F'
			if (gbFullscreen == false)
			{
				ToggleFullscreen();
				gbFullscreen = true;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = false;
			}
			break;
		default:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{
	//variable declarations
	MONITORINFO mi;

	//code
	if (gbFullscreen == false)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
	}

	else
	{
		//code
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);
	}
}

void initialize(void)
{
	//function prototypes
	void resize(int, int);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//Initialization of structure 'PIXELFORMATDESCRIPTOR'
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32; //32 corresponds to color buffer
	ghdc = GetDC(ghwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);//to clear the old depth buffer just ase we do for color buffer
	glEnable(GL_DEPTH_TEST);//to enable the z value
	glDepthFunc(GL_LEQUAL);//which function to use for for Depth Checking
	glShadeModel(GL_SMOOTH);//to remove aliasing
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);//to do the nicest correction for the problems occured due to perspective
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	void drawPoints(void);
	void drawTriangleLoop(void);
	void drawQuadStrip(void);
	void drawColoredQuads(void);
	void drawQuads(void);
	void drawQuadsWithDiagonalLines(void);
	void drawDiagonalsLines(void);
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	//gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(-1.5f, 0.3f, -3.0f);
	
	drawPoints();

	glLoadIdentity();
	//gluLookAt(0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(-1.5f,-0.3f, -3.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	drawTriangleLoop();

	glLoadIdentity();
	glTranslatef(0.5f, -0.6f, -3.0f);
	//drawQuadStrip();

	glLoadIdentity();
	glTranslatef(0.5f,0.6f,-3.0f);
	drawColoredQuads();

	glLoadIdentity();
	glTranslatef(-0.5f, 0.6f, -3.0f);
	drawQuads();

	glLoadIdentity();
	glTranslatef(-0.5f,-0.6f, -3.0f);
	drawQuadsWithDiagonalLines();

	glLoadIdentity();
	glTranslatef(0.5f, -0.6f, -3.0f);
	drawDiagonalsLines();

	SwapBuffers(ghdc);

}

void drawPoints(void)
{ 
	glColor3f(1.0f, 1.0f, 1.0f);
	GLfloat i = 0.0f;
	GLfloat j = 0.0f;
	glEnable(GL_POINT_SMOOTH);
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	for (i = 0.0; i < 0.8; i += 0.2)
	{
		for (j = 0.0; j < 0.8; j += 0.2)
		{
			glVertex3f(j, i, 0.0f);
		}
	}
	glEnd();
}
void drawTriangleLoop(void)
{
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(1.5f);
	glEnable(GL_LINE_SMOOTH);
	GLfloat i = 0.0f;
	GLfloat j = 0.0f;
	glBegin(GL_LINE_LOOP);
	/*glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f,- 0.2f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.4f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.6f, 0.0f);*/
	for (i = 0; i < 3; i++)
	{
		for (GLfloat j = -0.6f; j <= -0.2f; j += 0.2)
		{
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.5f, 0.0f, 0.0f);
			glVertex3f(0.5f, j, 0.0f);
		}
	}
	/////////////////////////////////////////////////////////////
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, -0.6f, 0.0f);
	glVertex3f(0.30f, -0.6f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.30f,-0.6f, 0.0f);
	glVertex3f(0.15f, -0.6f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.15f, -0.6f, 0.0f);
	glVertex3f(0.0f, -0.6f, 0.0f);
	glEnd();
}

void drawQuadStrip(void)
{
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.1f, 0.2f, 0.0f);
	glVertex3f(-0.1f, -0.2f, 0.0f);
	glVertex3f(0.1f, -0.2f, 0.0f);
	glVertex3f(0.1f, 0.2f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.3f, 0.2f, 0.0f);
	glVertex3f(0.3f, -0.2f, 0.0f);
	glVertex3f(0.5f, -0.2f, 0.0f);
	glVertex3f(0.5f, 0.2f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.7f, 0.2f, 0.0f);
	glVertex3f(0.7f,-0.2f,0.0f);
	glVertex3f(0.9f, -0.2f, 0.0f);
	glVertex3f(0.9f, 0.2f, 0.0f);
	glEnd();
}

void drawColoredQuads(void)
{
	void drawLines(void);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 0.3f, 0.0f);
	glVertex3f(0.0f, -0.3f, 0.0f);
	glVertex3f(0.2f, -0.3f, 0.0f);
	glVertex3f(0.2f, 0.3f, 0.0f);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(0.2, 0.3, 0.0);
	glVertex3f(0.2, -0.3, 0.0);
	glVertex3f(0.4, -0.3f, 0.0f);
	glVertex3f(0.4, 0.3, 0.0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(0.4, 0.3, 0.0);
	glVertex3f(0.4, -0.3, 0.0);
	glVertex3f(0.6, -0.3f, 0.0f);
	glVertex3f(0.6, 0.3, 0.0);
	glEnd();

	drawLines();
}

void drawLines(void)
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0.0f, 0.1f, 0.0f);
	glVertex3f(0.6, 0.1, 0.0);
	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.6, -0.1, 0.0);
	glEnd();

	//glBegin(GL_LINES);
	//glEnd();
}

void drawQuads(void)
{
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.3, 0.0f);
	glVertex3f(0.0, -0.3, 0.0);
	glVertex3f(0.6, -0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);
	////////////////////////
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);
	glVertex3f(0.0, -0.3, 0.0);
	glVertex3f(0.6, -0.3, 0.0);
	

	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.6, 0.1, 0.0);

	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.6, -0.1, 0.0);

	glVertex3f(0.2, 0.3, 0.0);
	glVertex3f(0.2,-0.3, 0.0);

	glVertex3f(0.4, 0.3, 0.0);
	glVertex3f(0.4, -0.3, 0.0);

	glEnd();
}

void drawQuadsWithDiagonalLines(void)
{
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.3, 0.0f);
	glVertex3f(0.0, -0.3, 0.0);
	glVertex3f(0.6, -0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);
	////////////////////////
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);
	glVertex3f(0.0, -0.3, 0.0);
	glVertex3f(0.6, -0.3, 0.0);


	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.6, 0.1, 0.0);

	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.6, -0.1, 0.0);

	glVertex3f(0.2, 0.3, 0.0);
	glVertex3f(0.2, -0.3, 0.0);

	glVertex3f(0.4, 0.3, 0.0);
	glVertex3f(0.4, -0.3, 0.0);

	glVertex3f(0.0,0.1,0.0);
	glVertex3f(0.2, 0.3, 0.0);

	glVertex3f(0.0,-0.1, 0.0);
	glVertex3f(0.4,0.3, 0.0);

	glVertex3f(0.0,-0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);

	glVertex3f(0.2, -0.3, 0.0);
	glVertex3f(0.6,0.1, 0.0);

	glVertex3f(0.4, -0.3, 0.0);
	glVertex3f(0.6, -0.1, 0.0);
	glEnd();
}

void drawDiagonalsLines(void)
{
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.3, 0.0f);
	glVertex3f(0.0, -0.3, 0.0);
	//glVertex3f(0.6, -0.3, 0.0);
	//glVertex3f(0.6, 0.3, 0.0);
	////////////////////////
	glVertex3f(0.0, 0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);
	//glVertex3f(0.0, -0.3, 0.0);
	//glVertex3f(0.6, -0.3, 0.0);


	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.6, 0.1, 0.0);

	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.6, -0.1, 0.0);

	glVertex3f(0.2, 0.3, 0.0);
	glVertex3f(0.2, -0.3, 0.0);

	glVertex3f(0.4, 0.3, 0.0);
	glVertex3f(0.4, -0.3, 0.0);

	glVertex3f(0.0, 0.1, 0.0);
	glVertex3f(0.2, 0.3, 0.0);

	glVertex3f(0.0, -0.1, 0.0);
	glVertex3f(0.4, 0.3, 0.0);

	glVertex3f(0.0, -0.3, 0.0);
	glVertex3f(0.6, 0.3, 0.0);

	glVertex3f(0.2, -0.3, 0.0);
	glVertex3f(0.6, 0.1, 0.0);

	glVertex3f(0.4, -0.3, 0.0);
	glVertex3f(0.6, -0.1, 0.0);
	glEnd();
}
void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
}

void uninitialize(void)
{
	//UNINITIALIZATION CODE

	if (gbFullscreen == true)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);

	}

	wglMakeCurrent(NULL, NULL);

	wglDeleteContext(ghrc);
	ghrc = NULL;

	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;
}

