#pragma once

#include <MinHook.h>
#include <xmmintrin.h>

#include <thread>

#include "AtomicHook.hpp"
#include "tools/Logger.hpp"

//#define FAST_HOOK(C, A, H)                                                                                    \
//	class C {                                                                                                 \
//	public:                                                                                                   \
//		C(void* address)                                                                                      \
//			: m_address{ address } {                                                                          \
//                                                                                                              \
//			if (MH_CreateHook(this->m_address, hook, reinterpret_cast<void**>(&this->m_original)) != MH_OK) { \
//				return;																						  \
//			}                                                                                                 \
//		}                                                                                                     \
//		~InAirCollision() {                                                                                   \
//                                                                                                              \
//			MH_QueueDisableHook(this->m_address);                                                             \
//                                                                                                              \
//			do {                                                                                              \
//				std::this_thread::sleep_for(std::chrono::milliseconds(1));                                    \
//			} while (m_flag.test());                                                                          \
//                                                                                                              \
//			MH_RemoveHook(this->m_address);                                                                   \
//		}                                                                                                     \
//                                                                                                              \
//		static H                                                                                              \
//                                                                                                              \
//	private : void* m_address;																				  \
//		static std::atomic_flag m_flag;                                                                       \
//		static decltype(&hook) m_original;                                                                    \
//	};                                                                                                        \
//	std::unique_ptr<C> g_##C;
