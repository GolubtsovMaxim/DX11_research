#pragma once
#include "BaseWin.h"

class Window
{
public:
	Window(unsigned width, unsigned height, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window operator= (const Window&) = delete;

	private:	
	class Window_Class
	{
		public:		
			static const char* get_name() noexcept;
			static HINSTANCE get_hinstance() noexcept;
			
		private:
			Window_Class(const Window_Class&) = delete;
			Window_Class& operator=(const Window_Class&) = delete;
			Window_Class() noexcept;
			~Window_Class();
			static constexpr const char* wnd_class_name = "Main window";
			static Window_Class wnd_class_;
			HINSTANCE hinstance_;	
	};

	private:
		static LRESULT CALLBACK handle_message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		static LRESULT CALLBACK handle_message_thunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		LRESULT handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

	private:
		unsigned width_;
		unsigned height_;
		HWND hwnd_;
};