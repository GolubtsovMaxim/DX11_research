//#include "BaseWin.h"
//#include "WinMessagesMap.h"
#include "Window.h"

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLIne,
	int nCmdShow
)
{
	/*const auto pClassName = "dx11_research";
	
	HWND hWnd = CreateWindowExA(
		0, pClassName, "test_dx11",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		200, 200, 640, 480,
		nullptr, nullptr,
		hInstance,
		nullptr
	);
	ShowWindow(hWnd, SW_SHOW);*/

	Window wnd(800, 600, "Start_Window");
	
	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
	{
		return -1;
	}

	return msg.wParam;
}