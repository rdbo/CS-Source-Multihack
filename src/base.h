#pragma once
#ifndef BASE_H
#define BASE_H

#include <pch.h>
#include "utils/vmtmgr.h"
#include "utils/hookmgr.h"
#include "utils/d3d9dummy.h"
#include "utils/windowdata.h"
#include "utils/gameutl.h"

DWORD WINAPI EntryThread(LPVOID lpReserved);
DWORD WINAPI ExitThread(LPVOID lpReserved);

namespace Base
{
	void Init(HMODULE hMod);
	void Shutdown();
	void Unload();

	namespace Data
	{
		extern bool    IsLoaded;
		extern HMODULE hModule;
		extern std::unique_ptr<D3D9Dummy>    d3d9_dummy;
		extern std::unique_ptr<HookManager>  d3d9_hook;
		extern IDirect3DDevice9*             d3d9_dev;
		extern std::unique_ptr<WindowData>   window;
		extern WNDPROC                       oWndProc;
		extern std::array<bool, 0xFF>        WmKeys;
		extern mem_module_t                  m_Client;
		extern mem_module_t                  m_Engine;
		extern mem_module_t                  m_MaterialSystem;
		extern mem_module_t                  m_Vstdlib;

		extern std::unique_ptr<GameUtl>      game_utl;
		extern IBaseClientDLL*               client;
		extern IVEngineClient*               engine;
		extern IClientEntityList*            entitylist;
		extern ICvar*                        cvar;
		extern IClientMode*                  g_pClientMode;
		extern std::unique_ptr<VMTManager>   ClientModeVMT;
	}

	namespace Settings
	{
		extern bool                          ShowMenu;
		extern bool                          EnableBunnyhop;

		namespace Keys
		{
			const int                        Menu   = VK_INSERT;
			const int                        Unload = VK_END;
		}
	}

	namespace Hacks
	{
		void Bunnyhop(C_BasePlayer* pLocalPlayer, CUserCmd* pCmd);
	}

	namespace Hooks
	{
		HRESULT __stdcall EndScene(IDirect3DDevice9* thisptr);
		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		bool __cdecl CreateMove(IClientMode* thisptr, float flInputSampleTime, CUserCmd* cmd);
	}
}

#endif