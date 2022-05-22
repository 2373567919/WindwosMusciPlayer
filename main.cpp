#include "fun.h"



int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
	//创建窗口类
	WNDCLASSEX wcex = { 0 };
    TCHAR szWindowClass[] = _T("szWindowClass");
    TCHAR szTile[] = _T("音乐播放器");
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WindowsProc;
	wcex.lpszClassName = szWindowClass;
	wcex.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
	if (!RegisterClassEx(&wcex))//注册窗口类
	{
		MessageBox(NULL, _T("注册窗口类失败。"), _T("Trip。"), NULL);
		return 1;
	}
	//创建窗口
	HWND hWnd = CreateWindowEx(
		WS_EX_ACCEPTFILES,
		szWindowClass,
		szTile,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		425, 825,
		NULL, NULL,
		hInstance,
		NULL
		);
	if (!hWnd)
	{
		MessageBox(NULL, _T("创建窗口失败。"), _T("trip。"), NULL);
		return 1;
	}
	ShowWindow(hWnd, SW_SHOW);//显示窗口
	UpdateWindow(hWnd);//更新窗口
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))//消息循环
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}


