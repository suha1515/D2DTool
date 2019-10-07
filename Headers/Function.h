#pragma once

#ifndef __FUNCTION_H__

template <typename T>
void SafeDelete(T& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
void SafeDelete_Array(T& ptr)
{
	if (ptr)
	{
		delete[] ptr;
		ptr = nullptr;
	}
}

namespace strings {
	static std::wstring widen(const std::string& s)
	{
		std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, 0, 0));
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), s.size() + 1, &buf[0], buf.size());
		return std::wstring(&buf[0]);
	}
}

#define __FUNCTION_H__
#endif