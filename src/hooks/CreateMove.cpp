#include <pch.h>
#include <base.h>

bool __cdecl Base::Hooks::CreateMove(IClientMode* thisptr, float flInputSampleTime, CUserCmd* cmd)
{
	C_BasePlayer* pLocalPlayer = (C_BasePlayer*)Data::entitylist->GetClientEntity(Data::engine->GetLocalPlayer());
	if (pLocalPlayer)
	{
		Hacks::Bunnyhop(pLocalPlayer, cmd);
	}

	return reinterpret_cast<bool(__cdecl*)(IClientMode*, float, CUserCmd*)>(Data::ClientModeVMT->GetOriginal(25))(thisptr, flInputSampleTime, cmd);
}