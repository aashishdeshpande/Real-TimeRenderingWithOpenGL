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

GLuint gVertexShaderObjectPhongFragment;
GLuint gFragmentShaderObjectPhongFragment;

GLuint gVertexShaderObjectPhongVertex;
GLuint gFragmentShaderObjectPhongVertex;

GLuint gShaderProgramObjectPhongVertex;
GLuint gShaderProgramObjectPhongFragment;

GLuint gVao_sphere;

GLuint gVbo_sphere_position;
GLuint gVbo_sphere_normal;
GLuint gVbo_sphere_element;

GLuint gNumVertices;
GLuint gNumElements;

GLuint gModelMatrixUniform, gViewMatrixUniform, gProjectionMatrixUniform;

GLuint gLd0Uniform;
GLuint gLa0Uniform;
GLuint gLs0Uniform;

GLuint gLd1Uniform;
GLuint gLa1Uniform;
GLuint gLs1Uniform;

GLuint gLd2Uniform;
GLuint gLa2Uniform;
GLuint gLs2Uniform;

GLuint gKa0Uniform;
GLuint gKd0Uniform;
GLuint gKs0Uniform;

GLuint gKa1Uniform;
GLuint gKd1Uniform;
GLuint gKs1Uniform;

GLuint gKa2Uniform;
GLuint gKd2Uniform;
GLuint gKs2Uniform;

GLuint gmaterial0_shininess_uniform;
GLuint gmaterial1_shininess_uniform;
GLuint gmaterial2_shininess_uniform;

GLuint gModelMatrixUniformf, gViewMatrixUniformf, gProjectionMatrixUniformf;

GLuint gLd0Uniformf;
GLuint gLa0Uniformf;
GLuint gLs0Uniformf;

GLuint gLd1Uniformf;
GLuint gLa1Uniformf;
GLuint gLs1Uniformf;

GLuint gLd2Uniformf;
GLuint gLa2Uniformf;
GLuint gLs2Uniformf;

GLuint gKa0Uniformf;
GLuint gKd0Uniformf;
GLuint gKs0Uniformf;

GLuint gKa1Uniformf;
GLuint gKd1Uniformf;
GLuint gKs1Uniformf;

GLuint gKa2Uniformf;
GLuint gKd2Uniformf;
GLuint gKs2Uniformf;

GLuint gmaterial0_shininess_uniformf;
GLuint gmaterial1_shininess_uniformf;
GLuint gmaterial2_shininess_uniformf;
//GLuint gmaterial1_shininess_uniform;

GLuint glight0_position_uniform;
GLuint glight1_position_uniform;
GLuint glight2_position_uniform;

GLuint glight0_position_uniformf;
GLuint glight1_position_uniformf;
GLuint glight2_position_uniformf;


mat4 gPerspectiveProjectionMatrix;

bool gbPerVertex = true;
bool gbPerFragment = false;

GLfloat gfangle_red = 0.0f;
GLfloat gfangle_green = 0.0f;
GLfloat gfangle_blue = 0.0f;

GLfloat lightAmbient0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightSpecular0[] = { 1.0f, 0.0f, 0.0f, 1.0f };
//GLfloat lightPosition0[] = { 100.0f, 100.0f, 100.0f, 1.0f };
//GLfloat lightPosition0[4];

GLfloat lightAmbient1[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat lightSpecular1[] = { 0.0f, 1.0f, 0.0f, 1.0f };
//GLfloat lightPosition1[] = { -100.0, 100.0f, 100.0f, 1.0f };
//GLfloat lightPosition1[4];

GLfloat lightAmbient2[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat lightDiffuse2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
GLfloat lightSpecular2[] = { 0.0f, 0.0f, 1.0f, 1.0f };
//GLfloat lightPosition2[] = { 0.0f, 0.0f, 100.0f, 1.0f };
//GLfloat lightPosition2[4];

GLfloat material_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat material_shininess = 50.0f;
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
		TEXT("OpenGL Programmable Pipeline Window : Rotating Lights Static Sphere"),
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

		case 0x51:
			if (gbEscapeKeyIsPressed == false)
				gbEscapeKeyIsPressed = true;
			break;

		case 0x56:
			if (gbPerVertex == false)
			{
				gbPerVertex = true;
				gbPerFragment = false;
			}
			break;

		case 0x46:
			if (gbPerFragment == false)
			{
				gbPerFragment = true;
				gbPerVertex = false;
			}
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

	gVertexShaderObjectPhongVertex = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCodePerVertex =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec3 u_La0;" \
		"uniform vec3 u_La1;" \
		"uniform vec3 u_La2;" \
		"uniform vec3 u_Ka0;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ka2;" \
		"uniform vec3 u_Ld0;" \
		"uniform vec3 u_Ld1;" \
		"uniform vec3 u_Ld2;" \
		"uniform vec3 u_Kd0;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Kd2;" \
		"uniform vec3 u_Ls0;" \
		"uniform vec3 u_Ls1;" \
		"uniform vec3 u_Ls2;" \
		"uniform vec3 u_Ks0;" \
		"uniform vec3 u_Ks1;" \
		"uniform vec3 u_Ks2;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform vec4 u_light2_position;" \
		"uniform float u_material0_shininess;" \
		"uniform float u_material1_shininess;" \
		"uniform float u_material2_shininess;" \
		"out vec3 phong_ads_color0;" \
		"out vec3 phong_ads_color1;" \
		"out vec3 phong_ads_color2;" \
		"void main(void)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \

		"vec3 transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \

		"vec3 light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);" \
		"vec3 light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);" \
		"vec3 light_direction2 = normalize(vec3(u_light2_position) - eyeCoordinates.xyz);" \

		"float tn_dot_ld0 = max(dot(transformed_normals, light_direction0), 0.0);" \
		"float tn_dot_ld1 = max(dot(transformed_normals, light_direction1), 0.0);" \
		"float tn_dot_ld2 = max(dot(transformed_normals, light_direction2), 0.0);" \

		"vec3 ambient0 = u_La0 * u_Ka0;" \
		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;" \
		"vec3 reflection_vector0 = reflect(-light_direction0, transformed_normals);" \

		"vec3 ambient1 = u_La1 * u_Ka1;" \
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;" \
		"vec3 reflection_vector1 = reflect(-light_direction1, transformed_normals);" \

		"vec3 ambient2 = u_La2 * u_Ka2;" \
		"vec3 diffuse2 = u_Ld2 * u_Kd2 * tn_dot_ld2;" \
		"vec3 reflection_vector2 = reflect(-light_direction2, transformed_normals);" \


		"vec3 viewer_vector = normalize(-eyeCoordinates.xyz);"

		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection_vector0, viewer_vector), 0.0), u_material0_shininess);" \
		"phong_ads_color0 = ambient0 + diffuse0 + specular0;" \

		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection_vector1, viewer_vector), 0.0), u_material1_shininess);" \
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;" \


		"vec3 specular2 = u_Ls2 * u_Ks2 * pow(max(dot(reflection_vector2, viewer_vector), 0.0), u_material2_shininess);" \
		"phong_ads_color2 = ambient2 + diffuse2 + specular2;" \

		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(gVertexShaderObjectPhongVertex, 1, (const GLchar **)&vertexShaderSourceCodePerVertex, NULL);

	// compile shader
	glCompileShader(gVertexShaderObjectPhongVertex);
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	char *szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObjectPhongVertex, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObjectPhongVertex, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObjectPhongVertex, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	gFragmentShaderObjectPhongVertex = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCodePerVertex =
		"#version 450 core" \
		"\n" \
		"in vec3 phong_ads_color0;" \
		"in vec3 phong_ads_color1;" \
		"in vec3 phong_ads_color2;" \
		"out vec4 FragColor0;" \
		"void main(void)" \
		"{" \
		"FragColor0 = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0) + vec4(phong_ads_color2, 1.0); " \
		"}";
	glShaderSource(gFragmentShaderObjectPhongVertex, 1, (const GLchar **)&fragmentShaderSourceCodePerVertex, NULL);

	// compile shader
	glCompileShader(gFragmentShaderObjectPhongVertex);
	glGetShaderiv(gFragmentShaderObjectPhongVertex, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObjectPhongVertex, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObjectPhongVertex, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** SHADER PROGRAM ***
	// create
	gShaderProgramObjectPhongVertex = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObjectPhongVertex, gVertexShaderObjectPhongVertex);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObjectPhongVertex, gFragmentShaderObjectPhongVertex);

	// pre-link binding of shader program object with vertex shader position attribute
	glBindAttribLocation(gShaderProgramObjectPhongVertex, ARD_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObjectPhongVertex, ARD_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(gShaderProgramObjectPhongVertex);
	GLint iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObjectPhongVertex, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObjectPhongVertex, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength>0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObjectPhongVertex, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	//Fragment Shader//

	// *** VERTEX SHADER ***
	// create shader
	gVertexShaderObjectPhongFragment = glCreateShader(GL_VERTEX_SHADER);

	// provide source code to shader
	const GLchar *vertexShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec4 vPosition;" \
		"in vec3 vNormal;" \
		"uniform mat4 u_model_matrix;" \
		"uniform mat4 u_view_matrix;" \
		"uniform mat4 u_projection_matrix;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform vec4 u_light2_position;" \
		"out vec3 transformed_normals;" \
		"out vec3 light_direction0;" \
		"out vec3 light_direction1;" \
		"out vec3 light_direction2;" \
		"out vec3 viewer_vector;" \
		"void main(void)" \
		"{" \
		"vec4 eyeCoordinates = u_view_matrix * u_model_matrix * vPosition;" \
		"transformed_normals = normalize(mat3(u_view_matrix * u_model_matrix) * vNormal);" \
		"light_direction0 = normalize(vec3(u_light0_position) - eyeCoordinates.xyz);" \
		"light_direction1 = normalize(vec3(u_light1_position) - eyeCoordinates.xyz);" \
		"light_direction2 = normalize(vec3(u_light2_position) - eyeCoordinates.xyz);" \
		"viewer_vector = normalize(-eyeCoordinates.xyz);"
		"gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * vPosition;" \
		"}";
	glShaderSource(gVertexShaderObjectPhongFragment, 1, (const GLchar **)&vertexShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gVertexShaderObjectPhongFragment);
	iInfoLogLength = 0;
	iShaderCompiledStatus = 0;
	szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObjectPhongFragment, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gVertexShaderObjectPhongFragment, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gVertexShaderObjectPhongFragment, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Vertex Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}

	// *** FRAGMENT SHADER ***
	// create shader
	gFragmentShaderObjectPhongFragment = glCreateShader(GL_FRAGMENT_SHADER);

	// provide source code to shader
	const GLchar *fragmentShaderSourceCode =
		"#version 450 core" \
		"\n" \
		"in vec3 transformed_normals;" \
		"in vec3 light_direction0;" \
		"in vec3 light_direction1;" \
		"in vec3 light_direction2;" \
		"in vec3 viewer_vector;" \
		"out vec4 FragColor;" \
		"uniform vec3 u_La0;" \
		"uniform vec3 u_La1;" \
		"uniform vec3 u_La2;" \
		"uniform vec3 u_Ka0;" \
		"uniform vec3 u_Ka1;" \
		"uniform vec3 u_Ka2;" \
		"uniform vec3 u_Ld0;" \
		"uniform vec3 u_Ld1;" \
		"uniform vec3 u_Ld2;" \
		"uniform vec3 u_Kd0;" \
		"uniform vec3 u_Kd1;" \
		"uniform vec3 u_Kd2;" \
		"uniform vec3 u_Ls0;" \
		"uniform vec3 u_Ls1;" \
		"uniform vec3 u_Ls2;" \
		"uniform vec3 u_Ks0;" \
		"uniform vec3 u_Ks1;" \
		"uniform vec3 u_Ks2;" \
		"uniform vec4 u_light0_position;" \
		"uniform vec4 u_light1_position;" \
		"uniform vec4 u_light2_position;" \
		"uniform float u_material0_shininess;" \
		"uniform float u_material1_shininess;" \
		"uniform float u_material2_shininess;" \
		"void main(void)" \
		"{" \
		"vec3 phong_ads_color0;" \
		"vec3 phong_ads_color1;" \
		"vec3 phong_ads_color2;" \
		"vec3 normalized_transformed_normals = normalize(transformed_normals);" \
		"vec3 normalized_light_direction0 = normalize(light_direction0);" \
		"vec3 normalized_light_direction1 = normalize(light_direction1);" \
		"vec3 normalized_light_direction2 = normalize(light_direction2);" \

		"vec3 normalized_viewer_vector1 = normalize(viewer_vector);" \
		"vec3 ambient0 = u_La0 * u_Ka0;" \
		"vec3 ambient1 = u_La1 * u_Ka1;" \
		"vec3 ambient2 = u_La2 * u_Ka2;" \

		"float tn_dot_ld0 = max(dot(normalized_transformed_normals, normalized_light_direction0), 0.0);" \
		"float tn_dot_ld1 = max(dot(normalized_transformed_normals, normalized_light_direction1), 0.0);" \
		"float tn_dot_ld2 = max(dot(normalized_transformed_normals, normalized_light_direction2), 0.0);" \

		"vec3 diffuse0 = u_Ld0 * u_Kd0 * tn_dot_ld0;" \
		"vec3 diffuse1 = u_Ld1 * u_Kd1 * tn_dot_ld1;" \
		"vec3 diffuse2 = u_Ld2 * u_Kd2 * tn_dot_ld2;" \

		"vec3 reflection0_vector = reflect(-normalized_light_direction0, normalized_transformed_normals);" \
		"vec3 reflection1_vector = reflect(-normalized_light_direction1, normalized_transformed_normals);" \
		"vec3 reflection2_vector = reflect(-normalized_light_direction2, normalized_transformed_normals);" \

		"vec3 specular0 = u_Ls0 * u_Ks0 * pow(max(dot(reflection0_vector, normalized_viewer_vector1), 0.0), u_material0_shininess);" \
		"vec3 specular1 = u_Ls1 * u_Ks1 * pow(max(dot(reflection1_vector, normalized_viewer_vector1), 0.0), u_material1_shininess);" \
		"vec3 specular2 = u_Ls2 * u_Ks2 * pow(max(dot(reflection2_vector, normalized_viewer_vector1), 0.0), u_material2_shininess);" \

		"phong_ads_color0 = ambient0 + diffuse0 + specular0;" \
		"phong_ads_color1 = ambient1 + diffuse1 + specular1;" \
		"phong_ads_color2 = ambient2 + diffuse2 + specular2;" \

		"FragColor = vec4(phong_ads_color0, 1.0) + vec4(phong_ads_color1, 1.0) + vec4(phong_ads_color2, 1.0);" \
		"}";
	glShaderSource(gFragmentShaderObjectPhongFragment, 1, (const GLchar **)&fragmentShaderSourceCode, NULL);

	// compile shader
	glCompileShader(gFragmentShaderObjectPhongFragment);
	glGetShaderiv(gFragmentShaderObjectPhongFragment, GL_COMPILE_STATUS, &iShaderCompiledStatus);
	if (iShaderCompiledStatus == GL_FALSE)
	{
		glGetShaderiv(gFragmentShaderObjectPhongFragment, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetShaderInfoLog(gFragmentShaderObjectPhongFragment, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Fragment Shader Compilation Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}



	// *** SHADER PROGRAM ***
	// create
	gShaderProgramObjectPhongFragment = glCreateProgram();

	// attach vertex shader to shader program
	glAttachShader(gShaderProgramObjectPhongFragment, gVertexShaderObjectPhongFragment);

	// attach fragment shader to shader program
	glAttachShader(gShaderProgramObjectPhongFragment, gFragmentShaderObjectPhongFragment);

	// pre-link binding of shader program object with vertex shader position attribute
	//glBindAttribLocation(gShaderProgramObjectPhongVertex, ARD_ATTRIBUTE_POSITION, "vPosition");
	//glBindAttribLocation(gShaderProgramObjectPhongVertex, ARD_ATTRIBUTE_NORMAL, "vNormal");

	glBindAttribLocation(gShaderProgramObjectPhongFragment, ARD_ATTRIBUTE_POSITION, "vPosition");
	glBindAttribLocation(gShaderProgramObjectPhongFragment, ARD_ATTRIBUTE_NORMAL, "vNormal");

	// link shader
	glLinkProgram(gShaderProgramObjectPhongFragment);
	iShaderProgramLinkStatus = 0;
	glGetProgramiv(gShaderProgramObjectPhongFragment, GL_LINK_STATUS, &iShaderProgramLinkStatus);
	if (iShaderProgramLinkStatus == GL_FALSE)
	{
		glGetProgramiv(gShaderProgramObjectPhongFragment, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength>0)
		{
			szInfoLog = (char *)malloc(iInfoLogLength);
			if (szInfoLog != NULL)
			{
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObjectPhongFragment, iInfoLogLength, &written, szInfoLog);
				fprintf(gpFile, "Shader Program Link Log : %s\n", szInfoLog);
				free(szInfoLog);
				uninitialize();
				exit(0);
			}
		}
	}


	gModelMatrixUniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_model_matrix");
	gViewMatrixUniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_view_matrix");
	gProjectionMatrixUniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_projection_matrix");

	
	gLa1Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_La1");
	gKa1Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ka1");

	gLd1Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ld1");
	gKd1Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Kd1");

	gLs1Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ls1");
	gKs1Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ks1");

	glight1_position_uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_light1_position");

	gmaterial1_shininess_uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_material1_shininess");

	gLa0Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_La0");
	gKa0Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ka0");

	gLd0Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ld0");
	gKd0Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Kd0");

	gLs0Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ls0");
	gKs0Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ks0");

	glight0_position_uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_light0_position");

	gmaterial0_shininess_uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_material0_shininess");

	gLa2Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_La2");
	gKa2Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ka2");

	gLd2Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ld2");
	gKd2Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Kd2");

	gLs2Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ls2");
	gKs2Uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_Ks2");

	glight2_position_uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_light2_position");

	gmaterial2_shininess_uniform = glGetUniformLocation(gShaderProgramObjectPhongVertex, "u_material2_shininess");


	//uniform for fragment shader

	gModelMatrixUniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_model_matrix");
	gViewMatrixUniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_view_matrix");
	gProjectionMatrixUniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_projection_matrix");


	gLa1Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_La1");
	gKa1Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ka1");

	gLd1Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ld1");
	gKd1Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Kd1");

	gLs1Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ls1");
	gKs1Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ks1");

	glight1_position_uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_light1_position");

	gmaterial1_shininess_uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_material1_shininess");

	gLa0Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_La0");
	gKa0Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ka0");

	gLd0Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ld0");
	gKd0Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Kd0");

	gLs0Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ls0");
	gKs0Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ks0");

	glight0_position_uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_light0_position");

	gmaterial0_shininess_uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_material0_shininess");

	gLa2Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_La2");
	gKa2Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ka2");

	gLd2Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ld2");
	gKd2Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Kd2");

	gLs2Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ls2");
	gKs2Uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_Ks2");

	glight2_position_uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_light2_position");

	gmaterial2_shininess_uniformf = glGetUniformLocation(gShaderProgramObjectPhongFragment, "u_material2_shininess");




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

	//glShadeModel(GL_SMOOTH);
	// set-up depth buffer
	glClearDepth(1.0f);
	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// depth test to do
	glDepthFunc(GL_LEQUAL);
	// We will always cull back faces for better performance
	//glEnable(GL_CULL_FACE);

	// set background color to which it will display even if it will empty. THIS LINE CAN BE IN drawRect().
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // black

	gPerspectiveProjectionMatrix = mat4::identity();
	// resize
	resize(WIN_WIDTH, WIN_HEIGHT);
}

void display(void)
{
	//code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat lightPosition0[4] = {0.0f, (float)2 * sin(gfangle_red), (float)2 * cos(gfangle_red), 1.0f};
	//GLfloat lightPosition0[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat lightPosition1[4] = { 2 * (float)cos(gfangle_green), 0.0f, (float)2* sin(gfangle_green), 1.0 };
	GLfloat lightPosition2[4] = { 2 * (float)cos(gfangle_blue) , (float)2* sin(gfangle_blue), 0.0f, 1.0 };
	/*	if (gbPerFragment == true)
	{
	// start using OpenGL program object
	glUseProgram(gShaderProgramObjectPhongFragment);

	if (gbLight == true)
	{
	glUniform1i(gLKeyPressedUniform, 1);

	glUniform3fv(gLaUniform, 1, lightAmbient);
	glUniform3fv(gKaUniform, 1, material_ambient);

	glUniform3fv(gLdUniform, 1, lightDiffuse);
	glUniform3fv(gKdUniform, 1, material_diffuse);

	glUniform3fv(gLsUniform, 1, lightSpecular);
	glUniform3fv(gKsUniform, 1, material_specular);

	glUniform1f(gmaterial_shininess_uniform, material_shininess);

	//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	glUniform4fv(glight_position_uniform, 1, (GLfloat *)lightPosition);
	}

	else
	{
	glUniform1i(gLKeyPressedUniform, 0);
	}

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	// stop using OpenGL program object
	glUseProgram(0);

	}

	if (gbPerVertex == true)
	{
	glUseProgram(gShaderProgramObjectPhongVertex);

	if (gbLight == true)
	{
	glUniform1i(gLKeyPressedUniform, 1);

	glUniform3fv(gLaUniform, 1, lightAmbient);
	glUniform3fv(gKaUniform, 1, material_ambient);

	glUniform3fv(gLdUniform, 1, lightDiffuse);
	glUniform3fv(gKdUniform, 1, material_diffuse);

	glUniform3fv(gLsUniform, 1, lightSpecular);
	glUniform3fv(gKsUniform, 1, material_specular);

	glUniform1f(gmaterial_shininess_uniform, material_shininess);

	//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
	glUniform4fv(glight_position_uniform, 1, (GLfloat *)lightPosition);
	}

	else
	{
	glUniform1i(gLKeyPressedUniform, 0);
	}

	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);

	glUniformMatrix4fv(gProjectionMatrixUniform, 1, GL_FALSE, gPerspectiveProjectionMatrix);

	glBindVertexArray(gVao_sphere);

	// *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gVbo_sphere_element);
	glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0);

	// *** unbind vao ***
	glBindVertexArray(0);

	// stop using OpenGL program object
	glUseProgram(0);

	}*/

	if (gbPerVertex == true)
	{
		glUseProgram(gShaderProgramObjectPhongVertex);
	}

	else
	{
		glUseProgram(gShaderProgramObjectPhongFragment);
	}

		glUniform3fv(gLa0Uniform, 1, lightAmbient0);
		glUniform3fv(gKa0Uniform, 1, material_ambient);

		glUniform3fv(gLd0Uniform, 1, lightDiffuse0);
		glUniform3fv(gKd0Uniform, 1, material_diffuse);

		glUniform3fv(gLs0Uniform, 1, lightSpecular0);
		glUniform3fv(gKs0Uniform, 1, material_specular);

		glUniform1f(gmaterial0_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(glight0_position_uniform, 1, (GLfloat *)lightPosition0);
	
		glUniform3fv(gLa1Uniform, 1, lightAmbient1);
		glUniform3fv(gKa1Uniform, 1, material_ambient);

		glUniform3fv(gLd1Uniform, 1, lightDiffuse1);
		glUniform3fv(gKd1Uniform, 1, material_diffuse);

		glUniform3fv(gLs1Uniform, 1, lightSpecular1);
		glUniform3fv(gKs1Uniform, 1, material_specular);

		glUniform1f(gmaterial1_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(glight1_position_uniform, 1, (GLfloat *)lightPosition1);

		glUniform3fv(gLa2Uniform, 1, lightAmbient2);
		glUniform3fv(gKa2Uniform, 1, material_ambient);

		glUniform3fv(gLd2Uniform, 1, lightDiffuse2);
		glUniform3fv(gKd2Uniform, 1, material_diffuse);

		glUniform3fv(gLs2Uniform, 1, lightSpecular2);
		glUniform3fv(gKs2Uniform, 1, material_specular);

		glUniform1f(gmaterial2_shininess_uniform, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(glight2_position_uniform, 1, (GLfloat *)lightPosition2);

		///////////////////////////////////////////////////////////////////
		glUniform3fv(gLa0Uniformf, 1, lightAmbient0);
		glUniform3fv(gKa0Uniformf, 1, material_ambient);

		glUniform3fv(gLd0Uniformf, 1, lightDiffuse0);
		glUniform3fv(gKd0Uniformf, 1, material_diffuse);

		glUniform3fv(gLs0Uniformf, 1, lightSpecular0);
		glUniform3fv(gKs0Uniformf, 1, material_specular);

		glUniform1f(gmaterial0_shininess_uniformf, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(glight0_position_uniformf, 1, (GLfloat *)lightPosition0);

		glUniform3fv(gLa1Uniformf, 1, lightAmbient1);
		glUniform3fv(gKa1Uniformf, 1, material_ambient);

		glUniform3fv(gLd1Uniformf, 1, lightDiffuse1);
		glUniform3fv(gKd1Uniformf, 1, material_diffuse);

		glUniform3fv(gLs1Uniformf, 1, lightSpecular1);
		glUniform3fv(gKs1Uniformf, 1, material_specular);

		glUniform1f(gmaterial1_shininess_uniformf, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(glight1_position_uniformf, 1, (GLfloat *)lightPosition1);

		glUniform3fv(gLa2Uniformf, 1, lightAmbient2);
		glUniform3fv(gKa2Uniformf, 1, material_ambient);

		glUniform3fv(gLd2Uniformf, 1, lightDiffuse2);
		glUniform3fv(gKd2Uniformf, 1, material_diffuse);

		glUniform3fv(gLs2Uniformf, 1, lightSpecular2);
		glUniform3fv(gKs2Uniformf, 1, material_specular);

		glUniform1f(gmaterial2_shininess_uniformf, material_shininess);

		//float lightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
		glUniform4fv(glight2_position_uniformf, 1, (GLfloat *)lightPosition2);



	mat4 modelMatrix = mat4::identity();
	mat4 viewMatrix = mat4::identity();

	modelMatrix = translate(0.0f, 0.0f, -2.0f);

	glUniformMatrix4fv(gModelMatrixUniform, 1, GL_FALSE, modelMatrix);
	glUniformMatrix4fv(gViewMatrixUniform, 1, GL_FALSE, viewMatrix);

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
	glDetachShader(gShaderProgramObjectPhongFragment, gVertexShaderObjectPhongFragment);
	// detach fragment  shader from shader program object
	glDetachShader(gShaderProgramObjectPhongFragment, gFragmentShaderObjectPhongFragment);

	glDetachShader(gShaderProgramObjectPhongVertex, gVertexShaderObjectPhongVertex);
	// detach fragment  shader from shader program object
	glDetachShader(gShaderProgramObjectPhongVertex, gFragmentShaderObjectPhongVertex);

	// delete vertex shader object
	glDeleteShader(gVertexShaderObjectPhongFragment);
	gVertexShaderObjectPhongFragment = 0;
	// delete fragment shader object
	glDeleteShader(gFragmentShaderObjectPhongFragment);
	gFragmentShaderObjectPhongFragment = 0;

	// delete shader program object
	glDeleteProgram(gShaderProgramObjectPhongFragment);
	gShaderProgramObjectPhongFragment = 0;

	glDeleteShader(gVertexShaderObjectPhongVertex);
	gVertexShaderObjectPhongVertex = 0;
	// delete fragment shader object
	glDeleteShader(gFragmentShaderObjectPhongVertex);
	gFragmentShaderObjectPhongVertex = 0;

	// delete shader program object
	glDeleteProgram(gShaderProgramObjectPhongVertex);
	gShaderProgramObjectPhongVertex = 0;

	// unlink shader program
	//glUseProgram(0);

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
	gfangle_red = gfangle_red + 0.05f;
	if (gfangle_red > 360.0f)
		gfangle_red = gfangle_red - 360.0f;

	gfangle_green = gfangle_green + 0.05f;
	if (gfangle_green > 360.0f)
		gfangle_green = gfangle_green - 360.0f;

	gfangle_blue = gfangle_blue + 0.05f;
	if (gfangle_blue > 360.0f)
		gfangle_blue = gfangle_blue - 360.0f;
}

