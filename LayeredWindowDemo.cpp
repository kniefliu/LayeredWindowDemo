// LayeredWindowDemo.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "LayeredWindowDemo.h"

#include <sstream>

#define MAX_LOADSTRING 100

#define TIMER_ID 100

#define USE_LAYERED_WINDOW 1

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
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

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAYEREDWINDOWDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAYEREDWINDOWDEMO));

	// ����Ϣѭ��:
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
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
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
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

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
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
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
		// �����˵�ѡ��:
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
		// TODO: �ڴ���������ͼ����...
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

// �����ڡ������Ϣ�������
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
