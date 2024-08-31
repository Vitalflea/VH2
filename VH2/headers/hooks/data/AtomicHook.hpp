#pragma once

#include <memory>

class AtomicHook {
public:
	AtomicHook(std::atomic_flag* flag);
	~AtomicHook();
private:
	std::atomic_flag* m_flag;
};