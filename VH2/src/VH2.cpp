#include <Windows.h>

#include <thread>

#include "tools/Tools.hpp"
#include "tools/Logger.hpp"
#include "hooks/box_motion/InAirCollision.hpp"
#include "hooks/box_motion/sub_1809DF060.hpp"
#include "hooks/other/sub_180707F00.hpp"

constexpr auto CAMERA{ 0x1436A18 };

namespace signatures {

	namespace objects {

		constexpr auto BOX_MOTION_VMT{ "48 8D 05 ? ? ? ? 48 89 ? 48 8B C3 C7 83" };
		constexpr auto RENDERER{ "4C 8B 35 ? ? ? ? 4C 89 78 ? 45 32" }; // 0x166B498
	}
	namespace functions {

		constexpr auto SUB_180707F00{ "48 8B C4 48 89 58 ? 48 89 68 ? 48 89 70 ? 48 89 78 ? 41 55 41 56 41" }; // 0x707F00
	}
}

std::jthread g_mainThread{};

std::unique_ptr<InAirCollision> g_jumpCollisionHook{};
std::unique_ptr<sub_1809DF060> g_sub_1809DF060Hook{};
std::unique_ptr<sub_180707F00> g_sub_180707F00Hook{};

class RendererUnknown {

	char pad1[0xA6C];
	std::int32_t elementCount; // 0xA6C
};

class Renderer {

	RendererUnknown* rendererUnkown;  // 0x0
	char pad1[0x2A2];				  // 0x8
	__m128 matrix;					  // 0x2AA
	char pad2[0x7DE];				  // 0x2C0
	std::uint32_t flag;				  // 0xA9E
	char pad3[0x1FE2];				  // 0xAA2
	std::int32_t tickCount;			  // 0x2A80
	RendererUnknown* rendererUnkown2; // 0x2B10 // mostly a nullptr

	static Renderer* get() {

		static Renderer* renderer{ nullptr };
		if (renderer == nullptr) {

			renderer = tools::GetAddress<Renderer*>("halo2.dll", 0x166B498, true);
		}

		return renderer;
	}
};

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*reserved*/) {

	if (reason == DLL_PROCESS_ATTACH) {

		DisableThreadLibraryCalls(module);

		tools::OpenConsole();

		g_mainThread = std::jthread{ [&](std::stop_token token, HMODULE module) {

			MH_Initialize();

			{
				auto renderer = reinterpret_cast<Renderer*>(tools::FindSignature("halo2.dll", signatures::objects::RENDERER, 0x3));
			}

			{
				auto sub_180707F00Address = reinterpret_cast<void*>(tools::FindSignature("halo2.dll", signatures::functions::SUB_180707F00));
				g_sub_180707F00Hook = std::make_unique<sub_180707F00>(sub_180707F00Address);
			}

			{
				auto boxMotionVMT = reinterpret_cast<void*>(tools::FindSignature("halo2.dll", signatures::objects::BOX_MOTION_VMT, 0x3));
				auto jumpCollisionAddress = tools::GetVirtualFunction<void*>(boxMotionVMT, 1, false);
				auto sub_1809DF060Address = tools::GetVirtualFunction<void*>(boxMotionVMT, 2, false);


				g_jumpCollisionHook = std::make_unique<InAirCollision>(jumpCollisionAddress);
				g_sub_1809DF060Hook = std::make_unique<sub_1809DF060>(sub_1809DF060Address);
			}

			MH_EnableHook(MH_ALL_HOOKS);

			while (token.stop_requested() == false) {

				if (GetAsyncKeyState(VK_DELETE) & 1) {

					g_mainThread.request_stop();

					g_jumpCollisionHook.reset();
					g_sub_1809DF060Hook.reset();
					g_sub_180707F00Hook.reset();
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}

			std::this_thread::sleep_for(std::chrono::seconds(2));

			MH_Uninitialize();

			FreeLibrary(module);
		}, module };

		g_mainThread.detach();
	}

	return TRUE;
}
