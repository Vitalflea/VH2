#include "tools/Tools.hpp"

#include <Psapi.h>

#include <iostream>
#include <sstream>
#include <vector>

namespace tools {

	void OpenConsole() {

		AllocConsole();

		FILE* file{ nullptr };
		freopen_s(&file, "CONOUT$", "w", stdout);
		freopen_s(&file, "CONIN$", "r", stdin);
	}

	std::uintptr_t FindSignature(const std::string& module, const std::string& bytes, const std::uint8_t& relativeOffsetPos) {

		std::vector<std::uint8_t> sigBytes{};
		sigBytes.reserve(bytes.size() / 2);

		std::string number{};
		std::stringstream iss(bytes);
		while (iss >> number) {

			sigBytes.push_back(static_cast<std::uint8_t>(std::strtol(number.c_str(), nullptr, 16)));
		}

		std::uintptr_t address{ 0 };

		const auto& moduleHandle = GetModuleHandle(module.c_str());
		if (moduleHandle == nullptr) {

			return 0;
		}

		MODULEINFO mod{};
		K32GetModuleInformation(GetCurrentProcess(), moduleHandle, &mod, sizeof(MODULEINFO));

		const auto& start = reinterpret_cast<std::uintptr_t>(mod.lpBaseOfDll);

		std::uintptr_t sig_position{ 0 };

		for (std::uintptr_t i = start; i < start + mod.SizeOfImage; ++i) {

			if (sig_position == sigBytes.size()) {

				address = i - sigBytes.size();
				break;
			}

			const auto& sigByte = sigBytes.at(sig_position);

			*reinterpret_cast<std::uint8_t*>(i) == sigByte || sigByte == '\0'
				? sig_position++
				: sig_position = 0;
		}

		if (relativeOffsetPos != 0 && address != 0) {

			address += (relativeOffsetPos + 4) + *reinterpret_cast<std::uint32_t*>(address + relativeOffsetPos);
		}

		return address;
	}
}