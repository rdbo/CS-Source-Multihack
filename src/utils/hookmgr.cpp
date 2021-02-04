#include <pch.h>
#include "hookmgr.h"

HookManager::HookManager()
{
	this->BaseAddress = (mem_voidptr_t)0;
}

HookManager::HookManager(mem_module_t& Module)
{
	this->BaseAddress = Module.base;
}

HookManager::~HookManager()
{
	this->BaseAddress = (mem_voidptr_t)0;

	for (auto Iterator : this->HookTable)
	{
		if (Iterator.second.data)
		{
			free(Iterator.second.data);
			Iterator.second.data = nullptr;
			Iterator.second.size = 0;
		}
	}
}

mem_bool_t    HookManager::Hook(mem_voidptr_t Source, mem_voidptr_t Dest, mem_size_t Size, mem_detour_t Method)
{
	mem_data_t StolenBytes = nullptr;
	if (mem::in::detour(Source, Dest, Size, Method, &StolenBytes))
	{
		this->HookTable[Source] = {};

		if (this->HookTable[Source].data)
			this->HookTable[Source].data;

		this->HookTable[Source].data = StolenBytes;
		this->HookTable[Source].size = Size;

		return MEM_TRUE;
	}

	return MEM_FALSE;
}

mem_bool_t    HookManager::Hook(mem_uintptr_t Offset, mem_voidptr_t Dest, mem_size_t Size, mem_detour_t Method)
{
	return this->Hook((mem_voidptr_t)GET_OFFSET(this->BaseAddress, Offset), Dest, Size, Method);
}

mem_bool_t    HookManager::HookTrampoline(mem_voidptr_t Source, mem_voidptr_t Dest, mem_size_t Size, mem_detour_t Method)
{
	mem_voidptr_t Trampoline = (mem_voidptr_t)MEM_BAD;
	mem_data_t    StolenBytes = nullptr;
	mem_prot_t    OldProtection = 0;

	if ((Trampoline = mem::in::detour_trampoline(Source, Dest, Size, Method, &StolenBytes)) != (mem_voidptr_t)MEM_BAD)
	{
		this->HookTable[Source] = {};

		if (this->HookTable[Source].data)
			free(this->HookTable[Source].data);

		this->HookTable[Source].data = (mem_data_t)Trampoline;
		mem::in::protect(this->HookTable[Source].data, Size + mem::in::detour_size(Method), PAGE_EXECUTE_READWRITE, &OldProtection);

		free(StolenBytes);

		return MEM_TRUE;
	}

	return MEM_FALSE;
}

mem_bool_t    HookManager::HookTrampoline(mem_uintptr_t Offset, mem_voidptr_t Dest, mem_size_t Size, mem_detour_t Method)
{
	return this->HookTrampoline((mem_voidptr_t)GET_OFFSET(this->BaseAddress, Offset), Dest, Size, Method);
}

mem_voidptr_t HookManager::GetOriginal(mem_voidptr_t Source)
{
	DWORD OldProtection = 0;
	HookEntry StolenBytes = this->HookTable[Source];
	mem_voidptr_t Original = StolenBytes.data;
	return Original;
}

mem_voidptr_t HookManager::GetOriginal(mem_uintptr_t Offset)
{
	return this->GetOriginal((mem_voidptr_t)GET_OFFSET(this->BaseAddress, Offset));
}

mem_bool_t    HookManager::Restore(mem_voidptr_t Source)
{
	mem_bool_t Check = MEM_FALSE;

	HookEntry StolenBytes = this->HookTable[Source];
	Check = mem::in::detour_restore(Source, StolenBytes.data, StolenBytes.size);

	return Check;
}

mem_bool_t    HookManager::Restore(mem_uintptr_t Offset)
{
	return this->Restore((mem_voidptr_t)GET_OFFSET(this->BaseAddress, Offset));
}

mem_bool_t    HookManager::RestoreAll()
{
	mem_bool_t Check = MEM_TRUE;
	for (auto Iterator : this->HookTable)
		Check = this->Restore(Iterator.first) ? Check : MEM_FALSE;
	return Check;
}
