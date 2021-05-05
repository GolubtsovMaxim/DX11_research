#include "DXGIInfoContainer.h"

#include <dxgidebug.h>
#include <memory>

#include "Graphics.h"
#include "Window.h"


#pragma comment (lib, "dxguid.lib")

#define GRAPHICS_THROW_NOINFO(hrcall) if (FAILED(hr = (hrcall))) throw Graphics::HrException( __LINE__,__FILE__,hr )

DXGIInfoContainer::DXGIInfoContainer()
{
	// DXGIGetDebugInterface function signature
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);


	// load dll containing DXGIGetDebugInterface function
	const auto hModDxgiDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	if (hModDxgiDebug == nullptr)
	{
		throw LAST_EXCPT();
	}

	// get address of DXGIGetDebugInterface in dll
	const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDxgiDebug, "DXGIGetDebugInterface"))
		);
	if (DxgiGetDebugInterface == nullptr)
	{
		throw LAST_EXCPT();
	}

	HRESULT hr;
	GRAPHICS_THROW_NOINFO(DxgiGetDebugInterface(__uuidof(IDXGIInfoQueue), &p_idxgi_info_queue_));
	
}

void DXGIInfoContainer::set() noexcept
{
	next = p_idxgi_info_queue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DXGIInfoContainer::get_messages() const
{
	std::vector<std::string> messages;
	const auto end = p_idxgi_info_queue_->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;
		// get the size of message i in bytes
		GRAPHICS_THROW_NOINFO(p_idxgi_info_queue_->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));
		// allocate memory for message
		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());
		// get the message and push its description into the vector
		GRAPHICS_THROW_NOINFO(p_idxgi_info_queue_->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));
		messages.emplace_back(pMessage->pDescription);
	}
	return messages;
}