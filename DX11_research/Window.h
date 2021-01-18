#pragma once
#include "BaseWin.h"
#include "BaseException.h"

class Window
{
public:
	class Exception : public BaseException
	{
		public:
			Exception(unsigned line, const char* file, HRESULT hr) noexcept;
			const char* what() const noexcept override;
			const char* get_type() const noexcept override;
			static std::string translate_error_code( HRESULT hr) noexcept;
			HRESULT get_error_code() const noexcept;
			std::string get_error_string() const noexcept;
		private:
			HRESULT m_hr; //HResult - 32-bit value that is used to describe an error or warning. 
	};
	
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

		static LRESULT CALLBACK handle_message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		static LRESULT CALLBACK handle_message_thunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;
		LRESULT handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) noexcept;

		unsigned width_;
		unsigned height_;
		HWND hwnd_;
};

#define EXCPT (hr) Window::Exception (__LINE__, __FILE__, hr)
