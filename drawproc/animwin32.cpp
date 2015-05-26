

#include "animwin32.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#define MAX_LOADSTRING 100

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variables:
HINSTANCE hInst;								// current instance

char szTitle[] = "Window";					// The title bar text
char szWindowClass[] = "animwin";			// the main window class name
HWND ghWnd;

// offscreen bitmap
HBITMAP gbmHandle;
HDC ghMemDC; 
void *gPixelData = nullptr;
static int gbmWidth = 0;
static int gbmHeight = 0;
bool continueRunning = true;
bool gDrawDuringLoop = true;


static CallToHandler gSetupRoutine = nullptr;
static CallToHandler gLoopRoutine = nullptr;




// Keyboard event handlers
static KeyboardHandler gkbdHandler = nullptr;

// Mouse event handlers
static MouseHandler gmouseHandler = nullptr;

CallToHandler setSetupRoutine(CallToHandler handler)
{
	CallToHandler oldRoutine = gSetupRoutine;
	gSetupRoutine = handler;
	return oldRoutine;
}

CallToHandler setLoopRoutine(CallToHandler handler)
{
	CallToHandler oldRoutine = gLoopRoutine;
	gLoopRoutine = handler;
	return oldRoutine;
}

void setKeyboardHandler(KeyboardHandler handler)
{
	gkbdHandler = handler;
}


void setMouseHandler(MouseHandler handler)
{
	gmouseHandler = handler;
}

// for time keeping
uint64_t startcount = 0;
double clockfrequency = 1;

// time keeping
void resettime()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&startcount);
}

double seconds()
{
	uint64_t currentCount;
	::QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);

	uint64_t ellapsed = currentCount - startcount;
	double secs = ellapsed * clockfrequency;

	return secs;
}



// Internal to animwin32
void InitializeInstance()
{
	// Setup time
	uint64_t freq;

	::QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	::QueryPerformanceCounter((LARGE_INTEGER*)&startcount);

	clockfrequency = 1.0f / freq;

}

//
// Internal Win32 specificconstruction
//
HDC CreateOffscreenDC(HWND hWnd, const size_t width, const size_t height, void **data)
{
	gbmWidth = width;
	gbmHeight = height;

	BITMAPINFO info;
	memset(&info, 0, sizeof(BITMAPINFO));

	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -(int)height;	// top-down DIB Section
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;


	gbmHandle = ::CreateDIBSection(
		NULL,
		&info,
		DIB_RGB_COLORS,
		data,
		NULL,
		0);

	// Create offscreen memory DC for DIB Section rendering
	HDC hdc = ::CreateCompatibleDC(NULL);
	::SelectObject(hdc, gbmHandle);

	return hdc;
}

void * GetPixelBuffer(const int width, const int height)
{
	void *data;
	ghMemDC = CreateOffscreenDC(ghWnd, width, height, &data);

	return data;
}



void CreateWindowHandle(int lwidth, int lheight)
{
	UINT style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	HMODULE hInst = ::GetModuleHandleA(NULL);

	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(wcex);
	wcex.style = style;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;				//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;		//(HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = NULL;			//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));

	ATOM winclassatom = ::RegisterClassExA(&wcex);



	DWORD dwExStyle = 0;
	DWORD winstyle = WS_OVERLAPPEDWINDOW;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;
	HWND hWndParent = NULL;
	HMENU hMenu = NULL;

	if (!winclassatom) {
		return ;
	}

	RECT clientRECT = { 0, 0, lwidth, lheight };
	BOOL err = AdjustWindowRect(&clientRECT, WS_CAPTION, 0);

	ghWnd = ::CreateWindowExA(
		0,
		szWindowClass,
		szTitle,
		winstyle,
		x, y, clientRECT.right - clientRECT.left, clientRECT.bottom - clientRECT.top,
		hWndParent,
		hMenu,
		hInst,
		NULL);
}


void * SetWindowSize(const int width, const int height)
{
	CreateWindowHandle(width, height);

	// Create offscreen bitmap
	gPixelData = GetPixelBuffer(width, height);	// open the window

	// Display the window on the screen
	ShowWindow(ghWnd, SW_SHOW);
	UpdateWindow(ghWnd);

	return gPixelData;
}



void OnPaint(HDC hdc, PAINTSTRUCT &ps)
{
	// bitblt bmhandle to client area
	if ((NULL != ghMemDC) && (nullptr != gPixelData)) {
		::BitBlt(hdc, 0, 0, gbmWidth, gbmHeight, ghMemDC, 0, 0, SRCCOPY);
	}
}

void quit()
{
	PostQuitMessage(0);
	continueRunning = false;
}





//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
		case WM_CHAR:
		case WM_KEYDOWN:
		case WM_KEYUP:
			if (gkbdHandler != nullptr)
			{
				return gkbdHandler(hWnd, message, wParam, lParam);
			}				
		break;


		case WM_MOUSEWHEEL:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			if (gmouseHandler != nullptr) {
				return gmouseHandler(hWnd, message, wParam, lParam);
			}
		break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			OnPaint(hdc, ps);

			EndPaint(hWnd, &ps);
		break;

		case WM_DESTROY:
			quit();
		break;
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void setDrawInLoop(bool doDraw)
{
	gDrawDuringLoop = doDraw;
}

void forceDraw()
{
	// Assume the 'draw()' did something which requires the 
	// screen to be redrawn, so, invalidate the entire client area
	InvalidateRect(ghWnd, 0, TRUE);
}

void eventLoop(HWND hWnd)
{
	MSG msg;

	// call draw at least once before we start looping
	if (gLoopRoutine != nullptr) {
		gLoopRoutine();
	}
	draw();

	InvalidateRect(hWnd, 0, TRUE);

	while (continueRunning)
	{
		// Drain messages out of queue
		while (::PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}


		if (gDrawDuringLoop) {
			// This should adhere to a framerate 
			// if one is specified
			if (gLoopRoutine != nullptr) {
				gLoopRoutine();
			}
			draw();

			// Assume the 'draw()' did something which requires the 
			// screen to be redrawn, so, invalidate the entire client area
			InvalidateRect(hWnd, 0, TRUE);
		}
	}
}



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	InitializeInstance();

	// Call setup to get the window size
	// any any global variables set
	if (gSetupRoutine != nullptr) {
		gSetupRoutine();
	}
	setup();

	// Start running the event loop
	eventLoop(ghWnd);

	return 0;
}