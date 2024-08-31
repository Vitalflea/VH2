#pragma once

#include <Windows.h>
#include <string>

namespace tools {

	void OpenConsole();
	std::uintptr_t FindSignature(const std::string& module, const std::string& bytes, const std::uint8_t& relativeOffsetPod = 0);

	template<typename T = void*>
	T GetVirtualFunction(void* classBase, unsigned int index, const bool& dereference = true) {

		if (classBase == nullptr) {

			return nullptr;
		}

		return dereference
			? reinterpret_cast<T>((*static_cast<std::uintptr_t**>(classBase))[index])
			: reinterpret_cast<T>((static_cast<std::uintptr_t*>(classBase))[index]);
	}

	template <typename T>
	T GetAddress(const std::string& module, const std::uintptr_t& address, const bool& dereference = false) {

		auto moduleHandle = GetModuleHandle(module.c_str());
		if (moduleHandle == nullptr) {

			return nullptr;
		}

		return dereference
			? *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(moduleHandle) + address)
			: reinterpret_cast<T>(reinterpret_cast<std::uintptr_t>(moduleHandle) + address);
	}
}