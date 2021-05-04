#pragma once
//#include "Window.h"

#include <d3d11.h>

#include "BaseWin.h"
#include "BaseException.h"
#include "DXGIInfoContainer.h"

class Graphics
{
public:
	class Exception : public BaseException
	{
		using BaseException::BaseException;
	};
	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* get_type() const noexcept override;
		HRESULT get_error_code() const noexcept;
		std::string get_error_string() const noexcept;
		std::string get_error_description() const noexcept;
		std::string get_error_info() const noexcept;
	private:
		HRESULT m_hr;
		std::string m_info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* get_type() const noexcept override;
	private:
		std::string m_reason_;
	};

	public:
		Graphics(HWND hWnd); //we need a window ref to construct a d3d device
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;
		~Graphics();
		void end_frame();
		void clear_buffer(float red, float green, float blue) noexcept;
	private:
#ifndef NDEBUG
		DXGIInfoContainer m_info_container_;
#endif

		ID3D11Device* p_device_ = nullptr;
		IDXGISwapChain* p_swap_chain_ = nullptr;
		ID3D11DeviceContext* p_device_context_ = nullptr;
		ID3D11RenderTargetView* p_render_target_view_ = nullptr;
		//DXGI_SWAP_CHAIN_DESC p_desc_ = nullptr;
};