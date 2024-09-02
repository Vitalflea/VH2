#include <Windows.h>

#include <thread>

#include "tools/Tools.hpp"
#include "tools/Logger.hpp"
#include "hooks/box_motion/InAirCollision.hpp"
#include "hooks/box_motion/sub_1809DF060.hpp"

constexpr auto CAMERA{ 0x1436A18 };

std::jthread g_mainThread{};

std::unique_ptr<InAirCollision> g_jumpCollisionHook{};
std::unique_ptr<sub_1809DF060> g_sub_1809DF060Hook{};

//_QWORD* __fastcall sub_180707F00(
//	int a1,
//	int a2,
//	__int64 a3,
//	int a4,
//	__int64 a5,
//	float a6,
//	__int64 a7,
//	char a8,
//	float a9,
//	unsigned int a10)
//{
BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID /*reserved*/) {

	if (reason == DLL_PROCESS_ATTACH) {

		DisableThreadLibraryCalls(module);

		tools::OpenConsole();

		g_mainThread = std::jthread{ [&](std::stop_token token, HMODULE module) {

			auto boxMotionVMT = reinterpret_cast<void*>(tools::FindSignature("halo2.dll", "48 8D 05 ? ? ? ? 48 89 ? 48 8B C3 C7 83", 0x3));
			auto jumpCollisionAddress = tools::GetVirtualFunction<void*>(boxMotionVMT, 1, false);
			auto sub_1809DF060Address = tools::GetVirtualFunction<void*>(boxMotionVMT, 2, false);

			MH_Initialize();

			g_jumpCollisionHook = std::make_unique<InAirCollision>(jumpCollisionAddress);
			g_sub_1809DF060Hook = std::make_unique<sub_1809DF060>(sub_1809DF060Address);

			MH_EnableHook(MH_ALL_HOOKS);

			while (token.stop_requested() == false) {

				if (GetAsyncKeyState(VK_DELETE) & 1) {

					g_mainThread.request_stop();

					g_jumpCollisionHook.reset();
					g_sub_1809DF060Hook.reset();
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