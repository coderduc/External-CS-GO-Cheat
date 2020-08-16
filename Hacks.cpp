#include "Hacks.h"
#include "Offset.h"

void Hacks::EntryPoint()
{
	while (true)
	{
		Prepare();
		TriggerBot();
	}
}

void Hacks::TriggerBot()
{
	int id = readMem<int>(localPlayer + var.m_iCrosshairId);
	int myTeam = readMem<int>(localPlayer + var.m_iTeamNum);
	if (id > 0 && id < 64)
	{
		DWORD pEntity = readMem<DWORD>((DWORD)client.modBaseAddr + sig.dwEntityList + (id - 1) * 0x10);
		int iTeamNum = readMem<int>(pEntity + var.m_iTeamNum);
		if ((iTeamNum != myTeam) && (iTeamNum != 0) && GetAsyncKeyState(VK_MENU))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(0.5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}
}
