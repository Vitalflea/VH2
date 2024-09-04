#pragma once

#include "hooks/other/sub_180707F00.hpp"

sub_180707F00::sub_180707F00(void* address)
	: m_address{ address } {

	if (MH_CreateHook(this->m_address, hook, reinterpret_cast<void**>(&this->m_original)) != MH_OK) {
		throw std::runtime_error("Failed to create sub_180707F00 hook");
	}

	logger::Info("sub_180707F00 Hooked: 0x%llx", this->m_address);
}

sub_180707F00::~sub_180707F00() {

	MH_QueueDisableHook(this->m_address);

	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	} while (m_flag.test());

	MH_RemoveHook(this->m_address);

	logger::Info("sub_180707F00 Unhooked: 0x%llx", this->m_address);
}

std::int64_t* sub_180707F00::hook(int a1, int a2, __int64 a3, int a4, __int64 a5, float a6, __int64 a7, char a8, float a9, unsigned int a10) {

	AtomicHook atomicHook(&m_flag);

	logger::Info("a1: %d a2: %d a3: %ll a4: %d a5: %ll a7: %f a8: %c a9: %f a10: %d", a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);

	return m_original(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

std::atomic_flag sub_180707F00::m_flag = ATOMIC_FLAG_INIT;
decltype(&sub_180707F00::hook) sub_180707F00::m_original;
