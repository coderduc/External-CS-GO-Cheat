#include "Hacks.h"
#include "Offset.h"

void Hacks::EntryPoint()
{
	cout << "CSProject v1.3\n";
	while (true)
	{
		Prepare();
		TriggerBot();
		Bunnyhop();
		NoFlash();
		GlowESP();
	}
}

void Hacks::TriggerBot()
{
	int id = readMem<int>(localPlayer + var.m_iCrosshairId);
	int myTeam = readMem<int>(localPlayer + var.m_iTeamNum);
	if (id > 0 && id < 64)
	{
		DWORD pEntity = readMem<DWORD>((DWORD)client.modBaseAddr + dwEntityList + (id - 1) * 0x10);
		int iTeamNum = readMem<int>(pEntity + var.m_iTeamNum);
		if ((iTeamNum != myTeam) && (iTeamNum != 0) && GetAsyncKeyState(VK_MENU))
		{
			mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
			Sleep(0.5);
			mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		}
	}
}
void Hacks::Bunnyhop()
{
	BYTE flag = readMem<BYTE>(localPlayer + var.m_fFlags);
	if (GetAsyncKeyState(VK_SPACE) && flag && (1 << 0))
		writeMem<DWORD>((DWORD)client.modBaseAddr + dwForceJump, 6);
	Sleep(1);
}

void Hacks::NoFlash()
{
	int flashDur;
	flashDur = readMem<int>(localPlayer + var.m_flFlashDuration);
	if (flashDur > 0) writeMem<DWORD>(localPlayer + var.m_flFlashDuration, 0);
}

void Hacks::GlowESP()
{
	DWORD glowObject = readMem<DWORD>((DWORD)client.modBaseAddr + dwGlowObjectManager);
	int myTeam = readMem<int>(localPlayer + var.m_iTeamNum);

	for (short int i = 0; i < 64; i++)
	{
		DWORD entity = readMem<DWORD>((DWORD)client.modBaseAddr + dwEntityList + i * 0x10);
		if (entity != NULL)
		{
			int glowIndx = readMem<int>(entity + var.m_iGlowIndex);
			int entityTeam = readMem<int>(entity + var.m_iTeamNum);

			if (myTeam == entityTeam)
			{
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x4)),0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x8)),0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0xC)),0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x10)), 0.4);
			}
			else
			{
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x4)),2);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x8)), 0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0xC)),0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x10)),0.4);
			}
			writeMem<BYTE>((glowObject + ((glowIndx * 0x38) + 0x24)), 1);
			writeMem<BYTE>((glowObject + ((glowIndx * 0x38) + 0x25)),0);
		}
	}
}

Vector3 Hacks::PlayerVelocity(){
	return readMem<Vector3>(localPlayer + var.m_vecVelocity);
}
