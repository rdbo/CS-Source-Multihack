#pragma once
#ifndef HOOKMGR_H
#define HOOKMGR_H

#include <pch.h>

#define GET_OFFSET(base, offset) (&((char*)base)[offset])
#define GET_MODULE_OFFSET(mod, offset) GET_OFFSET(mod.base, offset)
#define GET_PMODULE_OFFSET(pmod, offset) GET_MODULE_OFFSET(*pmod, offset)

typedef struct HookEntry
{
	mem_data_t data = nullptr;
	mem_size_t size = 0;
} HookEntry;

class HookManager
{
private:
	mem_voidptr_t BaseAddress = (mem_voidptr_t)0;
	std::map < mem_voidptr_t, HookEntry > HookTable = {};

public:
	HookManager();
	HookManager(mem_module_t& Module);
	~HookManager();

public:
	mem_bool_t    Hook(mem_voidptr_t Source, mem_voidptr_t Dest, mem_size_t Size, mem_asm_t Method = MEM_ASM_x86_JMP32);
	mem_bool_t    Hook(mem_uintptr_t Offset, mem_voidptr_t Dest, mem_size_t Size, mem_asm_t Method = MEM_ASM_x86_JMP32);
	mem_bool_t    HookTrampoline(mem_voidptr_t Source, mem_voidptr_t Dest, mem_size_t Size, mem_asm_t Method = MEM_ASM_x86_JMP32);
	mem_bool_t    HookTrampoline(mem_uintptr_t Offset, mem_voidptr_t Dest, mem_size_t Size, mem_asm_t Method = MEM_ASM_x86_JMP32);
	mem_voidptr_t GetOriginal(mem_voidptr_t Source);
	mem_voidptr_t GetOriginal(mem_uintptr_t Offset);
	mem_bool_t    Restore(mem_voidptr_t Source);
	mem_bool_t    Restore(mem_uintptr_t Offset);
	mem_bool_t    RestoreAll();
};

#endif