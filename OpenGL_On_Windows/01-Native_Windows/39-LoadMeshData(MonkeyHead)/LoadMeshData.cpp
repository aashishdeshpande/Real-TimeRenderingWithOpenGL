#include <windows.h>
#include <gl/GL.h>
#include <GL/glu.h>
#include<vector>
#include "LoadMeshData.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define BUFFER_SIZE			256
#define S_EQUAL				0

#define NR_POINTS_COORDS	3
#define NR_TEXTURE_COORDS	2
#define NR_NORMAL_COORDS    3
#define NR_FACE_TOKENS		3

#define MONKEYHEAD_X_SCALE_FACTOR	1.5f
#define MONKEYHEAD_Y_SCALE_FACTOR	1.5f
#define MONKEYHEAD_Z_SCALE_FACTOR	1.5f

#define MONKEYHEAD_X_TRANSLATE	 0.0f
#define MONKEYHEAD_Y_TRANSLATE	-0.0f
#define MONKEYHEAD_Z_TRANSLATE	-5.0f

#define START_ANGLE_POS		0.0f
#define END_ANGLE_POS		360.0f
#define MONKEYHEAD_ANGLE_INCREMENT	0.5f



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
bool gbLighting = false;


GLfloat g_rotate;
GLuint Texture_monkeyhead = 0;

std::vector<std::vector<float>> g_vertices; //Vector of vector to hold vertex data

std::vector<std::vector<float>> g_texture;  //Vector of vector to hold texture data

std::vector<std::vector<float>> g_noramls; //Vector of vector to hold normal data

										   //Vector of vector of int to hold index data in g_vertices

std::vector<std::vector<int>> g_face_tri, g_face_texture, g_face_normals;

//Handle to a mesh file
FILE *g_fp_meshfile = NULL;

//Handle to a log file
FILE *g_fp_logfile = NULL;

//Hold line in a file
char line[BUFFER_SIZE];



GLfloat light0_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light0_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light0_specular[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light0_position[] = { 0.5f,1.0f,0.7f,0.0f };


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//function prototype
	void initialize(void);
	void uninitialize(void);
	void display(void);
	void update();

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
		TEXT("Mesh Loading: Monkey Head"),
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

void LoadMeshData(void)
{
	void uninitialize(void);

	g_fp_meshfile = fopen("MonkeyHead.OBJ", "r");
	if (!g_fp_meshfile)
		uninitialize();

	char *sep_space = " ";

	char *sep_fslash = "/";

	char *first_token = NULL;

	char *token = NULL;

	char *face_tokens[NR_FACE_TOKENS];

	int nr_tokens;

	char *token_vertex_index = NULL;

	char *token_texture_index = NULL;

	char *token_normal_index = NULL;

	while (fgets(line, BUFFER_SIZE, g_fp_meshfile) != NULL)
	{
		first_token = strtok(line, sep_space);

		if (strcmp(first_token, "v") == S_EQUAL)
		{
			std::vector<float>vec_point_coord(NR_POINTS_COORDS);
			for (int i = 0; i != NR_POINTS_COORDS; i++)
			{
				vec_point_coord[i] = atof(strtok(NULL, sep_space));
			}
			g_vertices.push_back(vec_point_coord);
		}

		else if (strcmp(first_token, "vt") == S_EQUAL)
		{
			std::vector<float>vec_texture_coord(NR_TEXTURE_COORDS);
			for (int i = 0; i != NR_TEXTURE_COORDS; i++)
			{
				vec_texture_coord[i] = atof(strtok(NULL, sep_space));
			}
			g_texture.push_back(vec_texture_coord);
		}

		else if (strcmp(first_token, "vn") == S_EQUAL)
		{
			std::vector<float>vec_normal_coord(NR_NORMAL_COORDS);

			for (int i = 0; i != NR_TEXTURE_COORDS; i++)
			{
				vec_normal_coord[i] = atof(strtok(NULL, sep_space));
			}
			g_noramls.push_back(vec_normal_coord);
		}

		else if (strcmp(first_token, "f") == S_EQUAL)
		{
			std::vector<int>triangle_vertex_indices(3), texture_vertex_indices(3), normal_vertex_indices(3);
			memset((void *)face_tokens, 0, NR_FACE_TOKENS);

			nr_tokens = 0;
			while (token = strtok(NULL, sep_space))
			{
				if (strlen(token) < 3)
					break;
				face_tokens[nr_tokens] = token;
				nr_tokens++;
			}
			for (int i = 0; i != NR_FACE_TOKENS; i++)
			{
				token_vertex_index = strtok(face_tokens[i], sep_fslash);
				token_texture_index = strtok(NULL, sep_fslash);
				token_normal_index = strtok(NULL, sep_fslash);
				triangle_vertex_indices[i] = atoi(token_vertex_index);
				texture_vertex_indices[i] = atoi(token_texture_index);
				normal_vertex_indices[i] = atoi(token_normal_index);
			}

			g_face_tri.push_back(triangle_vertex_indices);
			g_face_texture.push_back(texture_vertex_indices);
			g_face_normals.push_back(normal_vertex_indices);
		}

		memset((void*)line, (int)'\0', BUFFER_SIZE);
	}

	fclose(g_fp_meshfile);
	g_fp_meshfile = NULL;

	fprintf(g_fp_logfile, "g_vertices:%llu g_texture:%llu g_noramls:%llu g_face_tri:%llu\n", g_vertices.size(), g_texture.size(), g_noramls.size(), g_face_tri.size());
}

void update(void)
{
	g_rotate = g_rotate + MONKEYHEAD_ANGLE_INCREMENT;

	if (g_rotate >= END_ANGLE_POS)
	{
		g_rotate = START_ANGLE_POS;
	}
}

void display(void)
{
	
	//fprintf(g_fp_logfile, "In Display");
	void uninitialize(void);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(MONKEYHEAD_X_TRANSLATE, MONKEYHEAD_Y_TRANSLATE, MONKEYHEAD_Z_TRANSLATE);
	glRotatef(g_rotate, 0.0f, 1.0f, 0.0f);
	glScalef(MONKEYHEAD_X_SCALE_FACTOR, MONKEYHEAD_Y_SCALE_FACTOR, MONKEYHEAD_Z_SCALE_FACTOR);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	for (int i = 0; i != g_face_tri.size(); i++)
	{
		glBegin(GL_TRIANGLES);

		for (int j = 0; j != g_face_tri[i].size(); j++)
		{
			int vi = g_face_tri[i][j] - 1;
			int vn = g_face_normals[i][j] - 1;
			int vtex = g_face_texture[i][j] - 1;

			glNormal3f(g_noramls[vn][0], g_noramls[vn][1], g_noramls[vn][2]);
			glTexCoord2f(g_texture[vtex][0], g_texture[vtex][1]);
			glVertex3f(g_vertices[vi][0], g_vertices[vi][1], g_vertices[vi][2]);
		}
		glEnd();
	//	fprintf(g_fp_logfile,"sr no %d" "value of i %d\n", "value of j %d", "value of k %d", "value of l %d",count++, i, j, k, l);
	}
	SwapBuffers(ghdc);
	
}


void initialize(void)
{
	//function prototypes
	void resize(int, int);
	void uninitialize(void);
	void LoadMeshData(void);
	int LoadGLTextures(GLuint *, TCHAR[]);

	g_fp_logfile = fopen("MONKEYHEADLOADER.LOG", "w");
	if (!g_fp_logfile)
		uninitialize();


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

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	LoadGLTextures(&Texture_monkeyhead, MAKEINTRESOURCE(IDBitmapMonkeyHead));

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	

	LoadMeshData();

	resize(WIN_WIDTH, WIN_HEIGHT);
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


int LoadGLTextures(GLuint *texture, TCHAR imageResourceId[])
{
	HBITMAP hBitmap;
	BITMAP bmp;
	int iStatus = FALSE;
	glGenTextures(1, texture);
	hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), imageResourceId, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		iStatus = TRUE;
		GetObject(hBitmap, sizeof(bmp), &bmp);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, bmp.bmWidth, bmp.bmHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		DeleteObject(hBitmap);
	}

	return(iStatus);
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

	fclose(g_fp_logfile);
	g_fp_logfile = NULL;

	DestroyWindow(ghwnd);
	ghwnd = NULL;

	
}



