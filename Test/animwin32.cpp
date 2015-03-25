

#include "animwin32.h"
#include <math.h>

#define MAX_LOADSTRING 100

// Forward declarations of functions included in this code module:
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[] = "Window";					// The title bar text
TCHAR szWindowClass[] = "animwin";			// the main window class name
bool continueRunning = true;
HBITMAP gbmHandle;
HDC ghMemDC;
pb_rgba *gpb;




// Globals for the environment
size_t width;
size_t height;

void size(const size_t lwidth, const size_t lheight)
{
	width = lwidth;
	height = lheight;
}

uint64_t startcount;	// for time keeping
double clockfrequency;

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

// color setting
uint32_t bgColor;
pb_rgba *bgImage = nullptr;
uint32_t strokeColor = RGBA(0, 0, 0, 255);

void background(const uint32_t value)
{
	bgColor = value;
}

void stroke(const uint8_t value)
{
	strokeColor = RGBA(value,value,value,255);
}

void stroke(const uint32_t value)
{
	strokeColor = value;
}

// 2D primitives

void ellipse(const int a, const int b, const int c, const int d)
{
	size_t xradius = c / 2;
	size_t yradius = d / 2;

	uint32_t cx = a + xradius;
	uint32_t cy = b + yradius;
	raster_rgba_ellipse_stroke(gpb, cx, cy, xradius, yradius, strokeColor);
}

void line(const int x1, const int y1, const int x2, const int y2)
{
	raster_rgba_line(gpb, x1, y1, x2, y2, strokeColor);
}

void point(const int x, const int y)
{
	pb_rgba_cover_pixel(gpb, x, y, strokeColor);
}

// Internal to animwin32

void InitializeInstance()
{
	width = 640;
	height = 480;
	bgColor = pDarkGray;

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
	BITMAPINFO info;
	memset(&info, 0, sizeof(BITMAPINFO));

	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = -(int)height;	// top-down DIB Section
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;

	//int bytesPerRow = width * 4;
	//info.bmiHeader.biSizeImage = bytesPerRow * abs((int)height);
	//info.bmiHeader.biClrImportant = 0;
	//info.bmiHeader.biClrUsed = 0;

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

pb_rgba * CreatePixelBuffer(const size_t width, const int height, void *data)
{
	// Save the pointer into the pb_rgba pixel buffer
	pb_rgba * pb = new pb_rgba();
	pb->data = (uint8_t *)data;
	pb->owndata = false;
	pb->pixelpitch = width;
	pb->frame.height = abs(height);
	pb->frame.width = width;
	pb->frame.x = 0;
	pb->frame.y = 0;

	return pb;
}



HWND CreateWindowHandle(int width, int height)
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
	wcex.hIconSm = NULL;			//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	ATOM winclassatom = ::RegisterClassExA(&wcex);



	DWORD dwExStyle = 0;
	DWORD winstyle = WS_OVERLAPPEDWINDOW;
	int x = CW_USEDEFAULT;
	int y = CW_USEDEFAULT;
	HWND hWndParent = NULL;
	HMENU hMenu = NULL;

	if (!winclassatom) {
		return NULL;
	}

	HWND hWnd = ::CreateWindowExA(
		0,
		szWindowClass,
		szTitle,
		winstyle,
		x, y, width, height,
		hWndParent,
		hMenu,
		hInst,
		NULL);

	return hWnd;
}

void OnPaint(HDC hdc, PAINTSTRUCT &ps)
{
	// TODO: Add any drawing code here...
	// bitblt bmhandle to client area
	if ((NULL != ghMemDC) && (NULL != gpb)) {


		::BitBlt(hdc, 0, 0, gpb->frame.width, gpb->frame.height, ghMemDC, 0, 0, SRCCOPY);
	}
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
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	printf("WndProc, MSG: %d\n", message);

	switch (message)
	{
		/*
	case WM_LBUTTONDOWN:
		dragmode = true;
		break;

	case WM_MOUSEMOVE:
		if (dragmode)
		{
			boxx = GET_X_LPARAM(lParam);
			boxy = GET_Y_LPARAM(lParam);
		}
		break;

	case WM_LBUTTONUP:
		dragmode = false;
		InvalidateRect(hWnd, 0, TRUE);
		break;
		*/
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
/*
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
*/
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		OnPaint(hdc, ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		printf("WM_DESTROY\n");

		PostQuitMessage(0);
		continueRunning = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void eventLoop(HWND hWnd)
{
	MSG msg;

	while (continueRunning)
	{
		// Drain messages out of queue
		while (::PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE) != 0) {
			printf("MSG: %d\n", msg.message);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// draw the background color
		raster_rgba_rect_fill(gpb, 0, 0, width - 1, height - 1, bgColor);

		// Allow the client to do some drawing if desired
		step(gpb);

		// Assume the 'step()' did something which requires the 
		// screen to be redrawn, so, invalidate the entire client area
		InvalidateRect(hWnd, 0, TRUE);
	}
}



int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	InitializeInstance();
	setup();

	HWND hWnd = CreateWindowHandle(width, height);

	// open the window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Create offscreen bitmap
	void *data;
	ghMemDC = CreateOffscreenDC(hWnd, width, height, &data);
	gpb = CreatePixelBuffer(width, height, data);

	// Start running the event loop
	eventLoop(hWnd);

	return 0;
}