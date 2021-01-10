#include "Window.h"

Window::Window_Class Window::Window_Class::wnd_class_;

Window::Window_Class::Window_Class() noexcept
		:
			hinstance_(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc{ 0 };

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handle_message_setup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance_;
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = get_name();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::Window_Class::~Window_Class()
{
	UnregisterClass(wnd_class_name, get_hinstance());
}

const char* Window::Window_Class::get_name() noexcept
{
	return wnd_class_name;
}

HINSTANCE Window::Window_Class::get_hinstance() noexcept
{
	return wnd_class_.hinstance_;
}

Window::Window(unsigned width, unsigned height, const char* name) noexcept
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	
	// create window & get hwnd_
	hwnd_ = CreateWindow(
		Window_Class::get_name(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, Window_Class::get_hinstance(), this
	);
	// show window
	ShowWindow(hwnd_, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(hwnd_);
}

LRESULT WINAPI Window::handle_message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE) // WM_NCCREATE = Non Client Create
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::handle_message_thunk));
		// forward message to window class handler
		return pWnd->handle_message(hwnd, msg, wparam, lparam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT WINAPI Window::handle_message_thunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->handle_message(hWnd, msg, wParam, lParam);
}

LRESULT Window::handle_message(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}