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

GLfloat light0_ambient[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light0_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light0_specular[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light0_position[] = { 0.0f,0.0f,0.0f,0.0f };

//Emrald
GLfloat material1_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
GLfloat material1_diffuse[] = { 0.07568f, 0.061424f, 0.07568f, 1.0f };
GLfloat material1_specular[] = { 0.633f, 0.727811f, 0.633f, 1.0f };
GLfloat material1_shininess[] = { 76.8f };

//Jade
GLfloat material2_ambient[] = { 0.135f, 0.2225f ,0.1575f, 1.0f };
GLfloat material2_diffuse[] = { 0.54f, 0.89f, 0.63f, 1.0f };
GLfloat material2_specular[] = { 0.316288f, 0.316288f, 0.316288f, 1.0f };
GLfloat material2_shininess[] = { 12.8f };

//Obsidian
GLfloat material3_ambient[] = { 0.05375f, 0.05f, 0.06625f, 1.0f };
GLfloat material3_diffuse[] = { 0.18275f, 0.17f, 0.22525f, 1.0f };
GLfloat material3_specular[] = { 0.332741f, 0.328634f, 0.346435f, 1.0f };
GLfloat material13_shininess[] = { 38.4f };

//Pearl
GLfloat material4_ambient[] = { 0.25f, 0.20725f, 0.20725f, 1.0f };
GLfloat material4_diffuse[] = { 1.0f, 0.829f, 0.829f, 1.0f };
GLfloat material4_specular[] = { 0.296648f, 0.296648f, 0.296648f, 1.0f };
GLfloat material4_shininess[] = { 11.264f };

//Ruby
GLfloat material5_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
GLfloat material5_diffuse[] = { 0.61424f, 0.04136f, 0.04136f, 1.0f };
GLfloat material5_specular[] = { 0.727811f, 0.626959f, 0.626959f, 1.0f };
GLfloat material5_shininess[] = { 76.8f };

//Turquoise
GLfloat material6_ambient[] = { 0.1f, 0.18725f, 0.1745f, 1.0f };
GLfloat material6_diffuse[] = { 0.396f, 0.74151f, 0.69102f, 1.0f };
GLfloat material6_specular[] = { 0.297254f, 0.30829f, 0.306678f, 1.0f };
GLfloat material6_shininess[] = { 12.8f };

//Brass
GLfloat material7_ambient[] = { 0.329412f, 0.223529f, 0.027451f, 1.0f };
GLfloat material7_diffuse[] = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
GLfloat material7_specular[] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
GLfloat material7_shininess[] = { 27.897443616f };

//Bronze
GLfloat material8_ambient[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
GLfloat material8_diffuse[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
GLfloat material8_specular[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
GLfloat material8_shininess[] = { 25.6f };

//Chrome
GLfloat material9_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat material9_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat material9_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat material9_shininess[] = { 76.8f };

//Copper
GLfloat material10_ambient[] = { 0.19125f, 0.0735f, 0.0225f, 1.0f };
GLfloat material10_diffuse[] = { 0.7038f, 0.27048f, 0.0828f, 1.0f };
GLfloat material10_specular[] = { 0.256777f, 0.137622f, 0.086014f, 1.0f };
GLfloat material10_shininess[] = { 12.8f };

//Gold
GLfloat material11_ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
GLfloat material11_diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
GLfloat material11_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
GLfloat material11_shininess[] = { 51.2f };

//Silver
GLfloat material12_ambient[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
GLfloat material12_diffuse[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
GLfloat material12_specular[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
GLfloat material12_shininess[] = { 51.2f };

//Black
GLfloat material13_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material13_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
GLfloat material13_specular[] = { 0.50f, 0.50f, 0.50f, 1.0f };
GLfloat materiall3_shininess[] = { 32.0f };

//Cyan
GLfloat material14_ambient[] = { 0.0f, 0.1f, 0.06f, 1.0f };
GLfloat material14_diffuse[] = { 0.0f, 0.50980392f, 0.50980392f, 1.0f };
GLfloat material14_specular[] = { 0.50196078f, 0.50196078f, 0.50196078f, 1.0f };
GLfloat material14_shininess[] = { 32.0f };

//Green
GLfloat material15_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material15_diffuse[] = { 0.1f, 0.35f, 0.1f, 1.0f };
GLfloat material15_specular[] = { 0.45f, 0.55f, 0.45f, 1.0f };
GLfloat material15_shininess[] = { 32.0f };

//Red
GLfloat material16_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material16_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
GLfloat material16_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat material16_shininess[] = { 32.0f };

//White
GLfloat material17_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material17_diffuse[] = { 0.55f, 0.55f, 0.55f, 1.0f };
GLfloat material17_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
GLfloat material17_shininess[] = { 32.0f };

//Yellow Plastic
GLfloat material18_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material18_diffuse[] = { 0.5f, 0.5f, 0.0f, 1.0f };
GLfloat material18_specular[] = { 0.60f, 0.60f, 0.50f, 1.0f };
GLfloat material18_shininess[] = { 32.0f };

//Black
GLfloat material19_ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
GLfloat material19_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
GLfloat material19_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat material19_shininess[] = { 10.0f };

//Cyan
GLfloat material20_ambient[] = { 0.0f, 0.05f, 0.05f, 1.0f };
GLfloat material20_diffuse[] = { 0.4f, 0.5f, 0.5f, 1.0f };
GLfloat material20_specular[] = { 0.04f, 0.7f, 0.7f, 1.0f };
GLfloat material20_shininess[] = { 10.0f };

//Green
GLfloat material21_ambient[] = { 0.0f, 0.05f, 0.0f, 1.0f };
GLfloat material21_diffuse[] = { 0.4f, 0.5f, 0.4f, 1.0f };
GLfloat material21_specular[] = { 0.04f, 0.7f, 0.04f, 1.0f };
GLfloat material21_shininess[] = { 10.0f };

//Red
GLfloat material22_ambient[] = { 0.05f, 0.0f, 0.0f, 1.0f };
GLfloat material22_diffuse[] = { 0.5f, 0.4f, 0.4f, 1.0f };
GLfloat material22_specular[] = { 0.7f, 0.04f, 0.04f, 1.0f };
GLfloat material22_shininess[] = { 10.0f };

//White
GLfloat material23_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat material23_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat material23_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat material23_shininess[] = { 10.0f };

//Yellow Rubber
GLfloat material24_ambient[] = { 0.05f, 0.05f, 0.0f, 1.0f };
GLfloat material24_diffuse[] = { 0.5f, 0.5f, 0.4f, 1.0f };
GLfloat material24_specular[] = { 0.7f, 0.7f, 0.04f, 1.0f };
GLfloat material24_shininess[] = { 10.0f };

GLfloat light_model_ambient[] = { 0.2, 0.2, 0.2, 0.2 };
GLfloat light_model_local_viewer[] = { 0.0f, 0.0f, 0.0f };

GLfloat gfangleX_axis = 0.0f;
GLfloat gfangleY_axis = 0.0f;
GLfloat gfangleZ_axis = 0.0f;

GLboolean gbX_axis = GL_FALSE;
GLboolean gbY_axis = GL_FALSE;
GLboolean gbZ_axis = GL_FALSE;
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
	wndclass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0.5,0.5,0.5));
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

		case 0x58:
			light0_position[0] = 0.0f;
			light0_position[1] = 0.0f;
			light0_position[2] = 0.0f;
			light0_position[3] = 0.0f;

			gbX_axis = GL_TRUE;
			gbY_axis = GL_FALSE;
			gbZ_axis = GL_FALSE;
			
			break;

		case 0x59:
			light0_position[0] = 0.0f;
			light0_position[1] = 0.0f;
			light0_position[2] = 0.0f;
			light0_position[3] = 0.0f;

			gbY_axis = GL_TRUE;
			gbX_axis = GL_FALSE;
			gbZ_axis = GL_FALSE;
			break;

		case 0x5A:
			light0_position[0] = 0.0f;
			light0_position[1] = 0.0f;
			light0_position[2] = 0.0f;
			light0_position[3] = 0.0f;

			gbZ_axis = GL_TRUE;
			gbY_axis = GL_FALSE;
			gbX_axis = GL_FALSE;
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

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	//glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glEnable(GL_LIGHT0);

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_model_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, light_model_local_viewer);

	


	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//code
	void drawSphere(void);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glPushMatrix();
	//glRotatef(gfangleSphere, 0.0f, 1.0f, 0.0f);
    
	if (gbX_axis == GL_TRUE)
	{
		glPushMatrix();
		glRotatef(gfangleX_axis, 1.0f, 0.0f, 0.0f);
		light0_position[1] = gfangleX_axis;
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glPopMatrix();
		drawSphere();
	}

	else if (gbY_axis == GL_TRUE)
	{
		glPushMatrix();
		glRotatef(gfangleY_axis, 0.0f, 1.0f, 0.0f);
		light0_position[0] = gfangleY_axis;
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glPopMatrix();
		drawSphere();
	}

	else if (gbZ_axis == GL_TRUE)
	{
		glPushMatrix();
		glRotatef(gfangleZ_axis, 0.0f, 0.0f, 1.0f);
		light0_position[0] = gfangleZ_axis;
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glPopMatrix();
		drawSphere();
	}

	else
	{
	//	glPushMatrix();
		drawSphere();
	//	glPopMatrix();
	}

	//glPopMatrix();
	SwapBuffers(ghdc);
}


void drawSphere()
{
	glPushMatrix();
	glTranslatef(-2.0f, 2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material1_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material1_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material1_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material1_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f, 2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material2_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material2_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material2_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material2_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material3_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material3_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material3_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material13_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, 2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material4_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material4_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material4_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material4_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, 1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material5_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material5_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material5_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material5_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f, 1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material6_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material6_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material6_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material6_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material7_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material7_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material7_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material7_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, 1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material8_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material8_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material8_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material8_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, 0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material9_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material9_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material9_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material9_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f, 0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material10_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material10_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material10_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material10_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material11_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material11_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material11_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material11_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, 0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material12_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material12_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material12_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material12_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, -0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material13_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material13_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material13_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material13_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f, -0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material14_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material14_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material14_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material14_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material15_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material15_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material15_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material15_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, -0.4f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material16_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material16_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material16_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material16_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, -1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material17_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material17_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material17_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material17_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f, -1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material18_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material18_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material18_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material18_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material19_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material19_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material19_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material19_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, -1.2f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material20_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material20_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material20_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material20_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, -2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material21_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material21_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material21_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material21_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f, -2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material22_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material22_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material22_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material22_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, -2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material23_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material23_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material23_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material23_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.0f, -2.0f, -6.0f);
	glMaterialfv(GL_FRONT, GL_AMBIENT, material24_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material24_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material24_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, material24_shininess);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluSphere(quadric, 0.30, 30, 30);
	glPopMatrix();
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
	if (gbX_axis == 1)
	{
		gfangleX_axis = gfangleX_axis + 0.3f;

		if (gfangleX_axis >= 360.0f)
		{
			gfangleX_axis = 0.0f;
		}
	}

	if (gbY_axis == 1)
	{

		gfangleY_axis = gfangleY_axis + 0.3f;

		if (gfangleY_axis >= 360.0f)
		{
			gfangleY_axis = 0.0f;
		}
	}

	if (gbZ_axis == 1)
	{
		gfangleZ_axis = gfangleZ_axis + 0.3f;

		if (gfangleZ_axis >= 360.0f)
		{
			gfangleZ_axis = 0.0f;
		}
	}
}