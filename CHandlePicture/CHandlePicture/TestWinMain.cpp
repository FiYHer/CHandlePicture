#include "BitmapHandle.h"
#include "CGeometricTransformation.h"

HWND g_winHwnd = NULL;
CGeometricTransformation g_pBitMap;

LRESULT CALLBACK Proc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC hdc = NULL;
	PAINTSTRUCT ps = { 0 };
	switch (message)
	{
	case WM_CREATE:
		g_pBitMap.ReadBitmap("H://111.bmp");
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		g_pBitMap.Clockwise90();
		g_pBitMap.BitmapDraw(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(hWnd,message,wParam,lParam);
	}
	return 1;
}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX stWnd = { 0 };
	stWnd.cbSize = sizeof(WNDCLASSEX);
	stWnd.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	stWnd.hCursor = LoadCursor(NULL, IDC_ARROW);
	stWnd.hInstance = hInstance;
	stWnd.lpfnWndProc = Proc;
	stWnd.lpszClassName = "WindowTest";
	stWnd.style = CS_VREDRAW | CS_HREDRAW;
	if (!::RegisterClassExA(&stWnd))
	{
		return -1;
	}

	g_winHwnd = ::CreateWindowEx(NULL,"WindowTest","WindowTest",
		WS_OVERLAPPEDWINDOW,300,300,500,500,
		NULL,NULL,hInstance,NULL);
	if (g_winHwnd == NULL)
	{
		return -1;
	}

	ShowWindow(g_winHwnd, nShowCmd);
	UpdateWindow(g_winHwnd);

	MSG msg = { 0 };
	while (GetMessage(&msg, 0, 0, 0))
	{
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return msg.wParam;
}