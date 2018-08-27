// LayeredWindowDemo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "LayeredWindowDemo.h"

#include <sstream>

#define MAX_LOADSTRING 100

#define TIMER_ID 100

#define USE_LAYERED_WINDOW 1

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAYEREDWINDOWDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAYEREDWINDOWDEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAYEREDWINDOWDEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LAYEREDWINDOWDEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   DWORD exStyle = 0;
#if USE_LAYERED_WINDOW
   exStyle = WS_EX_LAYERED;
#endif

   hWnd = CreateWindowEx(exStyle, szWindowClass, szTitle, WS_OVERLAPPED,
      100, 100, 800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
int layered_window_left_ = 0;
int layered_window_top_ = 0;
int layered_window_width_ = 0;
int layered_window_height_ = 0;
int layered_window_caption_height_ = 50;
TCHAR caption_text_[] = TEXT("Layered Window");
TCHAR description_text_[] = TEXT("win7 two display will result the problem : layered window on assistant display, and d3d's window over it, then the d3d's window have messed message.");  
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_ERASEBKGND:
		return 1;

	case WM_CREATE:
		{
			// create set timer
			::SetTimer(hWnd, TIMER_ID, 1000, 0);
		}
		break;
	case WM_SIZE:
		{
			//layered_window_width_ = LOWORD(lParam);
			//layered_window_height_ = HIWORD(lParam);
		}
		break;
	case WM_WINDOWPOSCHANGED:
		{
			WINDOWPOS * window_pos = (WINDOWPOS *)(lParam);
			layered_window_left_ = window_pos->x;
			layered_window_top_ = window_pos->y;
			layered_window_width_ = window_pos->cx;
			layered_window_height_ = window_pos->cy;
		}
		break;
	case WM_NCHITTEST:
		{
			POINT pt; pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
			::ScreenToClient(hWnd, &pt);

			RECT rcClient;
			::GetClientRect(hWnd, &rcClient);
			

			//if (!::IsZoomed(hWnd)) {
			//	RECT rcSizeBox = { 0, 0, 0, 0 };
			//	if (pt.y < rcClient.top + rcSizeBox.top) {
			//		if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
			//		if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
			//		return HTTOP;
			//	}
			//	else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
			//		if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
			//		if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
			//		return HTBOTTOM;
			//	}
			//	if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
			//	if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
			//}
			POINT ptLeftTop = {0, 0};
			{
				RECT rcWindow;
				GetWindowRect(hWnd, &rcWindow);
				ptLeftTop.x = rcWindow.left;
				ptLeftTop.y = rcWindow.top;
				::ScreenToClient(hWnd, &ptLeftTop);
			}
			RECT rcCaption = { 0, ptLeftTop.y, 0, layered_window_caption_height_ };
			if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
				&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
					return HTCAPTION;
			}

		}
		break;
	case WM_TIMER:
		{
			HDC hDesktopDc = GetDC(NULL);
			POINT ptDest;
			ptDest.x = layered_window_left_;
			ptDest.y = layered_window_top_;
			POINT ptSrc;
			ptSrc.x = 0;
			ptSrc.y = 0;
			SIZE sz;
			sz.cx = layered_window_width_;
			sz.cy = layered_window_height_;

			BLENDFUNCTION bf = { AC_SRC_OVER, 0, 200, AC_SRC_ALPHA };

			HDC hStrechMemDC = ::CreateCompatibleDC(GetDC(hWnd));
			BITMAPINFOHEADER bih;
			void *bits;
			bih.biSize = sizeof(bih);
			bih.biWidth = layered_window_width_;
			bih.biHeight = -layered_window_height_;
			bih.biPlanes = 1;
			bih.biBitCount = 32;
			bih.biCompression = BI_RGB;
			bih.biSizeImage = 0;
			bih.biXPelsPerMeter = 0;
			bih.biYPelsPerMeter = 0;
			bih.biClrUsed = 0;
			bih.biClrImportant = 0;
			HBITMAP hStrechBMP = CreateDIBSection(GetDC(hWnd), (BITMAPINFO*)&bih, DIB_RGB_COLORS, &bits, NULL, 0);
			int block_size = 4;
			char * data = (char *)bits;
			{
				char a = 0xFF;
				char r = 0xFF;
				char g = 0x00;
				char b = 0x00;
				static bool switch_clr = true;
				if (!switch_clr) {
					a = 0xFF;
					r = 0x00;
					g = 0xFF;
					b = 0x00;
				}
				switch_clr = !switch_clr;
				for (int y = 0; y < layered_window_height_; y++)
				{
					for (int x = 0; x < layered_window_width_; x ++)
					{
						data[y * (layered_window_width_ * block_size) + x * block_size + 0] = b; // B
						data[y * (layered_window_width_ * block_size) + x * block_size + 1] = g; // G
						data[y * (layered_window_width_ * block_size) + x * block_size + 2] = r; // R
						data[y * (layered_window_width_ * block_size) + x * block_size + 3] = a; // A
					}
				}
			}
			
			HBITMAP hBmpOld = (HBITMAP)::SelectObject(hStrechMemDC, hStrechBMP);

			{
				{
					RECT rc;
					rc.left = 0;
					rc.top = 0;
					rc.right = layered_window_width_;
					rc.bottom = layered_window_caption_height_;
					::FillRect(hStrechMemDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
					::DrawText(hStrechMemDC, caption_text_, _tcslen(caption_text_), &rc, DT_LEFT | DT_VCENTER);
				}
				{
					RECT rc;
					rc.left = 0;
					rc.top = layered_window_caption_height_;
					rc.right = layered_window_width_;
					rc.bottom = layered_window_height_;
					//::FillRect(hStrechMemDC, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
					::DrawText(hStrechMemDC, description_text_, _tcslen(description_text_), &rc, DT_CENTER | DT_WORDBREAK);
				}
			}
			{
				for (int y = 0; y < layered_window_height_; y++)
				{
					for (int x = 0; x < layered_window_width_; x ++)
					{
						data[y * (layered_window_width_ * block_size) + x * block_size + 3] = 0xff; // A
					}
				}
			}
			
			
#if USE_LAYERED_WINDOW
			BOOL ret = ::UpdateLayeredWindow(hWnd, hDesktopDc, &ptDest, &sz, hStrechMemDC, &ptSrc, 0, &bf, ULW_ALPHA);
			if (!ret) {
				DWORD err = GetLastError();
				std::stringstream ss;
				ss << "UpdateLayeredWindow err: " << err << "\n";
				OutputDebugStringA(ss.str().c_str());
			}
#else 
			HDC window_hdc = GetWindowDC(hWnd);
			BitBlt(window_hdc, 0, 0, layered_window_width_, layered_window_height_, hStrechMemDC, 0, 0, SRCCOPY);
			ReleaseDC(hWnd, window_hdc);
#endif
			SelectObject(hStrechMemDC, hBmpOld);
			::DeleteDC(hStrechMemDC);
			::DeleteObject(hStrechBMP);
			ReleaseDC(NULL, hDesktopDc);
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
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
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		return 0;
		break;
	case WM_DESTROY:
		::KillTimer(hWnd, TIMER_ID);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
