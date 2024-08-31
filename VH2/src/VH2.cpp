#include <Windows.h>

#include <thread>

#include "tools/Tools.hpp"
#include "tools/Logger.hpp"
#include "hooks/box_motion/JumpCollision.hpp"

constexpr auto CAMERA{ 0x1436A18 };

std::jthread g_mainThread{};

std::unique_ptr<JumpCollision> g_jumpCollisionHook{};

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*reserved*/) {

	if (reason == DLL_PROCESS_ATTACH) {

		DisableThreadLibraryCalls(module);

		tools::OpenConsole();

		g_mainThread = std::jthread{ [&](std::stop_token token, HMODULE module) {

			auto boxMotionVMT = reinterpret_cast<void*>(tools::FindSignature("halo2.dll", "48 8D 05 ? ? ? ? 48 89 ? 48 8B C3 C7 83", 0x3));
			auto jumpCollisionAddress = tools::GetVirtualFunction<void*>(boxMotionVMT, 1, false);

			MH_Initialize();

			g_jumpCollisionHook = std::make_unique<JumpCollision>(jumpCollisionAddress);

			MH_EnableHook(MH_ALL_HOOKS);

			while (token.stop_requested() == false) {

				if (GetAsyncKeyState(VK_DELETE) & 1) {

					g_mainThread.request_stop();

					g_jumpCollisionHook.reset();
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