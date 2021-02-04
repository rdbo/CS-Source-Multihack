#include <pch.h>
#include "base.h"

HMODULE                        Base::Data::hModule = (HMODULE)NULL;
bool                           Base::Data::Loaded = false;
std::unique_ptr<D3D9Dummy>     Base::Data::d3d9dummy = nullptr;
std::unique_ptr<HookManager>   Base::Data::d3d9hk  = nullptr;
IDirect3DDevice9*              Base::Data::d3d9dev = nullptr;
std::unique_ptr<WindowData>    Base::Data::window  = nullptr;
WNDPROC                        Base::Data::oWndProc = nullptr;
std::array<bool, 0xFF>         Base::Data::WmKeys = {};

bool                           Base::Settings::ShowMenu = true;

void Base::Init()
{
	//Data
	Data::d3d9dummy = std::make_unique<D3D9Dummy>();
	if (!Data::d3d9dummy->GetDevice())
	{
		Base::Message(MEM_STR("Unable to initialize D3D9 Dummy"));
		return;
	}

	Data::d3d9hk = std::make_unique<HookManager>();

	//Hooks
	mem_bool_t Check = MEM_TRUE;
	BCHECK(Check, Data::d3d9hk->HookTrampoline(Data::d3d9dummy->GetFunction(ifnEndScene), (mem_voidptr_t)Hooks::EndScene, 7));

	if (!Check)
	{
		Base::Message(MEM_STR("Unable to hook functions"));
		Data::d3d9hk->RestoreAll();
		return;
	}

	Base::Data::Loaded = true;
}

void Base::Message(mem_string_t msg)
{
	msg = MEM_STR("[CSMH] ") + msg;
	MessageBox(NULL, msg.c_str(), MEM_STR("CSMH"), MB_OK);
}

void Base::Shutdown()
{
	if (Base::Data::Loaded)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		SetWindowLongPtr(Data::window->GetHandle(), GWL_WNDPROC, (LONG)Data::oWndProc);
		Data::d3d9hk->RestoreAll();

		Base::Data::Loaded = false;
	}
}

void Base::Unload()
{
	CreateThread(nullptr, 0, ExitThread, Data::hModule, 0, nullptr);
}