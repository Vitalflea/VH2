#pragma once

#include "hooks/box_motion/sub_1809DF220.hpp"

sub_1809DF220::sub_1809DF220(void* address)
	: m_address{ address } {

	if (MH_CreateHook(this->m_address, hook, reinterpret_cast<void**>(&this->m_original)) != MH_OK) {
		throw std::runtime_error("Failed to create sub_1809DF220 hook");
	}

	logger::Info("sub_1809DF220 Hooked: 0x%llx", this->m_address);
}

sub_1809DF220::~sub_1809DF220() {

	MH_QueueDisableHook(this->m_address);

	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	} while (m_flag.test());

	MH_RemoveHook(this->m_address);

	logger::Info("sub_1809DF220 Unhooked: 0x%llx", this->m_address);
}

std::int64_t sub_1809DF220::hook(__m128* hitbox, float* a2, float* a3, std::int64_t a4) {

	AtomicHook atomicHook(&m_flag);

	//logger::Info("sub_1809DF220 - a2: %f | a3: %f | a4: %lld", *a2, *a3, a4);

	return m_original(hitbox, a2, a3, a4);
}

std::atomic_flag sub_1809DF220::m_flag = ATOMIC_FLAG_INIT;
decltype(&sub_1809DF220::hook) sub_1809DF220::m_original;