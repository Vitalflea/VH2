#pragma once

#include "hooks/box_motion/InAirCollision.hpp"

InAirCollision::InAirCollision(void* address)
	: m_address{ address } {

	if (MH_CreateHook(this->m_address, hook, reinterpret_cast<void**>(&this->m_original)) != MH_OK) {
		throw std::runtime_error("Failed to create InAirCollision hook");
	}

	logger::Info("InAirCollision Hooked: 0x%llx", this->m_address);
}

InAirCollision::~InAirCollision() {

	MH_QueueDisableHook(this->m_address);

	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	} while (m_flag.test());

	MH_RemoveHook(this->m_address);

	logger::Info("InAirCollision Unhooked: 0x%llx", this->m_address);
}

std::int64_t InAirCollision::hook(__m128* hitbox, float* rate) {

	AtomicHook atomicHook(&m_flag);

	//logger::Info("InAirCollision - rate: %f", *rate);

	return m_original(hitbox, rate);
}

std::atomic_flag InAirCollision::m_flag = ATOMIC_FLAG_INIT;
decltype(&InAirCollision::hook) InAirCollision::m_original;