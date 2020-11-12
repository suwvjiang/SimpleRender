// SimpleRender.cpp : This file contains the 'main' function. Program execution begins and ends there.
//右手定则坐标系
//相机与世界同一坐标系
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include "stdfx.h"
#include "RenderDevice.h"

//#define SCREEN_WIDTH 400
//#define SCREEN_HEIGHT 400

static const int windowWidth = 600;
static const int windowHeight = 600;
const char* windowTitle = "Simple Render";

bool inited = false;
std::shared_ptr<RenderDevice> device;

BITMAPINFO* mBitmapInfo = NULL;
HDC mScreenHDC;
HDC mCompatibleDC;
HBITMAP mOldBitmap;
HBITMAP mCompatibleBitmap;
HWND handler = NULL;

static LRESULT OnEvent(HWND, UINT, WPARAM, LPARAM);

//初始化窗口
void initWindow()
{
	WNDCLASS wndClass = { CS_BYTEALIGNCLIENT, (WNDPROC)OnEvent, 0, 0, 0, NULL, NULL, NULL, NULL, TEXT(windowTitle) };
	wndClass.hInstance = GetModuleHandle(NULL);

	if (!RegisterClass(&wndClass))
		return;

	handler = CreateWindow(TEXT(windowTitle), TEXT(windowTitle), WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0, 0, 0, 0, NULL, NULL, wndClass.hInstance, NULL);
	if (handler == NULL)
		return;

	mBitmapInfo = new BITMAPINFO();
	ZeroMemory(mBitmapInfo, sizeof(BITMAPINFO));

	mBitmapInfo->bmiHeader.biBitCount = PIX_BITS;
	mBitmapInfo->bmiHeader.biCompression = BI_RGB;
	mBitmapInfo->bmiHeader.biHeight = -windowHeight;
	mBitmapInfo->bmiHeader.biPlanes = 1;
	mBitmapInfo->bmiHeader.biSizeImage = 0;
	mBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	mBitmapInfo->bmiHeader.biWidth = windowWidth;

	mScreenHDC = GetDC((handler));
	mCompatibleDC = CreateCompatibleDC(mScreenHDC);
	mCompatibleBitmap = CreateCompatibleBitmap(mScreenHDC, windowWidth, windowHeight);
	mOldBitmap = (HBITMAP)SelectObject(mCompatibleDC, mCompatibleBitmap);

	SetWindowPos(handler, NULL, 200, 200, windowWidth, windowHeight, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));

	ShowWindow(handler, SW_NORMAL);
	UpdateWindow(handler);
}

void initRender()
{
	device = std::make_shared<RenderDevice>();
	device->initDevice(windowWidth, windowHeight);
}

//
void getInputStatus()
{
	int key;
	if (_kbhit())
	{
		key = _getch();
		std::cout << key;
		
		switch (key)
		{
		default:
			break;
		}
	}
}

void doRender()
{
	device->clear();
	device->update();
	device->drawcall();

	SetDIBits(mScreenHDC, mCompatibleBitmap, 0, windowHeight, device->getFragmentBuffer(), mBitmapInfo, DIB_RGB_COLORS);
	BitBlt(mScreenHDC, -1, -1, windowWidth, windowHeight, mCompatibleDC, 0, 0, SRCCOPY);
}

void showFPS()
{
	static float  fps = 0;
	static int    frameCount = 0;
	static float  currentTime = 0.0f;
	static float  lastTime = 0.0f;

	frameCount++;
	currentTime = timeGetTime() * 0.001f;

	if (currentTime - lastTime > 1.0f)
	{
		fps = (float)frameCount / (currentTime - lastTime);
		lastTime = currentTime;
		frameCount = 0;
	}

	char strBuffer[40];
	sprintf_s(strBuffer, 40, "fps:%0.1f rotatemodel:%s", fps, device->getCameraRotate()?"camera":"model ");
	TextOut(mScreenHDC, 0, 0, strBuffer, 27);
}

void update()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			getInputStatus();
			doRender();
			showFPS();
		}
	}
}

LRESULT OnEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		exit(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			device->sliderCamera(-0.5f);
			break;
		case VK_DOWN:
			device->sliderCamera(0.5f);
			break;
		case VK_LEFT:
			device->scaleModel(0.1f);
			break;
		case VK_RIGHT:
			device->scaleModel(-0.1f);
			break;
		case VK_SPACE:
			device->switchCameraRotate();
			break;
		default:
			break;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int main()
{
	if (!inited)
	{
		inited = true;

		initWindow();
		initRender();
	}
	update();
	return 0;
}

//参考文献：
//https://github.com/zpzsoft/SoftRaster/blob/master/main.cpp
//https://github.com/puppetmasterzj/AlpacaRenderer/blob/master/ApcMain.cpp

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
