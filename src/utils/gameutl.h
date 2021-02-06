#pragma once
#ifndef GAMEUTL_H
#define GAMEUTL_H

#include <pch.h>
#include <base.h>

class GameUtl
{
public:
	template<typename Interface_t>
	inline Interface_t* GetInterface(mem_module_t Module, mem_cstring_t InterfaceName)
	{
		Interface_t* pInterface = nullptr;
		CreateInterfaceFn pfnCreateInterface = (CreateInterfaceFn)mem::in::get_symbol(Module, "CreateInterface");
		if (pfnCreateInterface)
			pInterface = reinterpret_cast<Interface_t*>(pfnCreateInterface(InterfaceName, nullptr));

		return pInterface;
	}
};

#endif