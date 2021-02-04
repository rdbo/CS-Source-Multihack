#include <pch.h>
#include "d3d9dummy.h"

static HWND hWindowHolder = (HWND)NULL;

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD WindowPID = 0;
	GetWindowThreadProcessId(handle, &WindowPID);

	if (GetCurrentProcessId() != WindowPID)
		return TRUE;

	hWindowHolder = handle;
	return FALSE;
}

HWND GetProcessWindow()
{
	hWindowHolder = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return hWindowHolder;
}

D3D9Dummy::D3D9Dummy()
{
	this->d3d9_dummy = Direct3DCreate9(D3D_SDK_VERSION);

	if (!this->d3d9_dummy) return;

	this->hWindow = GetProcessWindow();

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.Windowed = false;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = this->hWindow;

	HRESULT DeviceCreated = this->d3d9_dummy->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->d3d9dev_dummy);

	if (DeviceCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;

		DeviceCreated = this->d3d9_dummy->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &this->d3d9dev_dummy);

		if (DeviceCreated != S_OK)
		{
			this->d3d9_dummy->Release();
			this->d3d9_dummy = nullptr;
			this->d3d9_dummy->Release();
			this->d3d9_dummy = nullptr;
			return;
		}
	}

	this->d3d9dev_vtable = *(mem_voidptr_t**)this->d3d9dev_dummy;
	this->d3d9dev_vmtmgr = std::make_unique<VMTManager>(this->d3d9dev_vtable);
}

D3D9Dummy::~D3D9Dummy()
{
	if (this->d3d9dev_dummy) this->d3d9dev_dummy->Release();
	if (this->d3d9_dummy)    this->d3d9_dummy->Release();

	this->d3d9dev_vtable = nullptr;
	this->hWindow = (HWND)NULL;
}

IDirect3DDevice9* D3D9Dummy::GetDevice()
{
	return this->d3d9dev_dummy;
}

mem_voidptr_t     D3D9Dummy::GetFunction(D3D9DevFnTable Index)
{
	mem_voidptr_t pD3D9Function = nullptr;

	if (Index < 0 || Index >= iD3D9DevFnTableLength || !this->d3d9dev_vmtmgr) return pD3D9Function;

	pD3D9Function = this->d3d9dev_vmtmgr->GetFunction(Index);

	return pD3D9Function;
}
