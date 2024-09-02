#pragma once

#include "hooks/data/Hook.hpp"

class sub_1809DF060 {
public:
	sub_1809DF060(void* address);
	~sub_1809DF060();

	static std::int64_t hook(__m128* a1, float* a2, float* a3);
private:
	void* m_address;
	static std::atomic_flag m_flag;
	static decltype(&hook) m_original;
};