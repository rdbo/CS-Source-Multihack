#include <pch.h>
#include <base.h>

void Base::Hacks::Bunnyhop(C_BasePlayer* pLocalPlayer, CUserCmd* pCmd)
{
	if (Settings::EnableBunnyhop)
	{
		if (pCmd->buttons & IN_JUMP && !(pLocalPlayer->GetFlags() & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;
	}
}