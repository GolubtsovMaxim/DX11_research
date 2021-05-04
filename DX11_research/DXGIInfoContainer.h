#pragma once
#include "BaseWin.h"
#include <string>
#include <vector>

class DXGIInfoContainer
{
public:
	DXGIInfoContainer();
	~DXGIInfoContainer();
	DXGIInfoContainer(const DXGIInfoContainer&) = delete;
	DXGIInfoContainer& operator=(const DXGIInfoContainer&) = delete;
	void set() noexcept;
	std::vector<std::string> get_messages() const;
private:
	unsigned long long next = 0u;

	struct IDXGIInfoQueue* p_idxgi_info_queue_ = nullptr;
};