// Headers
#include<windows.h>

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD dwStyle;
bool gbFullscreen;
HWND hwnd;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	HWND ghwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	// code
	// initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register above class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szAppName,
		TEXT("Aashish Deshpande"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	int xPos = LOWORD(lParam);
    int yPos = HIWORD(lParam);
	
	switch (iMsg)
	{
	

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("Co-ordinates are xPos=  yPos="), TEXT("LEFT_BUTTON"), MB_OK);

		break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			MessageBox(hwnd, TEXT("ESCAPE key is pressed"), TEXT("ESCAPE_KEY"), MB_OK);
			break;

		case 0X41:
			MessageBox(hwnd, TEXT("A is pressed"), TEXT("KEY_A"), MB_OK);
			break;


		case 0X43:
			MessageBox(hwnd, TEXT("C is pressed"), TEXT("KEY_C"), MB_OK);
			break;

		case 0X51:
			MessageBox(hwnd, TEXT("Q is pressed"), TEXT("KEY_Q"), MB_OK);
			break;


		case 0X54:
			MessageBox(hwnd, TEXT("T is pressed"), TEXT("KEY_T"), MB_OK);
			break;


		

		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

