#pragma once
#include <unordered_map>
#include <Windows.h>

class WinMessagesMap
{
public:
	WinMessagesMap() noexcept;
	std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const noexcept;
private:
	std::unordered_map<DWORD, std::string> map;
};