

#include "animwin32.h"
#include <windowsx.h>

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
size_t width=0;
size_t height=0;
pb_rect pixelFrame;

int grectMode = CORNER;
int gellipseMode = CORNER;

// for time keeping
uint64_t startcount=0;	
double clockfrequency=1;

// Mouse
int mouseX = 0;
int mouseY = 0;

// color setting
uint32_t bgColor=0;
pb_rgba *bgImage = nullptr;
uint32_t strokeColor = RGBA(0, 0, 0, 255);
uint32_t fillColor = RGBA(255, 255, 255, 255);



// size of window
void size(const size_t lwidth, const size_t lheight)
{
	width = lwidth;
	height = lheight;

	pixelFrame.x = 0;
	pixelFrame.y = 0;
	pixelFrame.width = width;
	pixelFrame.height = height;
}


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

// color setting
void background(const uint32_t value)
{
	bgColor = value;
}

void noFill()
{
	fillColor = 0;
}

void noStroke()
{
	strokeColor = 0;
}

void stroke(const uint8_t value)
{
	strokeColor = RGBA(value,value,value,255);
}

void stroke(const uint32_t value)
{
	strokeColor = value;
}

void fill(const uint8_t value)
{
	fillColor = RGBA(value, value, value, 255);
}

void fill(const uint32_t value)
{
	fillColor = value;
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

//
// Line Clipping in preparation for line drawing
//
typedef int OutCode;

static const int INSIDE = 0; // 0000
static const int LEFT = 1;   // 0001
static const int RIGHT = 2;  // 0010
static const int BOTTOM = 4; // 0100
static const int TOP = 8;    // 1000

// Compute the bit code for a point (x, y) using the clip rectangle
// bounded diagonally by (xmin, ymin), and (xmax, ymax)

OutCode ComputeOutCode(const pb_rect &rct, const int x, const int y)
{
	OutCode code;
	double xmin = rct.x;
	double xmax = rct.x + rct.width-1;
	double ymin = rct.y;
	double ymax = rct.y + rct.height-1;

	code = INSIDE;          // initialised as being inside of clip window

	if (x < xmin)           // to the left of clip window
		code |= LEFT;
	else if (x > xmax)      // to the right of clip window
		code |= RIGHT;
	if (y < ymin)           // below the clip window
		code |= BOTTOM;
	else if (y > ymax)      // above the clip window
		code |= TOP;

	return code;
}

// Cohen–Sutherland clipping algorithm clips a line from
// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
// diagonal from (xmin, ymin) to (xmax, ymax).
bool ClipLine(const pb_rect &bounds, int &x0, int &y0, int &x1, int &y1)
{
	// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
	OutCode outcode0 = ComputeOutCode(bounds, x0, y0);
	OutCode outcode1 = ComputeOutCode(bounds, x1, y1);
	bool accept = false;
	double xmin = bounds.x;
	double xmax = bounds.x + bounds.width - 1;
	double ymin = bounds.y;
	double ymax = bounds.y + bounds.height - 1;


	while (true) {
		if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
			accept = true;
			break;
		}
		else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
			break;
		}
		else {
			// failed both tests, so calculate the line segment to clip
			// from an outside point to an intersection with clip edge
			double x, y;

			// At least one endpoint is outside the clip rectangle; pick it.
			OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

			// Now find the intersection point;
			// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
			if (outcodeOut & TOP) {           // point is above the clip rectangle
				x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
				y = ymax;
			}
			else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
				x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
				y = ymin;
			}
			else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
				y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
				x = xmax;
			}
			else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
				y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
				x = xmin;
			}

			// Now we move outside point to intersection point to clip
			// and get ready for next pass.
			if (outcodeOut == outcode0) {
				x0 = x;
				y0 = y;
				outcode0 = ComputeOutCode(bounds, x0, y0);
			}
			else {
				x1 = x;
				y1 = y;
				outcode1 = ComputeOutCode(bounds, x1, y1);
			}
		}
	}

	return accept;
}



void line(const int x1, const int y1, const int x2, const int y2)
{
	int xx1 = x1;
	int yy1 = y1;
	int xx2 = x2;
	int yy2 = y2;

	if (!ClipLine(pixelFrame, xx1, yy1, xx2, yy2))
	{
		return;
	}

	// TODO - need to intersect line with pixelFrame
	if ((xx1 < 0) || (yy1 < 0))
		return;

	if ((xx2 > xx1 + width) || (yy2 > y1 + height))
		return;

	raster_rgba_line(gpb, xx1, yy1, xx2, yy2, strokeColor);
}

void lineloop(const size_t nPts, const int *pts)
{
	if (nPts < 2)
		return;

	int firstx = pts[0];
	int firsty = pts[1];
	int lastx = firstx;
	int lasty = firsty;

	for (int idx = 1; idx < nPts; idx++)
	{
		int nextx = pts[idx * 2];
		int nexty = pts[(idx * 2) + 1];
		line(lastx, lasty, nextx, nexty);
		lastx = nextx;
		lasty = nexty;
	}

	// draw last point to beginning
	line(lastx, lasty, firstx, firsty);
}

void point(const int x, const int y)
{
	if (!pb_rect_contains_point(pixelFrame, x, y))
		return;

	pb_rgba_cover_pixel(gpb, x, y, strokeColor);
}

void rectMode(const int mode)
{
	grectMode = mode;
}

void rect(const int a, const int b, const int c, const int d)
{
	int x1=0, y1=0;
	int rwidth=0, rheight=0;

	switch (grectMode) {
		case CORNER:
			x1 = a;
			y1 = b;
			rwidth = c;
			rheight = d;
		break;
		
		case CORNERS:
			x1 = a;
			y1 = b;
			rwidth = c - a + 1;
			rheight = d - b + 1;
		break;

		case CENTER:
			x1 = a - c / 2;
			y1 = b - d / 2;
			rwidth = c;
			rheight = d;
		break;

		case RADIUS:
			x1 = a - c;
			y1 = b - d;
			rwidth = c*2;
			rheight = d*2;
		break;
	}

	if (!pb_rect_contains_rect(pixelFrame, { x1, y1, rwidth, rheight }))
		return;



	if (fillColor != 0) {
		//raster_rgba_rect_fill(gpb, x1, y1, rwidth, rheight, fillColor);
		raster_rgba_rect_fill_blend(gpb, x1, y1, rwidth, rheight, fillColor);
	}

	// if the strokeColor != 0 then 
	// frame the rectangle in the strokeColor
	if (strokeColor != 0) {
		int pts[] = {
			x1, y1,
			x1, y1+rheight-1,
			x1+rwidth-1, y1+rheight-1,
			x1+rwidth-1, y1
		};

		lineloop(4, pts);
	}
}

void triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3)
{
	if (fillColor != 0) {
		raster_rgba_triangle_fill(gpb, x1, y1, x2, y2, x3, y3, fillColor);
	}

	if (strokeColor != 0) {
		int pts[] = {
			x1, y1,
			x2, y2,
			x3, y3
		};
		lineloop(3, pts);
	}
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
	wcex.hIconSm = NULL;			//LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPLICATION));

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
		case WM_MOUSEMOVE:
			mouseX = GET_X_LPARAM(lParam);
			mouseY = GET_Y_LPARAM(lParam);
		break;
/*
	case WM_LBUTTONDOWN:
		dragmode = true;
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