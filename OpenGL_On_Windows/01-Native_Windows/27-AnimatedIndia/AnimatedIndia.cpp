#include <windows.h>
#include <gl/GL.h>
#include <gl\GLU.h>
#include<mmsystem.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"winmm.lib")
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
GLint gflagOfI = false;
GLint gflagOfN = false;
GLint gflagOfIn = false;
GLint gflagOfD = false;
GLint gflagOfA = false;
GLfloat xExtreme = -2.0;
GLfloat yExtremeOfI = 2.5f;
GLfloat yExtremeOfIn = -2.5f;
GLfloat xExtremeLeft = 3.0f;
GLfloat ExtremeLeftForTriColor = -5.0f;
GLfloat Orange = 0.0f;
GLfloat OrGre = 0.0f;
GLfloat Green = 0.0f;

GLfloat gHeadOfx = -3.0f;
GLfloat gTailOfx= -3.0f;


//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype
	void initialize(void);
	void uninitialize(void);
	void display(void);
	//void spin(void);

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
		TEXT("OpenGL Fixed Function Pipeline:Animated INDIA"),
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
	void display(void);
	void resize(int, int);
	void ToggleFullscreen(void);
	void uninitialize(void);

	//code
	switch (iMsg)
	{

	case WM_CREATE:
		PlaySound(TEXT("SJA.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			gbActiveWindow = true;
		else
			gbActiveWindow = false;
		break;
		//case WM_PAINT:
		//display();
		//break;
		//case WM_ERASEBKGND:
		//return(0);
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
	pfd.cDepthBits = 24;

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
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//code
	void drawI(void);
	void drawN(void);
	void drawD(void);
	void drawIn(void);
	void drawA(void);
	void drawTricolor(void);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glLineWidth(3.0f);
	glEnable(GL_LINE_SMOOTH);

	glPushMatrix();

	glPushMatrix();
	if (xExtreme <= -1.5f)
	{
		xExtreme = xExtreme + 0.0001f;
	}
	else
	{
		gflagOfI = true;
	}
	glTranslatef(xExtreme, 0.0f, 0.0f);
	drawI();
	glPopMatrix();

	glPushMatrix();
	if (gflagOfI == true)
	{

		if (yExtremeOfI >= 0.0f)
		{
			yExtremeOfI = yExtremeOfI - 0.0003f;
		}
		else
		{
			gflagOfN = true;
		}
	}
	glTranslatef(-1.0f, yExtremeOfI, 0.0f);///-1.0f
	drawN();
	glPopMatrix();


	glPushMatrix();

	if (gflagOfN == true)
	{
		//glTranslatef(-0.25f, 0.0f, -3.0f);
		if (Orange < 1.0f && OrGre < 0.5f && Green < 1.0f)
		{
			Orange = Orange + 0.002f;
			OrGre = OrGre + 0.001f;
			Green = Green + 0.002f;
		}
		else
		{
			gflagOfD = true;
		}
	}
	glTranslatef(-0.25f, 0.0f, 0.0f);//-0.25
	drawD();
	glPopMatrix();


	glPushMatrix();
	if (gflagOfD == true)
	{

		if (yExtremeOfIn < 0.0f)
			yExtremeOfIn = yExtremeOfIn + 0.0003f;
		else
		{
			gflagOfIn = true;
		}
	}

	glTranslatef(0.25f, yExtremeOfIn, 0.0f);//0.25
	drawIn();
	glPopMatrix();


	glPushMatrix();
	if (gflagOfIn == true)
	{
		//glTranslatef(xExtremeLeft, 0.0f, -3.0f);
		if (xExtremeLeft > 1.0f)
			xExtremeLeft = xExtremeLeft - 0.0005f;
		else
		{
			gflagOfA = true;
		}
	}
	glTranslatef(xExtremeLeft, 0.0f, 0.0f);
	drawA();

	glPopMatrix();

	glPushMatrix();
	if (gflagOfA == true)
	{
		drawTricolor();
		/*if (ExtremeLeftForTriColor <= 0.5f)
		{
		ExtremeLeftForTriColor = ExtremeLeftForTriColor + 0.005f;
		}*/
	}
	/*glTranslatef(ExtremeLeftForTriColor,0.0f, 0.0f);
	drawTricolor();
	glPopMatrix();*/

	glPopMatrix();

	SwapBuffers(ghdc);
}


void drawI(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0, -1.0f, 0.0f);
	glEnd();
}

void drawN()
{

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.25f, -1.0f, 0.0f);

	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);


	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.25f, -1.0f, 0.0f);


	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.25f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.25f, -1.0f, 0.0f);


	glEnd();

}

void drawD(void)
{

	glBegin(GL_LINES);

	glColor3f(Orange, OrGre, 0.0f);
	glVertex3f(-0.25f, 1.0f, 0.0f);

	glColor3f(0.0f, Green, 0.0f);
	glVertex3f(-0.25f, -1.0f, 0.0f);

	glColor3f(Orange, OrGre, 0.0f);
	glVertex3f(-0.30f, 1.0f, 0.0f);


	glColor3f(Orange, OrGre, 0.0f);
	glVertex3f(0.255f, 1.0f, 0.0f);


	glColor3f(Orange, OrGre, 0.0f);
	glVertex3f(0.25f, 1.0f, 0.0f);

	glColor3f(0.0f, Green, 0.0f);
	glVertex3f(0.25f, -1.0f, 0.0f);

	glColor3f(0.0f, Green, 0.0f);
	glVertex3f(-0.30f, -1.0f, 0.0f);


	glColor3f(0.0f, Green, 0.0f);
	glVertex3f(0.255f, -1.0f, 0.0f);
	glEnd();

}

void drawIn(void)
{

	glBegin(GL_LINES);

	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);

	glEnd();
}

void drawA(void)
{


	glBegin(GL_LINES);

	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, -1.0f, 0.0f);

	glColor3f(1.0f, 0.5f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, -1.0f, 0.0f);

	glEnd();


}

void drawTricolor(void)
{
	//if (gflagOfA == true)
	//{
	//glTranslatef()
	//}

	if (gflagOfA == true)
	{
		glTranslatef(xExtremeLeft, 0.0f, 0.0f);

		glBegin(GL_LINES);
		glColor3f(1.0f, 1.0f, 1.0f);
		if (gHeadOfx <= 0.25)
		{
			glVertex3f(gTailOfx, 0.0f, 0.0f);
			glVertex3f(gHeadOfx, 0.0f, 0.0f);
			gHeadOfx = gHeadOfx + 0.0001f;
		}
		else if (gTailOfx <= -0.25f)
		{
			glVertex3f(gTailOfx, 0.0f, 0.0f);
			glVertex3f(gHeadOfx, 0.0f, 0.0f);
			gTailOfx = gTailOfx + 0.0001f;
		}
		else
		{
			glVertex3f(gTailOfx, 0.0f, 0.0f);
			glVertex3f(gHeadOfx, 0.0f, 0.0f);
		}

		glColor3f(1.0f, 0.5f, 0.0f);
		if (gHeadOfx <= 0.245)
		{
			glVertex3f(gTailOfx, 0.015f, 0.0f);
			glVertex3f(gHeadOfx, 0.015f, 0.0f);
			gHeadOfx = gHeadOfx + 0.0001f;
		}
		else if (gTailOfx <= -0.245)
		{
			glVertex3f(gTailOfx, 0.015f, 0.0f);
			glVertex3f(gHeadOfx, 0.015f, 0.0f);
			gTailOfx = gTailOfx + 0.0001f;
		}
		else
		{
			glVertex3f(gTailOfx, 0.015f, 0.0f);
			glVertex3f(gHeadOfx, 0.015f, 0.0f);
		}
		glColor3f(0.0f, 1.0f, 0.0f);
		if (gHeadOfx <= 0.255)
		{
			glVertex3f(gTailOfx, -0.015f, 0.0f);
			glVertex3f(gHeadOfx, -0.015f, 0.0f);
			gHeadOfx = gHeadOfx + 0.0001f;
		}
		else if (gTailOfx <= -0.255)
		{
			glVertex3f(gTailOfx, -0.015f, 0.0f);
			glVertex3f(gHeadOfx, -0.015f, 0.0f);
			gTailOfx = gTailOfx + 0.0001f;
		}
		else
		{
			glVertex3f(gTailOfx, -0.015f, 0.0f);
			glVertex3f(gHeadOfx, -0.015f, 0.0f);
		}
		glEnd();
	}

}

void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
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

