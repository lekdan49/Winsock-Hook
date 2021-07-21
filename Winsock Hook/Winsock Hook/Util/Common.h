#pragma once
#include <vector>

namespace util
{
	void CreateConsole();
	HMODULE GetCurrentModule();
	void Log_buffer(const char* fmt, const std::vector<uint8_t>& msg);
	void Log(const char* fmt, ...);
}