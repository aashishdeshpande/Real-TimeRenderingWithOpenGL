#include <windows.h>
#include <stdio.h> // for FILE I/O

#include <gl\glew.h> // for GLSL extensions IMPORTANT : This Line Should Be Before #include<gl\gl.h> And #include<gl\glu.h>

#include <gl/GL.h>

#include "vmath.h"

#include "Sphere.h"

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"Sphere.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

using namespace vmath;

enum
{
	ARD_ATTRIBUTE_POSITION = 0,
	ARD_ATTRIBUTE_COLOR,
	ARD_ATTRIBUTE_NORMAL,
	ARD_ATTRIBUTE_TEXTURE0,
};

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];

//Prototype Of WndProc() declared Globally
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Global variable declarations
FILE *gpFile = NULL;

HWND ghwnd = NULL;
HDC ghdc = NULL;
HGLRC ghrc = NULL;

DWORD dwStyle;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

bool gbActiveWindow = false;
bool gbEscapeKeyIsPressed = false;
bool gbFullscreen = false;

GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;

GLuint gVao_sphere;

GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint gNumVertices;
GLuint gNumElements;

GLuint gModelMatrixUniform, gViewMatrixUniform, gProjectionMatrixUniform;

GLuint gLdUniform;
GLuint gLaUniform;
GLuint gLsUniform;

GLuint gKa1Uniform;
GLuint gKd1Uniform;
GLuint gKs1Uniform;
GLuint gmaterial1_shininess_uniform;

GLuint gKa2Uniform;
GLuint gKd2Uniform;
GLuint gKs2Uniform;
GLuint gmaterial2_shininess_uniform;

GLuint gKa3Uniform;
GLuint gKd3Uniform;
GLuint gKs3Uniform;
GLuint gmaterial3_shininess_uniform;

GLuint gKa4Uniform;
GLuint gKd4Uniform;
GLuint gKs4Uniform;
GLuint gmaterial4_shininess_uniform;

GLuint gKa5Uniform;
GLuint gKd5Uniform;
GLuint gKs5Uniform;
GLuint gmaterial5_shininess_uniform;

GLuint gKa6Uniform;
GLuint gKd6Uniform;
GLuint gKs6Uniform;
GLuint gmaterial6_shininess_uniform;

GLuint gKa7Uniform;
GLuint gKd7Uniform;
GLuint gKs7Uniform;
GLuint gmaterial7_shininess_uniform;

GLuint gKa8Uniform;
GLuint gKd8Uniform;
GLuint gKs8Uniform;
GLuint gmaterial8_shininess_uniform;

GLuint gKa9Uniform;
GLuint gKd9Uniform;
GLuint gKs9Uniform;
GLuint gmaterial9_shininess_uniform;

GLuint gKa10Uniform;
GLuint gKd10Uniform;
GLuint gKs10Uniform;
GLuint gmaterial10_shininess_uniform;

GLuint gKa11Uniform;
GLuint gKd11Uniform;
GLuint gKs11Uniform;
GLuint gmaterial11_shininess_uniform;

GLuint gKa12Uniform;
GLuint gKd12Uniform;
GLuint gKs12Uniform;
GLuint gmaterial12_shininess_uniform;

GLuint gKa13Uniform;
GLuint gKd13Uniform;
GLuint gKs13Uniform;
GLuint gmaterial13_shininess_uniform;

GLuint gKa14Uniform;
GLuint gKd14Uniform;
GLuint gKs14Uniform;
GLuint gmaterial14_shininess_uniform;

GLuint gKa15Uniform;
GLuint gKd15Uniform;
GLuint gKs15Uniform;
GLuint gmaterial15_shininess_uniform;

GLuint gKa16Uniform;
GLuint gKd16Uniform;
GLuint gKs16Uniform;
GLuint gmaterial16_shininess_uniform;

GLuint gKa17Uniform;
GLuint gKd17Uniform;
GLuint gKs17Uniform;
GLuint gmaterial17_shininess_uniform;

GLuint gKa18Uniform;
GLuint gKd18Uniform;
GLuint gKs18Uniform;
GLuint gmaterial18_shininess_uniform;

GLuint gKa19Uniform;
GLuint gKd19Uniform;
GLuint gKs19Uniform;
GLuint gmaterial19_shininess_uniform;

GLuint gKa20Uniform;
GLuint gKd20Uniform;
GLuint gKs20Uniform;
GLuint gmaterial20_shininess_uniform;

GLuint gKa21Uniform;
GLuint gKd21Uniform;
GLuint gKs21Uniform;
GLuint gmaterial21_shininess_uniform;

GLuint gKa22Uniform;
GLuint gKd22Uniform;
GLuint gKs22Uniform;
GLuint gmaterial22_shininess_uniform;

GLuint gKa23Uniform;
GLuint gKd23Uniform;
GLuint gKs23Uniform;
GLuint gmaterial23_shininess_uniform;

GLuint gKa24Uniform;
GLuint gKd24Uniform;
GLuint gKs24Uniform;
GLuint gmaterial24_shininess_uniform;


GLuint glight_position_uniform;

GLuint gLKeyPressedUniform;
GLuint gSphereNo;

mat4 gPerspectiveProjectionMatrix;

bool gbLight;
bool gbXaxis = GL_FALSE;
bool gbYaxis = GL_FALSE;
bool gbZaxis = GL_FALSE;

GLfloat gfangleX_axis = 0.0f;
GLfloat gfangleY_axis = 0.0f;
GLfloat gfangleZ_axis = 0.0f;

GLfloat lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[4];

//Emrald
GLfloat material1_ambient[] = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
GLfloat material1_diffuse[] = { 0.07568f, 0.061424f, 0.07568f, 1.0f };
GLfloat material1_specular[] = { 0.633f, 0.727811f, 0.633f, 1.0f };
GLfloat material1_shininess = 76.8f;

//Jade
GLfloat material2_ambient[] = { 0.135f, 0.2225f ,0.1575f, 1.0f };
GLfloat material2_diffuse[] = { 0.54f, 0.89f, 0.63f, 1.0f };
GLfloat material2_specular[] = { 0.316288f, 0.316288f, 0.316288f, 1.0f };
GLfloat material2_shininess = 12.8f;

//Obsidian
GLfloat material3_ambient[] = { 0.05375f, 0.05f, 0.06625f, 1.0f };
GLfloat material3_diffuse[] = { 0.18275f, 0.17f, 0.22525f, 1.0f };
GLfloat material3_specular[] = { 0.332741f, 0.328634f, 0.346435f, 1.0f };
GLfloat material3_shininess =  38.4f;

//Pearl
GLfloat material4_ambient[] = { 0.25f, 0.20725f, 0.20725f, 1.0f };
GLfloat material4_diffuse[] = { 1.0f, 0.829f, 0.829f, 1.0f };
GLfloat material4_specular[] = { 0.296648f, 0.296648f, 0.296648f, 1.0f };
GLfloat material4_shininess = 11.264f;

//Ruby
GLfloat material5_ambient[] = { 0.1745f, 0.01175f, 0.01175f, 1.0f };
GLfloat material5_diffuse[] = { 0.61424f, 0.04136f, 0.04136f, 1.0f };
GLfloat material5_specular[] = { 0.727811f, 0.626959f, 0.626959f, 1.0f };
GLfloat material5_shininess =  76.8f;

//Turquoise
GLfloat material6_ambient[] = { 0.1f, 0.18725f, 0.1745f, 1.0f };
GLfloat material6_diffuse[] = { 0.396f, 0.74151f, 0.69102f, 1.0f };
GLfloat material6_specular[] = { 0.297254f, 0.30829f, 0.306678f, 1.0f };
GLfloat material6_shininess =  12.8f;

//Brass
GLfloat material7_ambient[] = { 0.329412f, 0.223529f, 0.027451f, 1.0f };
GLfloat material7_diffuse[] = { 0.780392f, 0.568627f, 0.113725f, 1.0f };
GLfloat material7_specular[] = { 0.992157f, 0.941176f, 0.807843f, 1.0f };
GLfloat material7_shininess =  27.897443616f;

//Bronze
GLfloat material8_ambient[] = { 0.2125f, 0.1275f, 0.054f, 1.0f };
GLfloat material8_diffuse[] = { 0.714f, 0.4284f, 0.18144f, 1.0f };
GLfloat material8_specular[] = { 0.393548f, 0.271906f, 0.166721f, 1.0f };
GLfloat material8_shininess =  25.6f;

//Chrome
GLfloat material9_ambient[] = { 0.25f, 0.25f, 0.25f, 1.0f };
GLfloat material9_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat material9_specular[] = { 0.774597f, 0.774597f, 0.774597f, 1.0f };
GLfloat material9_shininess = 76.8f;

//Copper
GLfloat material10_ambient[] = { 0.19125f, 0.0735f, 0.0225f, 1.0f };
GLfloat material10_diffuse[] = { 0.7038f, 0.27048f, 0.0828f, 1.0f };
GLfloat material10_specular[] = { 0.256777f, 0.137622f, 0.086014f, 1.0f };
GLfloat material10_shininess = 12.8f;

//Gold
GLfloat material11_ambient[] = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
GLfloat material11_diffuse[] = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
GLfloat material11_specular[] = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
GLfloat material11_shininess = 51.2f;

//Silver
GLfloat material12_ambient[] = { 0.19225f, 0.19225f, 0.19225f, 1.0f };
GLfloat material12_diffuse[] = { 0.50754f, 0.50754f, 0.50754f, 1.0f };
GLfloat material12_specular[] = { 0.508273f, 0.508273f, 0.508273f, 1.0f };
GLfloat material12_shininess = 51.2f;

//Black
GLfloat material13_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material13_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
GLfloat material13_specular[] = { 0.50f, 0.50f, 0.50f, 1.0f };
GLfloat material13_shininess = 32.0f;

//Cyan
GLfloat material14_ambient[] = { 0.0f, 0.1f, 0.06f, 1.0f };
GLfloat material14_diffuse[] = { 0.0f, 0.50980392f, 0.50980392f, 1.0f };
GLfloat material14_specular[] = { 0.50196078f, 0.50196078f, 0.50196078f, 1.0f };
GLfloat material14_shininess =  32.0f;

//Green
GLfloat material15_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material15_diffuse[] = { 0.1f, 0.35f, 0.1f, 1.0f };
GLfloat material15_specular[] = { 0.45f, 0.55f, 0.45f, 1.0f };
GLfloat material15_shininess = 32.0f;

//Red
GLfloat material16_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material16_diffuse[] = { 0.5f, 0.0f, 0.0f, 1.0f };
GLfloat material16_specular[] = { 0.7f, 0.6f, 0.6f, 1.0f };
GLfloat material16_shininess = 32.0f;

//White
GLfloat material17_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material17_diffuse[] = { 0.55f, 0.55f, 0.55f, 1.0f };
GLfloat material17_specular[] = { 0.70f, 0.70f, 0.70f, 1.0f };
GLfloat material17_shininess = 32.0f;

//Yellow Plastic
GLfloat material18_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material18_diffuse[] = { 0.5f, 0.5f, 0.0f, 1.0f };
GLfloat material18_specular[] = { 0.60f, 0.60f, 0.50f, 1.0f };
GLfloat material18_shininess = 32.0f;

//Black
GLfloat material19_ambient[] = { 0.02f, 0.02f, 0.02f, 1.0f };
GLfloat material19_diffuse[] = { 0.01f, 0.01f, 0.01f, 1.0f };
GLfloat material19_specular[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat material19_shininess = 10.0f;

//Cyan
GLfloat material20_ambient[] = { 0.0f, 0.05f, 0.05f, 1.0f };
GLfloat material20_diffuse[] = { 0.4f, 0.5f, 0.5f, 1.0f };
GLfloat material20_specular[] = { 0.04f, 0.7f, 0.7f, 1.0f };
GLfloat material20_shininess = 10.0f;

//Green
GLfloat material21_ambient[] = { 0.0f, 0.05f, 0.0f, 1.0f };
GLfloat material21_diffuse[] = { 0.4f, 0.5f, 0.4f, 1.0f };
GLfloat material21_specular[] = { 0.04f, 0.7f, 0.04f, 1.0f };
GLfloat material21_shininess = 10.0f;

//Red
GLfloat material22_ambient[] = { 0.05f, 0.0f, 0.0f, 1.0f };
GLfloat material22_diffuse[] = { 0.5f, 0.4f, 0.4f, 1.0f };
GLfloat material22_specular[] = { 0.7f, 0.04f, 0.04f, 1.0f };
GLfloat material22_shininess = 10.0f;

//White
GLfloat material23_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
GLfloat material23_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat material23_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat material23_shininess = 10.0f;

//Yellow Rubber
GLfloat material24_ambient[] = { 0.05f, 0.05f, 0.0f, 1.0f };
GLfloat material24_diffuse[] = { 0.5f, 0.5f, 0.4f, 1.0f };
GLfloat material24_specular[] = { 0.7f, 0.7f, 0.04f, 1.0f };
GLfloat material24_shininess = 10.0f;

//main()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype
	void initialize(void);
	void uninitialize(void);
	void display(void);
	void spin(void);

	//variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("OpenGLPP");
	bool bDone = false;

	//code
	// create log file
	if (fopen_s(&gpFile, "Log.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Can Not Be Created\nExitting ..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Log File Is Successfully Opened.\n");
	}

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
	//Parallel to glutInitWindowSize(), glutInitWindowPosition() and glutCreateWindow() all three together
	hwnd = CreateWindow(szClassName,
		TEXT("OpenGL Programmable Pipeline Window : Phong Lighting(PerFragment)"),
		WS_OVERLAPPEDWINDOW,
		(x / 2) - (WIN_WIDTH / 2),
		(y / 2) - (WIN_HEIGHT / 2),
		WIN_WIDTH,
		WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	//initialize
	initialize();

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
			// rendring function
			display();
			spin();
			if (gbActiveWindow == true)
			{
				if (gbEscapeKeyIsPressed == true) //Continuation to glutLeaveMainLoop();
					bDone = true;
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

	//variable declarations
	//static bool bIsAKeyPressed = false;
	static bool bIsLKeyPressed = false;

	//code
	switch (iMsg)
	{
	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0) //if 0, the window is active
			gbActiveWindow = true;
		else //if non-zero, the window is not active
			gbActiveWindow = false;
		break;
	case WM_ERASEBKGND:
		return(0);
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: //case 27
			if (gbEscapeKeyIsPressed == false)
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
			if (bIsLKeyPressed == false)
			{
				gbLight = true;
				bIsLKeyPressed = true;
			}
			else
			{
				gbLight = false;
				bIsLKeyPressed = false;
			}
			break;


		case 0x58:
			//lightPosition[0] = 0.0f;
			//lightPosition[1] = 0.0f;
			//lightPosition[2] = 0.0f;
			//lightPosition[3] = 0.0f;

			gbXaxis = GL_TRUE;
			gbYaxis = GL_FALSE;
			gbZaxis = GL_FALSE;

			break;

		case 0x59:
			//lightPosition[0] = 0.0f;
			//lightPosition[1] = 0.0f;
			//lightPosition[2] = 0.0f;
			//lightPosition[3] = 0.0f;

			gbYaxis = GL_TRUE;
			gbXaxis = GL_FALSE;
			gbZaxis = GL_FALSE;
			break;

		case 0x5A:
			//lightPosition[0] = 0.0f;
			//lightPosition[1] = 0.0f;
			//lightPosition[2] = 0.0f;
			//lightPosition[3] = 0.0f;

			gbZaxis = GL_TRUE;
			gbYaxis = GL_FALSE;
			gbXaxis = GL_FALSE;
			break;

		default:
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_CLOSE:
		uninitialize();
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

//FUNCTION DEFINITIONS
void initialize(void)
{
	//MessageBox(NULL, TEXT("Error"), TEXT("In intit"), MB_OK);
	fprintf(gpFile, "Renderer : %s\n", glGetString(GL_RENDERER));
	//function prototypes
	void uninitialize(void);
	void resize(int, int);

	//variable declarations
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;

	//code
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	//Initialization of structure 'PIXELFORMATDESCRIPTOR'
	//Parallel to glutInitDisplayMode()
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

	//choose a pixel format which best matches with that of 'pfd'
	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//set the pixel format chosen above
	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == false)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//create OpenGL rendering context
	ghrc = wglCreateContext(ghdc);
	if (ghrc == NULL)
	{
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	//make the rendering context created above as current n the current hdc
	if (wglMakeCurrent(ghdc, ghrc) == false)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// GLEW Initialization Code For GLSL ( IMPORTANT : It Must Be Here. Means After Creating OpenGL Context But Before Using Any OpenGL Function )
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		wglDeleteContext(ghrc);
		ghrc = NULL;
		ReleaseDC(ghwnd, ghdc);
		ghdc = NULL;
	}

	// *** VERTEX SHADER ***
	// create shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light_position;" \
		"uniform int u_lighting_enabled;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction;" \
		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
		"transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"light_direction = normalize(vec3(u_light_position) - eyeCoordinates.xyz);" \
		"viewer_vector = normalize(-eyeCoordinates.xyz);"
		"}" \
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(gVertexShaderObject, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gVertexShaderObject);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 430 core" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction;" \
		"in vec3 viewer_vector;" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ka2;" \
		"uniform vec3 u_Ka3;" \
		"uniform vec3 u_Ka4;" \
		"uniform vec3 u_Ka5;" \
		"uniform vec3 u_Ka6;" \
		"uniform vec3 u_Ka7;" \
		"uniform vec3 u_Ka8;" \
		"uniform vec3 u_Ka9;" \
		"uniform vec3 u_Ka10;" \
		"uniform vec3 u_Ka11;" \
		"uniform vec3 u_Ka12;" \
		"uniform vec3 u_Ka13;" \
		"uniform vec3 u_Ka14;" \
		"uniform vec3 u_Ka15;" \
		"uniform vec3 u_Ka16;" \
		"uniform vec3 u_Ka17;" \
		"uniform vec3 u_Ka18;" \
		"uniform vec3 u_Ka19;" \
		"uniform vec3 u_Ka20;" \
		"uniform vec3 u_Ka21;" \
		"uniform vec3 u_Ka22;" \
		"uniform vec3 u_Ka23;" \
		"uniform vec3 u_Ka24;" \
		"uniform vec3 u_Ld;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Kd2;" \
		"uniform vec3 u_Kd3;" \
		"uniform vec3 u_Kd4;" \
		"uniform vec3 u_Kd5;" \
		"uniform vec3 u_Kd6;" \
		"uniform vec3 u_Kd7;" \
		"uniform vec3 u_Kd8;" \
		"uniform vec3 u_Kd9;" \
		"uniform vec3 u_Kd10;" \
		"uniform vec3 u_Kd11;" \
		"uniform vec3 u_Kd12;" \
		"uniform vec3 u_Kd13;" \
		"uniform vec3 u_Kd14;" \
		"uniform vec3 u_Kd15;" \
		"uniform vec3 u_Kd16;" \
		"uniform vec3 u_Kd17;" \
		"uniform vec3 u_Kd18;" \
		"uniform vec3 u_Kd19;" \
		"uniform vec3 u_Kd20;" \
		"uniform vec3 u_Kd21;" \
		"uniform vec3 u_Kd22;" \
		"uniform vec3 u_Kd23;" \
		"uniform vec3 u_Kd24;" \
		"uniform vec3 u_Ls;" \
		"uniform vec3 u_Ks1;" \
		"uniform vec3 u_Ks2;" \
		"uniform vec3 u_Ks3;" \
		"uniform vec3 u_Ks4;" \
		"uniform vec3 u_Ks5;" \
		"uniform vec3 u_Ks6;" \
		"uniform vec3 u_Ks7;" \
		"uniform vec3 u_Ks8;" \
		"uniform vec3 u_Ks9;" \
		"uniform vec3 u_Ks10;" \
		"uniform vec3 u_Ks11;" \
		"uniform vec3 u_Ks12;" \
		"uniform vec3 u_Ks13;" \
		"uniform vec3 u_Ks14;" \
		"uniform vec3 u_Ks15;" \
		"uniform vec3 u_Ks16;" \
		"uniform vec3 u_Ks17;" \
		"uniform vec3 u_Ks18;" \
		"uniform vec3 u_Ks19;" \
		"uniform vec3 u_Ks20;" \
		"uniform vec3 u_Ks21;" \
		"uniform vec3 u_Ks22;" \
		"uniform vec3 u_Ks23;" \
		"uniform vec3 u_Ks24;" \
		"uniform float u_material1_shininess;" \
		"uniform float u_material2_shininess;" \
		"uniform float u_material3_shininess;" \
		"uniform float u_material4_shininess;" \
		"uniform float u_material5_shininess;" \
		"uniform float u_material6_shininess;" \
		"uniform float u_material7_shininess;" \
		"uniform float u_material8_shininess;" \
		"uniform float u_material9_shininess;" \
		"uniform float u_material10_shininess;" \
		"uniform float u_material11_shininess;" \
		"uniform float u_material12_shininess;" \
		"uniform float u_material13_shininess;" \
		"uniform float u_material14_shininess;" \
		"uniform float u_material15_shininess;" \
		"uniform float u_material16_shininess;" \
		"uniform float u_material17_shininess;" \
		"uniform float u_material18_shininess;" \
		"uniform float u_material19_shininess;" \
		"uniform float u_material20_shininess;" \
		"uniform float u_material21_shininess;" \
		"uniform float u_material22_shininess;" \
		"uniform float u_material23_shininess;" \
		"uniform float u_material24_shininess;" \
		"uniform int u_lighting_enabled;" \
		"uniform int sphere;"\
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color;" \
		"if(u_lighting_enabled == 1)" \
		"{" \
		"if(sphere == 1)"\
		"{"
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction = normalize(light_direction);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka1;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = u_Ld * u_Kd1 * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks1 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material1_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"

		"else if(sphere == 2 )" \
		"{"\
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction = normalize(light_direction);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka2;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = u_Ld * u_Kd2 * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks2 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material2_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"\

		"else if(sphere == 3 )" \
		"{"\
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction = normalize(light_direction);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka3;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = u_Ld * u_Kd3 * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks3 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material3_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"\

		"else if(sphere == 4)" \
		"{"\
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction = normalize(light_direction);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka4;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = u_Ld * u_Kd4 * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks4 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material4_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"\

		"else if(sphere == 5)" \
		"{"\
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction = normalize(light_direction);" \
		"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
		"vec3 ambient = u_La * u_Ka5;" \
		"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
		"vec3 diffuse = u_Ld * u_Kd5 * tn_dot_ld;" \
		"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
		"vec3 specular = u_Ls * u_Ks5 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material5_shininess);" \
		"phong_ads_color = ambient + diffuse + specular;" \
		"}"\

			"else if(sphere == 6)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka6;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd6 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks6 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material6_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 7)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka7;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd7 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks7 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material7_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 8)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka8;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd8 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks8 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material8_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 9)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka9;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd9 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks9 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material9_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 10)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka10;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd10 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks10 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material10_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 11)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka11;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd11 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks11 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material11_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 12)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka12;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd12 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks12 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material12_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"\

			"else if(sphere == 13)" \
			"{"\
			"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
			"vec3 normalized_light_direction = normalize(light_direction);" \
			"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
			"vec3 ambient = u_La * u_Ka13;" \
			"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
			"vec3 diffuse = u_Ld * u_Kd13 * tn_dot_ld;" \
			"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
			"vec3 specular = u_Ls * u_Ks13 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material13_shininess);" \
			"phong_ads_color = ambient + diffuse + specular;" \
			"}"

				"else if(sphere == 14)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka14;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd14 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks14 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material14_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 15)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka15;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd15 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks15 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material15_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 16)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka16;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd16 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks16 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material16_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 17)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka17;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd17 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks17 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material17_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 18)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka18;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd18 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks18 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material18_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 19)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka19;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd19 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks19 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material19_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 20)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka20;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd20 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks20 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material20_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

				"else if(sphere == 21)" \
				"{"\
				"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
				"vec3 normalized_light_direction = normalize(light_direction);" \
				"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
				"vec3 ambient = u_La * u_Ka21;" \
				"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
				"vec3 diffuse = u_Ld * u_Kd21 * tn_dot_ld;" \
				"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
				"vec3 specular = u_Ls * u_Ks21 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material21_shininess);" \
				"phong_ads_color = ambient + diffuse + specular;" \
				"}"

					"else if(sphere == 22)" \
					"{"\
					"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
					"vec3 normalized_light_direction = normalize(light_direction);" \
					"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
					"vec3 ambient = u_La * u_Ka22;" \
					"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
					"vec3 diffuse = u_Ld * u_Kd22 * tn_dot_ld;" \
					"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
					"vec3 specular = u_Ls * u_Ks22 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material22_shininess);" \
					"phong_ads_color = ambient + diffuse + specular;" \
					"}"

					"else if(sphere == 23)" \
					"{"\
					"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
					"vec3 normalized_light_direction = normalize(light_direction);" \
					"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
					"vec3 ambient = u_La * u_Ka23;" \
					"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
					"vec3 diffuse = u_Ld * u_Kd23 * tn_dot_ld;" \
					"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
					"vec3 specular = u_Ls * u_Ks23 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material23_shininess);" \
					"phong_ads_color = ambient + diffuse + specular;" \
					"}"

					"else if(sphere == 24)" \
					"{"\
					"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
					"vec3 normalized_light_direction = normalize(light_direction);" \
					"vec3 normalized_viewer_vector = normalize(viewer_vector);" \
					"vec3 ambient = u_La * u_Ka24;" \
					"float tn_dot_ld = max(dot(normalized_transformed_normals, normalized_light_direction), 0.0);" \
					"vec3 diffuse = u_Ld * u_Kd24 * tn_dot_ld;" \
					"vec3 reflection_vector = reflect(-normalized_light_direction, normalized_transformed_normals);" \
					"vec3 specular = u_Ls * u_Ks24 * pow(max(dot(reflection_vector, normalized_viewer_vector), 0.0), u_material24_shininess);" \
					"phong_ads_color = ambient + diffuse + specular;" \
					"}"

		"}" \
		"else" \
		"{" \
		"phong_ads_color = vec3(1.0, 1.0, 1.0);" \
		"}"\
		"FragColor = vec4(phong_ads_color, 1.0);" \
		"}";
	glShaderSource(gFragmentShaderObject, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gFragmentShaderObject);
	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** SHADER PROGRAM ***
	// create
	gShaderProgramObject = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	// pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(gShaderProgramObject, ARD_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObject, ARD_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(gShaderProgramObject);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength>0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	gModelMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_model_matrix");
	gViewMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_view_matrix");
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObject, "u_projection_matrix");

	gLKeyPressedUniform = glGetUniformLocation(gShaderProgramObject, "u_lighting_enabled");
	gSphereNo = glGetUniformLocation(gShaderProgramObject, "sphere");

	gLaUniform = glGetUniformLocation(gShaderProgramObject, "u_La");
	gKa1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka1");
	gKa2Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka2");
	gKa3Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka3");
	gKa4Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka4");
	gKa5Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka5");
	gKa6Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka6");
	gKa7Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka7");
	gKa8Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka8");
	gKa9Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka9");
	gKa10Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka10");
	gKa11Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka11");
	gKa12Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka12");
	gKa13Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka13");
	gKa14Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka14");
	gKa15Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka15");
	gKa16Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka16");
	gKa17Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka17");
	gKa18Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka18");
	gKa19Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka19");
	gKa20Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka20");
	gKa21Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka21");
	gKa22Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka22");
	gKa23Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka23");
	gKa24Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka24");

	gLdUniform = glGetUniformLocation(gShaderProgramObject, "u_Ld");
	gKd1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd1");
	gKd2Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd2");
	gKd3Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd3");
	gKd4Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd4");
	gKd5Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd5");
	gKd6Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd6");
	gKd7Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd7");
	gKd8Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd8");
	gKd9Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd9");
	gKd10Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd10");
	gKd11Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd11");
	gKd12Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd12");
	gKd13Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd13");
	gKd14Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd14");
	gKd15Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd15");
	gKd16Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd16");
	gKd17Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd17");
	gKd18Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd18");
	gKd19Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd19");
	gKd20Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd20");
	gKd21Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd21");
	gKd22Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd22");
	gKd23Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd23");
	gKd24Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd24");

	gLsUniform = glGetUniformLocation(gShaderProgramObject, "u_Ls");
	gKs1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks1");
	gKs2Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks2");
	gKs3Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks3");
	gKs4Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks4");
	gKs5Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks5");
	gKs6Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks6");
	gKs7Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks7");
	gKs8Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks8");
	gKs9Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks9");
	gKs10Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks10");
	gKs11Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks11");
	gKs12Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks12");
	gKs13Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks13");
	gKs14Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks14");
	gKs15Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks15");
	gKs16Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks16");
	gKs17Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks17");
	gKs18Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks18");
	gKs19Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks19");
	gKs20Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks20");
	gKs21Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks21");
	gKs22Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks22");
	gKs23Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks23");
	gKs24Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks24");

	glight_position_uniform = glGetUniformLocation(gShaderProgramObject, "u_light_position");

	gmaterial1_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material1_shininess");
	gmaterial2_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material2_shininess");
	gmaterial3_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material3_shininess");
	gmaterial4_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material4_shininess");
	gmaterial5_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material5_shininess");
	gmaterial6_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material6_shininess");
	gmaterial7_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material7_shininess");
	gmaterial8_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material8_shininess");
	gmaterial9_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material9_shininess");
	gmaterial10_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material10_shininess");
	gmaterial11_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material11_shininess");
	gmaterial12_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material12_shininess");
	gmaterial13_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material13_shininess");
	gmaterial14_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material14_shininess");
	gmaterial15_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material15_shininess");
	gmaterial16_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material16_shininess");
	gmaterial17_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material17_shininess");
	gmaterial18_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material18_shininess");
	gmaterial19_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material19_shininess");
	gmaterial20_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material20_shininess");
	gmaterial21_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material21_shininess");
	gmaterial22_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material22_shininess");
	gmaterial23_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material23_shininess");
	gmaterial24_shininess_uniform = glGetUniformLocation(gShaderProgramObject, "u_material24_shininess");

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	// vao
	glGenVertexArrays(1, &gVao_sphere);
	glBindVertexArray(gVao_sphere);

	// position vbo
	glGenBuffers(1, &gVbo_sphere_position);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(ARD_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ARD_ATTRIBUTE_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// normal vbo
	glGenBuffers(1, &gVbo_sphere_normal);
	glBindBuffer(GL_ARRAY_BUFFER, gVbo_sphere_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);

	glVertexAttribPointer(ARD_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(ARD_ATTRIBUTE_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// element vbo
	glGenBuffers(1, &gVbo_sphere_element);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// unbind vao
	glBindVertexArray(0);


	//MessageBox(NULL, TEXT("After Sqaure final"), TEXT("In Init"), MB_OK);

	glShadeModel(GL_SMOOTH);
	// set-up depth buffer
	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);
	// We will always cull back faces for better performance
	//glEnable(GL_CULL_FACE);

	// set background color to which it will display even if it will empty. THIS LINE CAN BE IN drawRect().
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f); // black

	gPerspectiveProjectionMatrix = mat4::identity();

	gbLight = false;
	// resize
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// start using OpenGL program object
	glUseProgram(gShaderProgramObject);

	if (gbLight == true)
	{
		glUniform1i(gLKeyPressedUniform, 1);

		glUniform3fv(gLaUniform, 1, lightAmbient);
		//glUniform3fv(gKa1Uniform, 1, material1_ambient);

		glUniform3fv(gLdUniform, 1, lightDiffuse);
		//glUniform3fv(gKd1Uniform, 1, material1_diffuse);

		glUniform3fv(gLsUniform, 1, lightSpecular);
		//glUniform3fv(gKs1Uniform, 1, material1_specular);

		//glUniform1f(gmaterial1_shininess_uniform, material1_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		

		if (gbXaxis == GL_TRUE)
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = (float) 100 * sin(gfangleX_axis);
			lightPosition[2] = (float) 100 *cos(gfangleX_axis);
			lightPosition[3] = 1.0f;
		}

		if (gbYaxis == GL_TRUE)
		{
			lightPosition[0] = (float) 100 * cos(gfangleY_axis);
			lightPosition[1] = 0.0f;
			lightPosition[2] = (float) 100 * sin(gfangleY_axis);
			lightPosition[3] = 1.0f;
		}

		if (gbZaxis == GL_TRUE)
		{
			lightPosition[0] = (float) 100 * cos(gfangleZ_axis);
			lightPosition[1] = (float) 100 * sin(gfangleZ_axis);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;
		}

		glUniform4fv(glight_position_uniform, 1, (GLfloat *)lightPosition);
	}

	else
	{
		glUniform1i(gLKeyPressedUniform, 0);
	}
	//////////////////1st sphere////////////////////////////////


	//	for (float i = 0.0f, x = -3.0f ; i < 4.0f && x <= -1.0f; i = i + 1.0f, x = x - 2.0f)
	//{
	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();
	mat4 scaleMatrix = mat4::identity();
	modelMatrix = translate(-3.0f, 2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	glUniform1i(gSphereNo, 1);
	glUniform3fv(gKa1Uniform, 1, material1_ambient);
	glUniform3fv(gKd1Uniform, 1, material1_diffuse);
	glUniform3fv(gKs1Uniform, 1, material1_specular);
	glUniform1f(gmaterial1_shininess_uniform, material1_shininess);

	//gKa1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka1");
	//gKd1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd1");
	//gKs1Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks1");
	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	////////////////////////////////2nd sphere////////////////////
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(-1.0f, 2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform3fv(gKa2Uniform, 1, material2_ambient);
	glUniform3fv(gKd2Uniform, 1, material2_diffuse);
	glUniform3fv(gKs2Uniform, 1, material2_specular);
	glUniform1f(gmaterial2_shininess_uniform, material2_shininess);
	glUniform1i(gSphereNo, 2);
	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);
	//gKa2Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ka1");
	//gKd2Uniform = glGetUniformLocation(gShaderProgramObject, "u_Kd1");
	//gKs2Uniform = glGetUniformLocation(gShaderProgramObject, "u_Ks1");
	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////3rd sphere//////////////////////
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(1.0f, 2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 3);
	glUniform3fv(gKa3Uniform, 1, material3_ambient);
	glUniform3fv(gKd3Uniform, 1, material3_diffuse);
	glUniform3fv(gKs3Uniform, 1, material3_specular);
	glUniform1f(gmaterial3_shininess_uniform, material3_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);
	////////////////////////////////////////////4th sphere/////////////////
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(3.0f, 2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 4);
	glUniform3fv(gKa4Uniform, 1, material4_ambient);
	glUniform3fv(gKd4Uniform, 1, material4_diffuse);
	glUniform3fv(gKs4Uniform, 1, material4_specular);
	glUniform1f(gmaterial4_shininess_uniform, material4_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////5th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-3.0f, 1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 5);
	glUniform3fv(gKa5Uniform, 1, material5_ambient);
	glUniform3fv(gKd5Uniform, 1, material5_diffuse);
	glUniform3fv(gKs5Uniform, 1, material5_specular);
	glUniform1f(gmaterial5_shininess_uniform, material5_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	//////////////////////////////////////6th Sphere//////////////////////////////
	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(-1.0f, 1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 6);
	glUniform3fv(gKa6Uniform, 1, material6_ambient);
	glUniform3fv(gKd6Uniform, 1, material6_diffuse);
	glUniform3fv(gKs6Uniform, 1, material6_specular);
	glUniform1f(gmaterial6_shininess_uniform, material6_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	//////////////////////////////////////////7th Sphere////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(1.0f, 1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 7);
	glUniform3fv(gKa7Uniform, 1, material7_ambient);
	glUniform3fv(gKd7Uniform, 1, material7_diffuse);
	glUniform3fv(gKs7Uniform, 1, material7_specular);
	glUniform1f(gmaterial7_shininess_uniform, material7_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	//////////////////////////////////////////8th Sphere//////////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(3.0f, 1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 8);
	glUniform3fv(gKa8Uniform, 1, material8_ambient);
	glUniform3fv(gKd8Uniform, 1, material8_diffuse);
	glUniform3fv(gKs8Uniform, 1, material8_specular);
	glUniform1f(gmaterial8_shininess_uniform, material8_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);



	/////////////////////////////////////9th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-3.0f, 0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 9);
	glUniform3fv(gKa9Uniform, 1, material9_ambient);
	glUniform3fv(gKd9Uniform, 1, material9_diffuse);
	glUniform3fv(gKs9Uniform, 1, material9_specular);
	glUniform1f(gmaterial9_shininess_uniform, material9_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);



	///////////////////////////////////////10th Sphere///////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(-1.0f, 0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 10);
	glUniform3fv(gKa10Uniform, 1, material10_ambient);
	glUniform3fv(gKd10Uniform, 1, material10_diffuse);
	glUniform3fv(gKs10Uniform, 1, material10_specular);
	glUniform1f(gmaterial10_shininess_uniform, material10_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	////////////////////////////////////////11th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(1.0f, 0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 11);
	glUniform3fv(gKa11Uniform, 1, material11_ambient);
	glUniform3fv(gKd11Uniform, 1, material11_diffuse);
	glUniform3fv(gKs11Uniform, 1, material11_specular);
	glUniform1f(gmaterial11_shininess_uniform, material11_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////////////////12th Sphere/////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();

	modelMatrix = translate(3.0f, 0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 12);
	glUniform3fv(gKa12Uniform, 1, material12_ambient);
	glUniform3fv(gKd12Uniform, 1, material12_diffuse);
	glUniform3fv(gKs12Uniform, 1, material12_specular);
	glUniform1f(gmaterial12_shininess_uniform, material12_shininess);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);




	/////////////////////////////////////13th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-3.0f, -0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 13);
	glUniform3fv(gKa13Uniform, 1, material13_ambient);
	glUniform3fv(gKd13Uniform, 1, material13_diffuse);
	glUniform3fv(gKs13Uniform, 1, material13_specular);
	glUniform1f(gmaterial13_shininess_uniform, material13_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////14th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-1.0f, -0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 14);
	glUniform3fv(gKa14Uniform, 1, material14_ambient);
	glUniform3fv(gKd14Uniform, 1, material14_diffuse);
	glUniform3fv(gKs14Uniform, 1, material14_specular);
	glUniform1f(gmaterial14_shininess_uniform, material14_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////15th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(1.0f, -0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 15);
	glUniform3fv(gKa15Uniform, 1, material15_ambient);
	glUniform3fv(gKd15Uniform, 1, material15_diffuse);
	glUniform3fv(gKs15Uniform, 1, material15_specular);
	glUniform1f(gmaterial15_shininess_uniform, material15_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////16th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(3.0f, -0.4f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 16);
	glUniform3fv(gKa16Uniform, 1, material16_ambient);
	glUniform3fv(gKd16Uniform, 1, material16_diffuse);
	glUniform3fv(gKs16Uniform, 1, material16_specular);
	glUniform1f(gmaterial16_shininess_uniform, material16_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////17th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-3.0f, -1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 17);
	glUniform3fv(gKa17Uniform, 1, material17_ambient);
	glUniform3fv(gKd17Uniform, 1, material17_diffuse);
	glUniform3fv(gKs17Uniform, 1, material17_specular);
	glUniform1f(gmaterial17_shininess_uniform, material17_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	/////////////////////////////////////18th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-1.0f, -1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 18);
	glUniform3fv(gKa18Uniform, 1, material18_ambient);
	glUniform3fv(gKd18Uniform, 1, material18_diffuse);
	glUniform3fv(gKs18Uniform, 1, material18_specular);
	glUniform1f(gmaterial18_shininess_uniform, material18_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////19th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(1.0f, -1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 19);
	glUniform3fv(gKa19Uniform, 1, material19_ambient);
	glUniform3fv(gKd19Uniform, 1, material19_diffuse);
	glUniform3fv(gKs19Uniform, 1, material19_specular);
	glUniform1f(gmaterial19_shininess_uniform, material19_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	/////////////////////////////////////20th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(3.0f, -1.2f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 20);
	glUniform3fv(gKa20Uniform, 1, material20_ambient);
	glUniform3fv(gKd20Uniform, 1, material20_diffuse);
	glUniform3fv(gKs20Uniform, 1, material20_specular);
	glUniform1f(gmaterial20_shininess_uniform, material20_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	/////////////////////////////////////21th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-3.0f, -2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 21);
	glUniform3fv(gKa21Uniform, 1, material21_ambient);
	glUniform3fv(gKd21Uniform, 1, material21_diffuse);
	glUniform3fv(gKs21Uniform, 1, material21_specular);
	glUniform1f(gmaterial21_shininess_uniform, material21_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	/////////////////////////////////////22th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(-1.0f, -2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 22);
	glUniform3fv(gKa22Uniform, 1, material22_ambient);
	glUniform3fv(gKd22Uniform, 1, material22_diffuse);
	glUniform3fv(gKs22Uniform, 1, material22_specular);
	glUniform1f(gmaterial22_shininess_uniform, material22_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	/////////////////////////////////////23th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(1.0f, -2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 23);
	glUniform3fv(gKa23Uniform, 1, material23_ambient);
	glUniform3fv(gKd23Uniform, 1, material23_diffuse);
	glUniform3fv(gKs23Uniform, 1, material23_specular);
	glUniform1f(gmaterial23_shininess_uniform, material23_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);


	/////////////////////////////////////24th Sphere///////////////////////////////////

	modelMatrix = mat4::identity();
	viewMatrix = mat4::identity();
	scaleMatrix = mat4::identity();
	modelMatrix = translate(3.0f, -2.0f, -6.0f);
	modelMatrix = modelMatrix * scale(0.50f, 0.50f, 0.50f);
	//modelMatrix = scale(1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);
	glUniform1i(gSphereNo, 24);
	glUniform3fv(gKa24Uniform, 1, material24_ambient);
	glUniform3fv(gKd24Uniform, 1, material24_diffuse);
	glUniform3fv(gKs24Uniform, 1, material24_specular);
	glUniform1f(gmaterial24_shininess_uniform, material24_shininess);


	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	// stop using OpenGL program object
	glUseProgram(0);

	SwapBuffers(ghdc);
}

void resize(int width, int height)
{
	//code
	if (height == 0)
		height = 1;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	gPerspectiveProjectionMatrix = vmath::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
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

	if (gVao_sphere)
	{
		glDeleteVertexArrays(1, &gVao_sphere);
		gVao_sphere = 0;
	}

	// destroy vbo
	if (gVbo_sphere_element)
	{
		glDeleteBuffers(1, &gVbo_sphere_element);
		gVbo_sphere_element = 0;
	}

	if (gVbo_sphere_normal)
	{
		glDeleteBuffers(1, &gVbo_sphere_normal);
		gVbo_sphere_normal = 0;
	}

	if (gVbo_sphere_position)
	{
		glDeleteBuffers(1, &gVbo_sphere_position);
		gVbo_sphere_position = 0;
	}

	// detach vertex shader from shader program object
	glDetachShader(gShaderProgramObject, gVertexShaderObject);
	// detach fragment  shader from shader program object
	glDetachShader(gShaderProgramObject, gFragmentShaderObject);

	// delete vertex shader object
	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;
	// delete fragment shader object
	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	// delete shader program object
	glDeleteProgram(gShaderProgramObject);
	gShaderProgramObject = 0;

	// unlink shader program
	glUseProgram(0);

	//Deselect the rendering context
	wglMakeCurrent(NULL, NULL);

	//Delete the rendering context
	wglDeleteContext(ghrc);
	ghrc = NULL;

	//Delete the device context
	ReleaseDC(ghwnd, ghdc);
	ghdc = NULL;

	if (gpFile)
	{
		fprintf(gpFile, "Log File Is Successfully Closed.\n");
		fclose(gpFile);
		gpFile = NULL;
	}
}

void spin(void)
{

		gfangleX_axis = gfangleX_axis + 0.001f;

		if (gfangleX_axis >= 360.0f)
		{
			gfangleX_axis = gfangleX_axis - 360.0f;
		}
	

		gfangleY_axis = gfangleY_axis + 0.001f;

		if (gfangleY_axis >= 360.0f)
		{
			gfangleY_axis = gfangleY_axis - 360.0f;
		}
	
		gfangleZ_axis = gfangleZ_axis + 0.001f;

		if (gfangleZ_axis >= 360.0f)
		{
			gfangleZ_axis = gfangleZ_axis - 360.0f;
		}
	
}
