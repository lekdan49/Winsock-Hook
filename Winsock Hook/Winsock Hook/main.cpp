#include <string>
#include <windows.h>
#include <iostream>
#include "MinHook\MinHook.h"
#include "Util\Common.h"
#include "Util\Packet.h"
#include "Util\Hook.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Dnsapi.lib")

#if defined _M_X64
#pragma comment(lib, "libMinHook-x64-v141-mdd.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook-x86-v141-mdd.lib")
#endif

void EjectDLL(HMODULE hModule)
{
	//std::cout << "Ejecting DLL after 0.5s" << std::endl;
	Sleep(500);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, EXIT_SUCCESS);
};

int DisableHooks() {
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return 1;
	}

	// Uninitialize MinHook.
	if (MH_Uninitialize() != MH_OK)
	{
		return 1;
	}
	return 0;
}

void check_MH_err(const std::string& mod_name, const MH_STATUS& err)
{
	if (err != MH_OK)
	{
		std::cout << mod_name << " Failed to hook: " << MH_StatusToString(err) << '\n';
	}
}

DWORD WINAPI HookThread(LPVOID lpModule)
{
	util::CreateConsole();
	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
		util::Log("MH_Initialize failed");

	// Create a hook for Ws2_32.send
	check_MH_err("send", MH_CreateHookApi(L"Ws2_32", "send", MySend, (LPVOID*)&psend));

	// Create a hook for Ws2_32.recv
	check_MH_err("recv", MH_CreateHookApi(L"Ws2_32", "recv", MyRecv, (LPVOID*)&precv));

	// create a hook for Dnsapi.DnsQuery_A
	check_MH_err("DnsQuery_A", MH_CreateHookApi(L"Dnsapi", "DnsQuery_A", myDNSQueryA, (LPVOID*)&pDNSQueryA));

	// Enable the hooks.
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		util::Log("MH_EnableHook failed");
		return 1;
	}

	return 0;
}



DWORD WINAPI MainThread(LPVOID lpModule)
{
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD_PTR ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, MainThread, hModule, NULL, NULL);
		CreateThread(NULL, 0, HookThread, hModule, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		DisableHooks();
		break;
	}
	return TRUE;
}