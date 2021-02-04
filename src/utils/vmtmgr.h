#pragma once
#ifndef VMTMGR_H
#define VMTMGR_H

#include <pch.h>

class VMTManager
{
private:
	mem_voidptr_t* vmt = nullptr;
	std::map<mem_size_t, mem_voidptr_t> orig_vmt = {};
public:
	VMTManager(mem_voidptr_t* VMT);
	~VMTManager();
public:
	mem_voidptr_t GetFunction(mem_size_t Index);
	mem_bool_t    SetFunction(mem_voidptr_t Function, mem_size_t Index);
	mem_voidptr_t GetOriginal(mem_size_t Index);
	mem_bool_t    SetOriginal(mem_size_t Index);
	mem_bool_t    RestoreOriginal();
};

#endif