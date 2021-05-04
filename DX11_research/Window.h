#pragma once
#include "BaseWin.h"
#include "BaseException.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

#include <memory>
#include <optional>

class Window
{
public:
	class Exception : public BaseException
	{
		//public:
		//	Exception(unsigned line, const char* file, HRESULT hr) noexcept;
		//	const char* what() const noexcept override;
		//	const char* get_type() const noexcept override;
		//	static std::string translate_error_code( HRESULT hr) noexcept;
		//	HRESULT get_error_code() const noexcept;
		//	std::string get_error_string() const noexcept;
		//private:
		//	HRESULT m_hr; //HResult - 32-bit value that is used to describe an error or warning.
			using BaseException::BaseException;
		public:
			static std::string translate_error_code(HRESULT hr) noexcept;
	};
	class HrException : public Exception
	{
		public:
			HrException(int line, const char* file, HRESULT hr) noexcept;
			const char* what() const noexcept override;
			const char* get_type() const noexcept override;
			HRESULT get_error_code() const noexcept;
			std::string get_error_description() const noexcept;
			//std::string get_error_string() const noexcept;
		private:
			HRESULT m_hr;
	};
	class NoGraphicsException : public Exception
	{
			using Exception::Exception;
		public:
			const char* get_type() const noexcept override;
	};
	
	Window(unsigned width, unsigned height, const char* name) noexcept;
	~Window();
	Window(const Window&) = delete;
	Window operator= (const Window&) = delete;
	void set_title(const std::string& title);
	static std::optional<int> process_messages() noexcept;
	Graphics& graphics();
	
	Keyboard kbd;
	Mouse mouse;
	

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
		std::unique_ptr<Graphics> p_graphics_;
};

#define EXCPT(hr) Window::HrException (__LINE__, __FILE__, hr)
#define LAST_EXCPT() Window::HrException(__LINE__, __FILE__, GetLastError())
#define NOGRAPHICS_EXCEPT() Window::NoGraphicsException( __LINE__,__FILE__ )