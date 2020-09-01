#include "Hacks.h"
#include "Offset.h"

void Hacks::EntryPoint()
{
	cout << "CSProject v1.3\n";
	while (true)
	{
		Prepare();
		skinChanger();
		//TriggerBot();
		//Bunnyhop();
		//NoFlash();
		//GlowESP();
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

void Hacks::ForceUpdate()
{
	DWORD _dwClientState = readMem<DWORD>((DWORD)engine.modBaseAddr + 0x589DD4);
	writeMem<int>(_dwClientState + 0x174, -1);
}

void Hacks::skinChanger()
{
	if (GetAsyncKeyState(VK_F2) & 1) ForceUpdate();
	int Local = readMem<int>((DWORD)client.modBaseAddr + dwLocalPlayer);
	int ActiveWeapon = readMem<int>(Local + var.m_hActiveWeapon) & 0xFFF;
	ActiveWeapon = readMem<int>((DWORD)client.modBaseAddr + dwEntityList + (ActiveWeapon - 1) * 0x10);
	short ActiveWeaponID = readMem<short>(ActiveWeapon + var.m_iItemDefinitionIndex);
	for (int i = 0; i < 8; i++)
	{
		int Weapon = readMem<int>(Local + var.m_hMyWeapons + i * 0x4) & 0xFFF;
		Weapon = readMem<int>((DWORD)client.modBaseAddr + dwEntityList + (Weapon - 1) * 0x10);
		int WeaponID = readMem<int>(Weapon + var.m_iItemDefinitionIndex);

		int PaintKit = 0;
		int EntityQuality = 0;
		float Wear = 0.0001f;
		int Seed = 0;
		int StatTrack = 1337;

		switch (WeaponID)
		{
		case 1://USP-S
			PaintKit = 527;
			break;
		case 7://AK47
			PaintKit = 675;
			break;
		case 9://AWM
			PaintKit = 344;
			break;
		case 4://Glock-18
			PaintKit = 353;
			break;
		case 16://M4A4
			PaintKit = 255;
			break;
		case 61://USP-S
			PaintKit = 504;
			break;
		case 10://FAMAS
			PaintKit = 154;
			break;
		case 17://MAC-10
			PaintKit = 433;
			break;
		case 19://P90
			PaintKit = 156;
			break;
		case 27://MAG-7
			PaintKit = 431;
			break;
		case 36://P250
			PaintKit = 464;
			break;
		case 39://SSG-08
			PaintKit = 487;
			break;
		case 60://M4A1-S
			PaintKit = 430;
			break;
		}

		if (PaintKit != 0)
		{
			if (readMem<int>(Weapon + var.m_iItemIDHigh) != -1)
				writeMem<int>(Weapon + var.m_iItemIDHigh, -1);

			writeMem<int>(Weapon + var.m_OriginalOwnerXuidLow, 0);
			writeMem<int>(Weapon + var.m_OriginalOwnerXuidHigh, 0);
			writeMem<int>(Weapon + var.m_nFallbackPaintKit, PaintKit);
			writeMem<int>(Weapon + var.m_nFallbackStatTrak, StatTrack);
			writeMem<float>(Weapon + var.m_flFallbackWear, Wear);

			/*if (StatTrack >= 0)
				writeMem<int>(Weapon + var.m_iEntityQuality, 9);
			else
				writeMem<int>(Weapon + var.m_iEntityQuality, EntityQuality);*/
		}
	}
}

Vector3 Hacks::PlayerVelocity(){
	return readMem<Vector3>(localPlayer + var.m_vecVelocity);
}

