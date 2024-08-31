#include "hooks/data/AtomicHook.hpp"

AtomicHook::AtomicHook(std::atomic_flag* flag)
	: m_flag{ flag } {
	this->m_flag->test_and_set();
}
AtomicHook::~AtomicHook() {
	this->m_flag->clear();
}
