#include <pch.h>
#include <base.h>

static bool bInit = false;

HRESULT __stdcall Base::Hooks::EndScene(IDirect3DDevice9* thisptr)
{
	if (!bInit)
	{
		if (!Data::d3d9_dev)
			Data::d3d9_dev = thisptr;

		D3DDEVICE_CREATION_PARAMETERS d3dpp = {};
		Data::d3d9_dev->GetCreationParameters(&d3dpp);

		if (!Data::window)
			Data::window = std::make_unique<WindowData>(d3dpp.hFocusWindow);

		//WndProc Hook
		if (!Data::oWndProc)
			Data::oWndProc = (WNDPROC)SetWindowLongPtr(Data::window->GetHandle(), GWL_WNDPROC, (LONG)Hooks::WndProc);

		//Init ImGui
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Data::window->GetHandle());
		ImGui_ImplDX9_Init(Data::d3d9_dev);

		bInit = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Settings::ShowMenu)
	{
		ImGui::Begin("Counter-Strike: Source - Multihack by rdbo");
		ImGui::Checkbox("Bunnyhop", &Settings::EnableBunnyhop);
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return reinterpret_cast<HRESULT(__stdcall*)(IDirect3DDevice9*)>(Data::d3d9_hook->GetOriginal(Data::d3d9_dummy->GetFunction(ifnEndScene)))(thisptr);
}