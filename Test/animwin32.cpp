

#include "animwin32.h"
#include <windowsx.h>	// GET_X_LPARAM

#include <math.h>
#include <stdio.h>

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


static EventObserverHandler gOnIdleHandler = nullptr;

// Keyboard event handlers
static KeyboardHandler gkbdHandler = nullptr;
static KeyboardHandler gkbdOnPressedHandler = nullptr;
static KeyboardHandler gkbdOnReleasedHandler = nullptr;
static KeyboardHandler gkbdOnTypedHandler = nullptr;

// Mouse event handlers
static MouseHandler gmouseHandler = nullptr;
static EventObserverHandler gOnMousePressedHandler = nullptr;
static MouseHandler gmouseOnUpHandler = nullptr;
static MouseHandler gmouseOnWheelHandler = nullptr;
static MouseHandler gmouseOnDraggedHandler = nullptr;
static MouseHandler gmouseOnMovedHandler = nullptr;


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

void setOnIdleHandler(EventObserverHandler handler)
{
	gOnIdleHandler = handler;
}

void setKeyboardHandler(KeyboardHandler handler)
{
	gkbdHandler = handler;
}

void setOnKeyPressedHandler(KeyboardHandler handler)
{
	gkbdOnPressedHandler = handler;
}

void setOnKeyReleasedHandler(KeyboardHandler handler)
{
	gkbdOnReleasedHandler = handler;
}

void setOnKeyTypedHandler(KeyboardHandler handler)
{
	gkbdOnTypedHandler = handler;
}


void setMouseHandler(MouseHandler handler)
{
	gmouseHandler = handler;
}

void setOnMousePressedHandler(EventObserverHandler handler)
{
	gOnMousePressedHandler = handler;
}

void setOnMouseUpHandler(MouseHandler handler)
{
	gmouseOnUpHandler = handler;
}

void setOnMouseWheelHandler(MouseHandler handler)
{
	gmouseOnWheelHandler = handler;
}

void setOnMouseDraggedHandler(MouseHandler handler)
{
	gmouseOnDraggedHandler = handler;
}

void setOnMouseMovedHandler(MouseHandler handler)
{
	gmouseOnMovedHandler = handler;
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

// Keyboard
int keyCode = 0;
int key = 0;
int isKeyPressed = 0;

// Mouse
int mouseX = 0;
int mouseY = 0;
bool isMousePressed = false;
int mouseButton = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

		case WM_CHAR:
			// Processing regular characters, after translation of various keycodes
			key = wParam;

			if (gkbdOnTypedHandler) {
				gkbdOnTypedHandler(hWnd, message, wParam, lParam);
			}

			switch (key){
				case 0x1B:  // ESC
					quit();
				break;
			}
		break;


		case WM_KEYDOWN:
			keyCode = wParam;
			isKeyPressed = 1;

			if (gkbdOnPressedHandler) {
				return gkbdOnPressedHandler(hWnd, message, wParam, lParam);
			}
		break;

		case WM_KEYUP:
			if (gkbdHandler != nullptr)
			{
				return gkbdHandler(hWnd, message, wParam, lParam);
			} else {
				// raw keycodes
				keyCode = wParam;
				isKeyPressed = 0;
			
				if (gkbdOnReleasedHandler) {
					return gkbdOnReleasedHandler(hWnd, message, wParam, lParam);
				}
			}
		break;

		case WM_MOUSEWHEEL:
			if (gmouseHandler != nullptr) {
				return gmouseHandler(hWnd, message, wParam, lParam);
			}

			if (gmouseOnWheelHandler != nullptr) {
				gmouseOnWheelHandler(hWnd, message, wParam, lParam);
			}
		break;


		case WM_MOUSEMOVE:
			if (gmouseHandler != nullptr) {
				return gmouseHandler(hWnd, message, wParam, lParam);
			}

			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);

			if (isMousePressed) {
				if (gmouseOnDraggedHandler != nullptr) {
					gmouseOnDraggedHandler(hWnd, message, wParam, lParam);
				}
			} else if (gmouseOnMovedHandler != nullptr) {
				gmouseOnMovedHandler(hWnd, message, wParam, lParam);
			}
		break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			if (gmouseHandler != nullptr) {
				return gmouseHandler(hWnd, message, wParam, lParam);
			}

			isMousePressed = true;
			mouseButton = wParam;

			if (gOnMousePressedHandler != nullptr) {
				gOnMousePressedHandler();
			}
		break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			if (gmouseHandler != nullptr) {
				return gmouseHandler(hWnd, message, wParam, lParam);
			}

			isMousePressed = false;

			if (gmouseOnUpHandler != nullptr) {
				gmouseOnUpHandler(hWnd, message, wParam, lParam);
			}
		break;
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

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
	draw();
	InvalidateRect(hWnd, 0, TRUE);

	while (continueRunning)
	{
		// Drain messages out of queue
		while (::PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) != 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		// routine to be called during idle times
		// pretty much the same as calling the draw() routine
		// except it's a function pointer, and it won't be subject
		// to framerate timing
		if (gOnIdleHandler) {
			gOnIdleHandler();
		}

		if (gDrawDuringLoop) {
			// This should adhere to a framerate 
			// if one is specified
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
	setup();

	// Start running the event loop
	eventLoop(ghWnd);

	return 0;
}