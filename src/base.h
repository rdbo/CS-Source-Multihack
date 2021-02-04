#pragma once
#ifndef BASE_H
#define BASE_H

#include <pch.h>
#include "utils/hookmgr.h"
#include "utils/vmtmgr.h"
#include "utils/d3d9dummy.h"
#include "utils/windowdata.h"

#define BCHECK(var, cond) var = cond ? var : MEM_FALSE

DWORD WINAPI EntryThread(LPVOID lpReserved);
DWORD WINAPI ExitThread(LPVOID lpReserved);

namespace Base
{
	void Init();
	void Message(mem_string_t msg);
	void Shutdown();
	void Unload();

	namespace Data
	{
		extern HMODULE                        hModule;
		extern bool                           Loaded;
		extern std::unique_ptr<D3D9Dummy>     d3d9dummy;
		extern std::unique_ptr<HookManager>   d3d9hk;
		extern IDirect3DDevice9*              d3d9dev;
		extern std::unique_ptr<WindowData>    window;
		extern WNDPROC                        oWndProc;
		extern std::array<bool, 0xFF>         WmKeys;
	}

	namespace Settings
	{
		extern bool                           ShowMenu;

		namespace Keys
		{
			const int Menu   = VK_INSERT;
			const int Unload = VK_END;
		}
	}

	namespace Hacks
	{

	}

	namespace Hooks
	{
		HRESULT __stdcall EndScene(IDirect3DDevice9* thisptr);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}

#endif