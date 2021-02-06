#include <pch.h>
#include "base.h"

bool                          Base::Data::IsLoaded = false;
HMODULE                       Base::Data::hModule = (HMODULE)NULL;
std::unique_ptr<D3D9Dummy>    Base::Data::d3d9_dummy = nullptr;
std::unique_ptr<HookManager>  Base::Data::d3d9_hook = nullptr;
IDirect3DDevice9*             Base::Data::d3d9_dev = nullptr;
std::unique_ptr<WindowData>   Base::Data::window = nullptr;
WNDPROC                       Base::Data::oWndProc = nullptr;
std::array<bool, 0xFF>        Base::Data::WmKeys = {};
mem_module_t                  Base::Data::m_Client = {};
mem_module_t                  Base::Data::m_Engine = {};
mem_module_t                  Base::Data::m_MaterialSystem = {};
mem_module_t                  Base::Data::m_Vstdlib = {};

std::unique_ptr<GameUtl>      Base::Data::game_utl = nullptr;
IBaseClientDLL*               Base::Data::client = nullptr;
IVEngineClient*               Base::Data::engine = nullptr;
IClientEntityList*            Base::Data::entitylist = nullptr;
ICvar*                        Base::Data::cvar = nullptr;
IClientMode*                  Base::Data::g_pClientMode = nullptr;
std::unique_ptr<VMTManager>   Base::Data::ClientModeVMT = nullptr;

bool                          Base::Settings::ShowMenu = true;
bool                          Base::Settings::EnableBunnyhop = false;

void Base::Init(HMODULE hMod)
{
	Data::hModule = hMod;

	//Modules
	Data::m_Client = mem::in::get_module("client.dll");
	Data::m_Engine = mem::in::get_module("engine.dll");
	Data::m_MaterialSystem = mem::in::get_module("materialsystem.dll");
	Data::m_Vstdlib = mem::in::get_module("vstdlib.dll");

	//D3D9
	Data::d3d9_dummy = std::make_unique<D3D9Dummy>();
	Data::d3d9_hook = std::make_unique<HookManager>();
	Data::d3d9_hook->HookTrampoline(Data::d3d9_dummy->GetFunction(ifnEndScene), (mem_voidptr_t)Hooks::EndScene, 7);

	//Game
	Data::game_utl = std::make_unique<GameUtl>();
	Data::client     = Data::game_utl->GetInterface<IBaseClientDLL>(Data::m_Client,    "VClient017");
	Data::engine     = Data::game_utl->GetInterface<IVEngineClient>(Data::m_Engine,    "VEngineClient014");
	Data::entitylist = Data::game_utl->GetInterface<IClientEntityList>(Data::m_Client, "VClientEntityList003");
	Data::cvar       = Data::game_utl->GetInterface<ICvar>(Data::m_Vstdlib,            "VEngineCvar004");
	Data::g_pClientMode = **(IClientMode***)GET_OFFSET((mem::in::pattern_scan((mem_data_t)"\xA3\x00\x00\x00\x00\x00\x00\x00\x00\xFF\x68\x00\x68", "x????????xx?x", Data::m_Client)), 0x1);
	Data::ClientModeVMT = std::make_unique<VMTManager>(*(mem_voidptr_t**)Data::g_pClientMode);
	Data::ClientModeVMT->SetFunction((mem_voidptr_t)Hooks::CreateMove, 25);

	Data::cvar->ConsoleColorPrintf(Color(125, 75, 255, 255), "[CS:MH] Hack Loaded");

	Data::IsLoaded = true;
}

void Base::Shutdown()
{
	if (Data::IsLoaded)
	{
		Data::d3d9_hook->RestoreAll();
		if (Data::window && Data::oWndProc)
			SetWindowLongPtr(Data::window->GetHandle(), GWL_WNDPROC, (LONG)Data::oWndProc);
		Data::IsLoaded = false;
	}
}

void Base::Unload()
{
	CreateThread(nullptr, 0, ExitThread, NULL, 0, nullptr);
}
