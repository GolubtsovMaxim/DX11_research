#include "Graphics.h"
#include "dxerr.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")

//#define GRAPHICS_THROW_FAILED(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )
//#define GRAPHICS_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )

#define GRAPHICS_EXCEPT_NOINFO(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GRAPHICS_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

#ifndef NDEBUG
#define GRAPHICS_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr), m_info_container_.get_messages() )
#define GRAPHICS_THROW_INFO(hrcall) m_info_container_.set(); if( FAILED( hr = (hrcall) ) ) throw GRAPHICS_EXCEPT(hr)
#define GRAPHICS_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr),m_info_container_.get_messages() )
#else
#define GRAPHICS_EXCEPT(hr) Graphics::HrException( __LINE__,__FILE__,(hr) )
#define GRAPHICS_THROW_INFO(hrcall) GRAPHICS_THROW_NOINFO(hrcall)
#define GRAPHICS_DEVICE_REMOVED_EXCEPT(hr) Graphics::DeviceRemovedException( __LINE__,__FILE__,(hr) )
#endif

Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
	swap_chain_desc.BufferDesc.Width = 0;
	swap_chain_desc.BufferDesc.Height = 0;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // -> R G B A, for short
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 0;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swap_chain_desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = hWnd;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;

	UINT swapCreateFlags = 0u;

#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	
	HRESULT hr;
	
	//create "mother" of all objects - Device
	//init device, front/back buffers, swap chain and rendering context
	
	GRAPHICS_THROW_INFO( D3D11CreateDeviceAndSwapChain
	( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&swap_chain_desc,
		&p_swap_chain_,
		&p_device_,
		nullptr,
		&p_device_context_
	));

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GRAPHICS_THROW_INFO(p_swap_chain_->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	GRAPHICS_THROW_INFO(p_device_->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &p_render_target_view_));
}

void Graphics::end_frame()
{
	HRESULT hr;
#ifndef NDEBUG
	m_info_container_.set();
#endif
	if (FAILED( hr = p_swap_chain_->Present(1u, 0u)))
	{
		if ( hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GRAPHICS_DEVICE_REMOVED_EXCEPT(p_device_->GetDeviceRemovedReason());
		}
		else
		{
			throw GRAPHICS_EXCEPT(hr);
		}
	}
}

void Graphics::clear_buffer(float red, float green, float blue) noexcept
{
	const float colors[] = { red, green, blue, 1.0f };
	p_device_context_->ClearRenderTargetView(p_render_target_view_.Get(), colors);
}

Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> info_msgs) noexcept
	:
	Exception(line, file),
	m_hr(hr)
{
	for (const auto& m : info_msgs)
	{
		m_info += m;
		m_info.push_back('\n');
	}
	// remove final newline if exists
	if (!m_info.empty())
	{
		m_info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << get_type() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << get_error_code()
		<< std::dec << " (" << (unsigned long)get_error_code() << ")" << std::endl
		<< "[Error String]" << get_error_string() << std::endl
		//<< "[Error Description]" << get_error_description() << std::endl
		//<< get_origin_string();

		<< "[Description] " << get_error_description() << std::endl;
	if (!m_info.empty())
	{
		oss << "\n[Error Info]\n" << get_error_info() << std::endl << std::endl;
	}
	oss << get_origin_string();
	
	what_buffer = oss.str();
	return what_buffer.c_str();
}

const char* Graphics::HrException::get_type() const noexcept
{
	return "Graphics Exception";
}

HRESULT Graphics::HrException::get_error_code() const noexcept
{
	return m_hr;
}

std::string Graphics::HrException::get_error_string() const noexcept
{
	return DXGetErrorString(m_hr);
}

std::string Graphics::HrException::get_error_description() const noexcept
{
	char buf[512];
	DXGetErrorDescription(m_hr, buf, sizeof(buf));
	return buf;
}


const char* Graphics::DeviceRemovedException::get_type() const noexcept
{
	return " Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

std::string Graphics::HrException::get_error_info() const noexcept
{
	return m_info;
}
