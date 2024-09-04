#pragma once

#include "hooks/data/Hook.hpp"

class sub_180707F00 {
public:
	sub_180707F00(void* address);
	~sub_180707F00();

	static std::int64_t* hook(int a1, int a2, __int64 a3, int a4, __int64 a5, float a6, __int64 a7, char a8, float a9, unsigned int a10);
private:
	void* m_address;
	static std::atomic_flag m_flag;
	static decltype(&hook) m_original;
};