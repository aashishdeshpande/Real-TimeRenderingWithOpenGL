#include <windows.h>
#include <gl/GL.h>
#include <GL/glu.h>
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

GLUquadric * quadric = NULL;

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
bool gbLighting = false;

GLfloat light0_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light0_diffuse[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light0_specular[] = { 1.0f,0.0f,0.0f,1.0f };
GLfloat light0_position[] = { 2.0f,1.0f,1.0f,0.0f };

GLfloat light1_ambient[] = { 0.0f,0.0f,0.0f,1.0f};
GLfloat light1_diffuse[] = { 0.0f,0.0f,1.0f,1.0f};
GLfloat light1_specular[] = { 0.0f,0.0f,1.0f,1.0f};
GLfloat light1_position[] = { -2.0f,1.0f,1.0f,0.0f};

GLfloat material_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat material_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat material_shininess[] = {50.0f};

GLboolean gbPyramid = GL_TRUE;
GLboolean gbCube = GL_FALSE;
GLboolean gbSphere = GL_FALSE;

GLfloat gfanglePyramid = 0.0f;
GLfloat gfangleCube = 0.0f;
GLfloat gfangleSphere = 0.0f;
//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype
	void initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

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
		TEXT("Lighting Assignment: Two Lights And Three Shapes"),
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
				update();
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

		case 0x4C:
			if (gbLighting == false)
			{
				glEnable(GL_LIGHTING);
				gbLighting = true;
			}

			else
			{
				gbLighting = false;
				glDisable(GL_LIGHTING);
			}

			break;

		case 0x50:
			gbPyramid = GL_TRUE;
			gbCube = GL_FALSE;
			gbSphere = GL_FALSE;
			break;

		case 0x53:
			gbPyramid = GL_FALSE;
			gbCube = GL_FALSE;
			gbSphere = GL_TRUE;
			break;

		case 0x43:
			gbPyramid = GL_FALSE;
			gbCube = GL_TRUE;
			gbSphere = GL_FALSE;
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
	pfd.cDepthBits = 32;



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

	quadric = gluNewQuadric();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glEnable(GL_LIGHT1);

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (gbPyramid == GL_TRUE)
	{
		glTranslatef(0.0f, 0.0f, -6.0f);
		glRotatef(gfanglePyramid, 0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLES);

		//FRONT FACE
		glNormal3f(0.0f, 0.447214f, 0.894427f);
		glVertex3f(0.0f, 1.0f, 0.0f);//apex
		glVertex3f(-1.0f, -1.0f, 1.0f);//left corner 
		glVertex3f(1.0f, -1.0f, 1.0f);//right corner

									  //RIGHT FACE
		glNormal3f(0.894427f, 0.447214f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);//apex
		glVertex3f(1.0f, -1.0f, 1.0f);//left corner of right face
		glVertex3f(1.0f, -1.0f, -1.0f);//right corner of right face

									   //BACK FACE
		glNormal3f(0.0f, 0.447214f, -0.894427f);
		glVertex3f(0.0f, 1.0f, 0.0f);//apex
		glVertex3f(1.0f, -1.0f, -1.0f);//left corner of back face
		glVertex3f(-1.0f, -1.0f, -1.0f);//right coner of back face

										//LEFT FACE
		glNormal3f(-0.894427f, 0.447214f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);//apex
		glVertex3f(-1.0f, -1.0f, -1.0f);//left corner of left face 
		glVertex3f(-1.0f, -1.0f, 1.0f);//right corner of left face

		glEnd();

	}

	else if (gbCube == GL_TRUE)
	{
		glTranslatef(0.0f, 0.0f, -6.0f);
		glScalef(0.7f, 0.75f, 0.75f);
		glRotatef(gfangleCube, 0.0f, 1.0f, 0.0f);
		glBegin(GL_QUADS);

		//TOP FACE
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);

		//BOTTOM FACE
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);//right top of bottom face
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);

		//FRONT FACE
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 1.0f);//right top
		glVertex3f(-1.0f, 1.0f, 1.0f);//left top
		glVertex3f(-1.0f, -1.0f, 1.0f);//left bottom
		glVertex3f(1.0f, -1.0f, 1.0f);

		//BACK FACE
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);//right top of back face
		glVertex3f(-1.0f, 1.0f, -1.0f);//left top of back face
		glVertex3f(-1.0f, -1.0f, -1.0f);//left bottom of back face
		glVertex3f(1.0f, -1.0f, -1.0f);//right bottom of back face

									   //RIGHT FACE
		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);//right top of right face
		glVertex3f(1.0f, 1.0f, 1.0f);//left top of right face
		glVertex3f(1.0f, -1.0f, 1.0f);//left bottom of right face
		glVertex3f(1.0f, -1.0f, -1.0f);//right bottom of right facce


									   //LEFT FACE
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);//right top of left face
		glVertex3f(-1.0f, 1.0f, -1.0f);//left top of left face
		glVertex3f(-1.0f, -1.0f, -1.0f);//left bottom of left face
		glVertex3f(-1.0f, -1.0f, 1.0f);//right bottom of left face


		glEnd();

	}

	else if (gbSphere == GL_TRUE)
	{
		glTranslatef(0.0f, 0.0f, -6.0f);
		glRotatef(gfangleSphere, 0.0f, 1.0f, 0.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluSphere(quadric, 0.50, 100, 100);
	}

	else
	{

	}

	SwapBuffers(ghdc);
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

	if (quadric)
	{
		gluDeleteQuadric(quadric);
	}
}

void update(void)
{
	gfanglePyramid = gfanglePyramid + 0.05f;
	if (gfanglePyramid >= 360.0f)
	{
		gfanglePyramid = 0.0f;
	}

	gfangleCube = gfangleCube + 0.05f;

	if (gfangleCube >= 360.0f)
	{
		gfangleCube = 0.0f;
	}

	gfangleSphere = gfangleSphere + 0.05f;

	if (gfangleSphere >= 360.0f)
	{
		gfangleSphere = 0.0f;
	}

}
