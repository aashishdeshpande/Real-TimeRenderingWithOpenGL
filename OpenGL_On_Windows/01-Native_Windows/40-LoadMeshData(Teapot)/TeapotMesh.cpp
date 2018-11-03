#include<Windows.h>

#include<gl/GL.h>
#include<gl/GLU.h>

#include<stdio.h>
#include<stdlib.h>

#include<vector>
#include "MyMath3D.h"
#include "TeapotMesh.h"

#define TRUE			1
#define FALSE			0

#define BUFFER_SIZE		256
#define S_EQUAL			0

#define NR_TEXTURE_COORDS	3
#define NR_POINT_COORDS		3
#define MIN_NR_FACE_TOKENS	3
#define MAX_NR_FACE_TOKENS	4

#define WIN_INIT_X			100
#define WIN_INIT_Y			100
#define WIN_WIDTH			800
#define WIN_HEIGHT			600

#define VK_F				0x46
#define VK_f				0x60

#define FOY_ANGLE			45
#define ZNEAR				0.1
#define ZFAR				200.0

#define VIEWPORT_BOTTOMLEFT_X	0
#define VIEWPORT_BOTTOMLEFT_Y	0

#define TEAPOT_X_TRANSLATE		0.0f
#define TEAPOT_Y_TRANSLATE		-25.0f
#define TEAPOT_Z_TRANSLATE		-125.0f

#define TEAPOT_X_SCALE_FACTOR	0.75f
#define TEAPOT_Y_SCALE_FACTOR	0.75f
#define TEAPOT_Z_SCALE_FACTOR   0.75f

#define START_ANGLE_POS			0.0f
#define END_ANGLE_POS			360.0f
#define TEAPOT_ANGLE_INCREMENT	1.0f

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_hwnd = NULL;
HDC g_hdc = NULL;
HGLRC g_hrc = NULL;

DWORD g_dwStyle = NULL;
WINDOWPLACEMENT g_wpPrev;

bool g_bActiveWindow = false;
bool g_bEscapeKeyIsPressed = false;
bool g_bFullSccreen = false;
bool gbLighting = false;


GLfloat g_rotate;
GLuint Textue_teapot = 0;

std::vector<std::vector<float>> g_vertices;

std::vector<std::vector<float>> g_normals;

std::vector<std::vector<float>> g_texture;

std::vector<std::vector<int>> g_face_tri, g_face_texture, g_face_normals;

FILE *g_fp_meshfile = NULL;

FILE *g_fp_logfile = NULL;



char line[BUFFER_SIZE];

GLfloat light0_ambient[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light0_diffuse[] = { 1.0f,1.0f,1.0f,1.0f };
GLfloat light0_specular[] = { 0.0f,0.0f,0.0f,1.0f };
GLfloat light0_position[] = { 0.5f,1.0f,1.0f,0.0f };

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	
	void initialize(void);
	void update(void);
	void display(void);
	void uninitialize(void);


	static TCHAR szAppName[] = TEXT("Mesh Loading version 2");

	HWND hWnd = NULL;
	HBRUSH hBrush = NULL;
	HCURSOR hCursor = NULL;
	HICON hIcon = NULL;
	HICON hIconSm = NULL;

	bool bDone = false;

	WNDCLASSEX wndEx;
	MSG msg;

	ZeroMemory((void *)&wndEx, sizeof(WNDCLASSEX));
	ZeroMemory((void *)&msg, sizeof(MSG));

	hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);

	hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);

	hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);

	hIconSm = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);

	wndEx.cbClsExtra = 0;
	wndEx.cbWndExtra = 0;
	wndEx.cbSize = sizeof(WNDCLASSEX);
	wndEx.hbrBackground = hBrush;
	wndEx.hCursor = hCursor;
	wndEx.hIcon = hIcon;
	wndEx.hIconSm = hIconSm;
	wndEx.hInstance = hInstance;
	wndEx.lpfnWndProc = WndProc;
	wndEx.lpszClassName = szAppName;
	wndEx.lpszMenuName = NULL;
	wndEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	RegisterClassEx(&wndEx);

	hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		szAppName,
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		WIN_INIT_X,
		WIN_INIT_Y,
		WIN_WIDTH,
		WIN_HEIGHT,
		(HWND)NULL,
		(HMENU)NULL,
		hInstance,
		(LPVOID)NULL);


	g_hwnd = hWnd;

	initialize();


	ShowWindow(hWnd, SW_SHOWNORMAL);
	
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);

	while (bDone == false)
	{
		if (PeekMessage(&msg, (HWND)NULL, 0, 0, PM_REMOVE))
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
			if (g_bActiveWindow == true)
			{
				if (g_bEscapeKeyIsPressed)
					bDone = true;
				else
				{
					update();
					display();
				}
			}
		}
	}
	uninitialize();
	return((int)msg.wParam);
	
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMSg, WPARAM wParam, LPARAM lParam)
{
	
	void resize(int,int);
	void ToggleFullScreen(void);
	void uninitialize(void);

	switch (uMSg)
	{

	case WM_ACTIVATE:
		if (HIWORD(wParam) == 0)
			g_bActiveWindow = true;
		else
			g_bActiveWindow = false;

//	case WM_ERASEBKGND:
	//	return(0);

	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{

			case VK_ESCAPE:
				if (g_bEscapeKeyIsPressed == false)
					g_bEscapeKeyIsPressed = true;
				break;

			case VK_F:
			case VK_f:
				if (g_bFullSccreen == false)
				{
					ToggleFullScreen();
					g_bFullSccreen = true;
				}
				else
				{
					ToggleFullScreen();
					g_bFullSccreen = false;
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

	case WM_CLOSE:
		uninitialize();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return(DefWindowProc(hWnd, uMSg, wParam, lParam));
	
}

void ToggleFullScreen(void)
{
	
	MONITORINFO mi;
	if (g_bFullSccreen == false)
	{
		g_dwStyle = GetWindowLong(g_hwnd, GWL_STYLE);

		if (g_dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(g_hwnd, &g_wpPrev) && GetMonitorInfo(MonitorFromWindow(g_hwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(g_hwnd, GWL_STYLE, g_dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(g_hwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
			ShowCursor(FALSE);
		}
	}
	else
	{
		SetWindowLong(g_hwnd, GWL_STYLE, g_dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(g_hwnd, &g_wpPrev);
		SetWindowPos(g_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}
	
}

void initialize(void)
{
	int LoadGLTextures(GLuint *, TCHAR[]);
	void resize(int, int);
	void uninitialize(void);
	void LoadMeshData(void);
	g_fp_logfile = fopen("MESHLAODERVERSIONG.LOG", "w");
	if (!g_fp_logfile)
	{
		uninitialize();
	//	DestroyWindow(g_hwnd);
	}
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex = -1;
	ZeroMemory((void*)&pfd, sizeof(PIXELFORMATDESCRIPTOR));
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

	g_hdc = GetDC(g_hwnd);
	iPixelFormatIndex = ChoosePixelFormat(g_hdc, &pfd);
	if (iPixelFormatIndex == 0)
	{
		ReleaseDC(g_hwnd, g_hdc);
		g_hdc = NULL;
	}

	if (SetPixelFormat(g_hdc, iPixelFormatIndex, &pfd) == false)
	{
		ReleaseDC(g_hwnd, g_hdc);
		g_hdc = NULL;
	}

	g_hrc = wglCreateContext(g_hdc);
	if (g_hrc == NULL)
	{
		ReleaseDC(g_hwnd, g_hdc);
		g_hdc = NULL;
	}

	if (wglMakeCurrent(g_hdc, g_hrc) == false)
	{
		wglDeleteContext(g_hrc);
		g_hrc = NULL;
		ReleaseDC(g_hwnd, g_hdc);
		g_hdc = NULL;
	}

	

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	LoadGLTextures(&Textue_teapot, MAKEINTRESOURCE(IDBitmapTeapot));

	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);


	LoadMeshData();
	resize(WIN_WIDTH, WIN_HEIGHT);
	

}

void resize(int width, int height)
{

	if (height == 0)
		height = 1;
	glViewport(VIEWPORT_BOTTOMLEFT_X, VIEWPORT_BOTTOMLEFT_Y, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOY_ANGLE, (GLfloat)width / (GLfloat)height, ZNEAR, ZFAR);
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

void LoadMeshData(void)
{
	
	void uninitialize(void);

	g_fp_meshfile = fopen("teapot.obj", "r");
	if (!g_fp_meshfile)
		uninitialize();

	char *sep_space = " ";
	char *sep_fslash = "/";
	char *first_token = NULL;
	char *token = NULL;
	char *face_tokens[MAX_NR_FACE_TOKENS];
	int nr_tokens;
	char *token_vertex_index = NULL;
	char *token_texture_index = NULL;

	while ((fgets(line, BUFFER_SIZE, g_fp_meshfile) != NULL) && !feof(g_fp_meshfile))
	{
		first_token = strtok(line, sep_space);

		if (strcmp(first_token, "v") == S_EQUAL)
		{
			std::vector<float>vec_point_coord(NR_POINT_COORDS);
			for (int i = 0; i != NR_POINT_COORDS; i++)
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
		else if (strcmp(first_token, "f") == S_EQUAL)
		{
			std::vector<int>triangle_vertex_indices(3), texture_vertex_indices(3);
			memset((void*)face_tokens, 0, MAX_NR_FACE_TOKENS);
			nr_tokens = 0;
			while (token = strtok(NULL, sep_space))
			{
				if (strlen(token) < 3)
					break;
				face_tokens[nr_tokens] = token;
				nr_tokens++;
			}

			for (int i = 0; i != MIN_NR_FACE_TOKENS; ++i)
			{
				token_vertex_index = strtok(face_tokens[i], sep_fslash);
				triangle_vertex_indices[i] = atoi(token_vertex_index);
				token_texture_index = strtok(NULL, sep_fslash);
				texture_vertex_indices[i] = atoi(token_texture_index);
			}
			g_face_tri.push_back(triangle_vertex_indices);
			g_face_texture.push_back(texture_vertex_indices);

			
		/*	if (nr_tokens == MAX_NR_FACE_TOKENS)
			{
				triangle_vertex_indices[1] = triangle_vertex_indices[2];

				token_vertex_index = strtok(face_tokens[MAX_NR_FACE_TOKENS - 1], sep_fslash);
				token_texture_index = strtok(NULL, sep_fslash);
				
				triangle_vertex_indices[2] = atoi(token_vertex_index);
				g_face_tri.push_back(triangle_vertex_indices);

				texture_vertex_indices[1] = texture_vertex_indices[2];
				texture_vertex_indices[2] = atoi(token_texture_index);
				g_face_texture.push_back(texture_vertex_indices);
				
			}*/


			if (nr_tokens == MAX_NR_FACE_TOKENS)
			{
				triangle_vertex_indices[1] = triangle_vertex_indices[2];
				token_vertex_index = strtok(face_tokens[MAX_NR_FACE_TOKENS - 1], sep_fslash);

				triangle_vertex_indices[2] = atoi(token_vertex_index);

				texture_vertex_indices[1] = texture_vertex_indices[2];
				token_texture_index = strtok(NULL, sep_fslash);

				texture_vertex_indices[2] = atoi(token_texture_index);

				g_face_tri.push_back(triangle_vertex_indices);
				
				g_face_texture.push_back(texture_vertex_indices);

			}

			//g_face_tri.push_back(triangle_vertex_indices);
			//g_face_texture.push_back(texture_vertex_indices);

		}
		memset((void*)line, (int)'\0', BUFFER_SIZE);
	}
	
	fclose(g_fp_meshfile);
	g_fp_meshfile = NULL;

	fprintf_s(g_fp_logfile, "g_vertices:%llu g_texture:%llu g_face_tri:%llu\n", g_vertices.size(), g_texture.size(), g_face_tri.size());


	
}

void update(void)
{
	
	g_rotate = g_rotate + TEAPOT_ANGLE_INCREMENT;
	if (g_rotate >= END_ANGLE_POS)
		g_rotate = START_ANGLE_POS;
	
}

void display(void)
{
	//std::vector<float>result(3);
	
	//float result[3];
	//float point1[3];
	//float point2[3];
	//float point3[3];

	//void calculateNormal(result, point1, point2, point3);
//	std::vector<float> calculateNormal(GLfloat,GLfloat,GLfloat);
	
		/*for (int j = 0; j != g_face_tri[i].size(); j++)
		{
			int vi = g_face_tri[i][j] - 1;
			result = calculateNormal(g_vertices[vi], g_vertices[vi + 1], g_vertices[vi + 2]);
			glNormal3f(result[0], result[1], result[2]);
		}*/
		
		//glNormal3fv(vNormal);

		//calculateNormal(result, point1[nix], point2[niy], point3[niz]);
		//glNormal3f(result[0], result[1], result[2]);

	//	for (int j = 0; j != g_face_tri[i].size(); j++)
		//{
		//int vi = g_face_tri[i][j] - 1;
		//result = calculateNormal(g_vertices[nix], g_vertices[niy], g_vertices[niz]);
		//glNormal3f(result[0], result[1], result[2]);
		//}
		
	

	//fprintf(g_fp_logfile, "In Display");
	void uninitialize(void);
	M3DVector3f vNormal;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(TEAPOT_X_TRANSLATE, TEAPOT_Y_TRANSLATE, TEAPOT_Z_TRANSLATE);
	glRotatef(g_rotate, 0.0f, 1.0f, 0.0f);
	
	glScalef(TEAPOT_X_SCALE_FACTOR, TEAPOT_Y_SCALE_FACTOR, TEAPOT_Z_SCALE_FACTOR);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i != g_face_tri.size(); ++i)
	{

		int nix = g_face_tri[i][0] - 1;
		int niy = g_face_tri[i][1] - 1;
		int niz = g_face_tri[i][2] - 1;

		M3DVector3f vPoints[3] = { { g_vertices[nix][0], g_vertices[nix][1], g_vertices[nix][2] },
		{ g_vertices[niy][0], g_vertices[niy][1], g_vertices[niy][2] },
		{ g_vertices[niz][0], g_vertices[niz][1], g_vertices[niz][2] } };

		m3dFindNormal(vNormal, vPoints[0], vPoints[1], vPoints[2]);

		glBegin(GL_TRIANGLES);

		for (int j = 0; j != g_face_tri[i].size(); j++)
		{
			int vi = g_face_tri[i][j] - 1;
			int vtex = g_face_texture[i][j] - 1;
			glNormal3fv(vNormal);
			glTexCoord3f(g_texture[vtex][0], g_texture[vtex][1], g_texture[vtex][2]);
			glVertex3f(g_vertices[vi][0], g_vertices[vi][1], g_vertices[vi][2]);
		}
		glEnd();
		//	fprintf(g_fp_logfile,"sr no %d" "value of i %d\n", "value of j %d", "value of k %d", "value of l %d",count++, i, j, k, l);
	}
	SwapBuffers(g_hdc);

}

void uninitialize(void)
{
	
	if (g_bFullSccreen == true)
	{
		g_dwStyle = GetWindowLong(g_hwnd, GWL_STYLE);
		SetWindowLong(g_hwnd, GWL_STYLE, g_dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(g_hwnd, &g_wpPrev);
		SetWindowPos(g_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
	}

	wglMakeCurrent((HDC)NULL, (HGLRC)NULL);
	wglDeleteContext(g_hrc);
	g_hrc = (HGLRC)NULL;

	ReleaseDC(g_hwnd, g_hdc);
	g_hdc = (HDC)NULL;

	fclose(g_fp_logfile);
//	g_fp_logfile = NULL;

	DestroyWindow(g_hwnd);
	g_hwnd = NULL;
	
}

void m3dFindNormal(M3DVector3f result, const M3DVector3f point1, const M3DVector3f point2, const M3DVector3f point3)
{
	M3DVector3f v1, v2;	
	v1[0] = point1[0] - point2[0];
	v1[1] = point1[1] - point2[1];
	v1[2] = point1[2] - point2[2];

	v2[0] = point2[0] - point3[0];
	v2[1] = point2[1] - point3[1];
	v2[2] = point2[2] - point3[2];
	m3dCrossProduct(result, v1, v2);
}









