#include <pch.h>
#include "base.h"

DWORD WINAPI EntryThread(LPVOID lpReserved)
{
	Base::Init((HMODULE)lpReserved);
	return TRUE;
}

DWORD WINAPI ExitThread(LPVOID lpReserved)
{
	if (Base::Data::hModule)
	{
		HMODULE hModule = Base::Data::hModule;
		Base::Data::hModule = (HMODULE)NULL;
		FreeLibraryAndExitThread(hModule, 0);
	}

	return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_THREAD_ATTACH:  break;
	case DLL_THREAD_DETACH:  break;
	case DLL_PROCESS_ATTACH: CreateThread(nullptr, 0, EntryThread, hModule, 0, nullptr); break;
	case DLL_PROCESS_DETACH: Base::Shutdown();  break;
	}

	return TRUE;
}