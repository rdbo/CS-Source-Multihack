#include <pch.h>
#include "base.h"

DWORD WINAPI EntryThread(LPVOID lpReserved)
{
	Base::Init();
	return TRUE;
}

DWORD WINAPI ExitThread(LPVOID lpReserved)
{
	Base::Shutdown();
	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, EntryThread, hModule, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		CreateThread(nullptr, 0, ExitThread, hModule, 0, nullptr);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}