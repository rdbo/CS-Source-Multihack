#include <pch.h>
#include <base.h>

static bool bInit = false;

HRESULT __stdcall Base::Hooks::EndScene(IDirect3DDevice9* thisptr)
{
	if (!bInit)
	{
		D3DDEVICE_CREATION_PARAMETERS d3dpp = {};
		thisptr->GetCreationParameters(&d3dpp);

		if (!Data::window)
			Data::window = std::make_unique<WindowData>(d3dpp.hFocusWindow);

		if (!Data::d3d9dev)
			Data::d3d9dev = thisptr;

		if (!Data::oWndProc)
			Data::oWndProc = (WNDPROC)SetWindowLongPtr(Data::window->GetHandle(), GWL_WNDPROC, (LONG)Hooks::WndProc);

		ImGui::CreateContext();
		ImGui_ImplWin32_Init(Data::window->GetHandle());
		ImGui_ImplDX9_Init(Data::d3d9dev);

		bInit = true;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (Settings::ShowMenu)
	{
		ImGui::Begin("ImGui Window");
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	mem_voidptr_t oEndScene = Data::d3d9hk->GetOriginal(Data::d3d9dummy->GetFunction(ifnEndScene));
	return reinterpret_cast<HRESULT(__stdcall*)(IDirect3DDevice9*)>(oEndScene)(thisptr);
}