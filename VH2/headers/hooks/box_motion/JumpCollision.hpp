#pragma once

#include "hooks/data/Hook.hpp"

#include <xmmintrin.h>

class JumpCollision {
public:
	JumpCollision(void* address);
	~JumpCollision();

	static std::int64_t hook(__m128* hitbox, float* rate);
private:
	void* m_address;
	static std::atomic_flag m_flag;
	static decltype(&hook) m_original;
};
