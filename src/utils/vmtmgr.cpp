#include <pch.h>
#include "vmtmgr.h"

VMTManager::VMTManager(mem_voidptr_t* VMT)
{
	this->vmt = VMT;
}

VMTManager::~VMTManager()
{
	this->vmt = nullptr;
	this->orig_vmt = {};
}

mem_voidptr_t VMTManager::GetFunction(mem_size_t Index)
{
	mem_voidptr_t IndexFunction = nullptr;
	if (!this->vmt) return IndexFunction;

	mem_prot_t OldProtection = 0;
	if (mem::in::protect(&this->vmt[Index], sizeof(mem_voidptr_t), PAGE_EXECUTE_READWRITE, &OldProtection) != MEM_TRUE)
		return IndexFunction;

	IndexFunction = this->vmt[Index];

	mem::in::protect(&this->vmt[Index], sizeof(mem_voidptr_t), OldProtection, &OldProtection);

	return IndexFunction;
}

mem_bool_t    VMTManager::SetFunction(mem_voidptr_t Function, mem_size_t Index)
{
	mem_bool_t Check = MEM_FALSE;

	if (!this->vmt) return Check;

	mem_prot_t OldProtection = 0;
	if (mem::in::protect(&this->vmt[Index], sizeof(mem_voidptr_t), PAGE_EXECUTE_READWRITE, &OldProtection) != MEM_TRUE)
		return Check;

	this->orig_vmt[Index] = this->vmt[Index];
	this->vmt[Index] = Function;

	mem::in::protect(&this->vmt[Index], sizeof(mem_voidptr_t), OldProtection, &OldProtection);

	Check = MEM_TRUE;
	return Check;
}

mem_voidptr_t VMTManager::GetOriginal(mem_size_t Index)
{
	return this->orig_vmt[Index];
}

mem_bool_t    VMTManager::SetOriginal(mem_size_t Index)
{
	mem_bool_t Check = MEM_FALSE;
	mem_voidptr_t Function = this->orig_vmt[Index];

	if (!this->vmt || !Function) return Check;

	mem_prot_t OldProtection = 0;
	if (mem::in::protect(&this->vmt[Index], sizeof(mem_voidptr_t), PAGE_EXECUTE_READWRITE, &OldProtection) != MEM_TRUE)
		return Check;

	this->vmt[Index] = Function;

	mem::in::protect(&this->vmt[Index], sizeof(mem_voidptr_t), OldProtection, &OldProtection);

	Check = MEM_TRUE;
	return Check;
}

mem_bool_t    VMTManager::RestoreOriginal()
{
	mem_bool_t Check = MEM_TRUE;

	if (!this->vmt) return MEM_FALSE;

	for(auto Iterator : this->orig_vmt)
		Check = this->SetOriginal(Iterator.first) ? Check : MEM_FALSE;

	return Check;
}