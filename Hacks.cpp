#include "Hacks.h"
#include "Offset.h"
#include "Classes.h"

class data_cv
{
public:
	DWORD base;
	data_cv(DWORD lpBase) : base(lpBase) {}
	data_cv(LPCVOID lpBase) : base((DWORD)lpBase) {}

	template <class _T>
	const data_cv& operator >> (_T& var) const
	{
		ReadProcessMemory(FindPattern::pHandle, (LPVOID)base, &var, sizeof(var), 0);
		return *this;
	}
	template <class _W>
	const data_cv& operator << (_W var) {
		return writeMem<_W>(base, var);
	}
};

class FakeClass
{
public:
	FakeClass(DWORD base = 0) : lpBase(base) {}

	data_cv FakeClass::operator[](DWORD address) const
	{
		return data_cv(lpBase + address);
	}

	data_cv FakeClass::operator[](LPCVOID address) const
	{
		return data_cv(lpBase + (DWORD)address);
	}
private:
	DWORD lpBase;
};

void Hacks::EntryPoint()
{
	cout << "CSProject v1.3\n";
	while (true)
	{
		Prepare();
		//NoFlash()
		skinChanger(507,561);
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
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x4)), 0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x8)), 0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0xC)), 0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x10)), 0.4);
			}
			else
			{
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x4)), 2);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x8)), 0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0xC)), 0);
				writeMem<float>((glowObject + ((glowIndx * 0x38) + 0x10)), 0.4);
			}
			writeMem<BYTE>((glowObject + ((glowIndx * 0x38) + 0x24)), 1);
			writeMem<BYTE>((glowObject + ((glowIndx * 0x38) + 0x25)), 0);
		}
	}
}

void Hacks::ForceUpdate()
{
	DWORD _dwClientState = readMem<DWORD>((DWORD)engine.modBaseAddr + 0x589DD4);
	writeMem<int>(_dwClientState + 0x174, -1);
}

UINT Hacks::GetModelIndexByName(const char* modelName)
{
	DWORD LocalPlayer;
	FakeClass engine_module = (DWORD)engine.modBaseAddr;
	FakeClass client_module = (DWORD)client.modBaseAddr;
	FakeClass cstate, nst, nsd, nsdi, nsdi_i;

	client_module[dwLocalPlayer] >> LocalPlayer;
	engine_module[dwClientState] >> cstate;
	// CClientState + 0x529C -> INetworkStringTable* m_pModelPrecacheTable
	cstate[0x529C] >> nst;
	// INetworkStringTable + 0x40 -> INetworkStringDict* m_pItems
	nst[0x40] >> nsd;
	// INetworkStringDict + 0xC -> void* m_pItems
	nsd[0xC] >> nsdi;

	for (UINT i = 0; i < 1024; i++)
	{
		nsdi[0xC + i * 0x34] >> nsdi_i;
		char str[128] = { 0 };
		nsdi_i[(DWORD)0] >> str;
		if (_stricmp(str, modelName) == 0)
		{
			return i;
		}
	}

	return 0;
}

UINT Hacks::GetModelIndex(const short itemIndex)
{
	UINT ret = 0;
	switch (itemIndex)
	{
	case WEAPON_KNIFE:
		ret = GetModelIndexByName("models/weapons/v_knife_default_ct.mdl");
		break;
	case WEAPON_KNIFE_T:
		ret = GetModelIndexByName("models/weapons/v_knife_default_t.mdl");
		break;
	case WEAPON_KNIFE_BAYONET:
		ret = GetModelIndexByName("models/weapons/v_knife_bayonet.mdl");
		break;
	case WEAPON_KNIFE_FLIP:
		ret = GetModelIndexByName("models/weapons/v_knife_flip.mdl");
		break;
	case WEAPON_KNIFE_GUT:
		ret = GetModelIndexByName("models/weapons/v_knife_gut.mdl");
		break;
	case WEAPON_KNIFE_KARAMBIT:
		ret = GetModelIndexByName("models/weapons/v_knife_karam.mdl");
		break;
	case WEAPON_KNIFE_M9_BAYONET:
		ret = GetModelIndexByName("models/weapons/v_knife_m9_bay.mdl");
		break;
	case WEAPON_KNIFE_TACTICAL:
		ret = GetModelIndexByName("models/weapons/v_knife_tactical.mdl");
		break;
	case WEAPON_KNIFE_FALCHION:
		ret = GetModelIndexByName("models/weapons/v_knife_falchion_advanced.mdl");
		break;
	case WEAPON_KNIFE_SURVIVAL_BOWIE:
		ret = GetModelIndexByName("models/weapons/v_knife_survival_bowie.mdl");
		break;
	case WEAPON_KNIFE_BUTTERFLY:
		ret = GetModelIndexByName("models/weapons/v_knife_butterfly.mdl");
		break;
	case WEAPON_KNIFE_PUSH:
		ret = GetModelIndexByName("models/weapons/v_knife_push.mdl");
		break;
	case WEAPON_KNIFE_URSUS:
		ret = GetModelIndexByName("models/weapons/v_knife_ursus.mdl");
		break;
	case WEAPON_KNIFE_GYPSY_JACKKNIFE:
		ret = GetModelIndexByName("models/weapons/v_knife_gypsy_jackknife.mdl");
		break;
	case WEAPON_KNIFE_STILETTO:
		ret = GetModelIndexByName("models/weapons/v_knife_stiletto.mdl");
		break;
	case WEAPON_KNIFE_WIDOWMAKER:
		ret = GetModelIndexByName("models/weapons/v_knife_widowmaker.mdl");
		break;
	case WEAPON_KNIFE_CSS:
		ret = GetModelIndexByName("models/weapons/v_knife_css.mdl");
		break;
	case WEAPON_KNIFE_CORD:
		ret = GetModelIndexByName("models/weapons/v_knife_cord.mdl");
		break;
	case WEAPON_KNIFE_CANIS:
		ret = GetModelIndexByName("models/weapons/v_knife_canis.mdl");
		break;
	case WEAPON_KNIFE_OUTDOOR:
		ret = GetModelIndexByName("models/weapons/v_knife_outdoor.mdl");
		break;
	case WEAPON_KNIFE_SKELETON:
		ret = GetModelIndexByName("models/weapons/v_knife_skeleton.mdl");
		break;
	default:
		break;
	}
	return ret;
}

UINT Hacks::GetWeaponSkin(const short Index)
{
	UINT PaintKit = 0;
	switch (Index)
	{
	case WEAPON_DEAGLE:
		PaintKit = 527;
		break;
	case WEAPON_AK47:
		PaintKit = 675;
		break;
	case WEAPON_AWP:
		PaintKit = 344;
		break;
	case WEAPON_GLOCK:
		PaintKit = 353;
		break;
	case WEAPON_M4A1:
		PaintKit = 255;
		break;
	case WEAPON_FAMAS:
		PaintKit = 154;
		break;
	case WEAPON_MAC10:
		PaintKit = 433;
		break;
	case WEAPON_P90:
		PaintKit = 359;
		break;
	case WEAPON_MAG7:
		PaintKit = 431;
		break;
	case WEAPON_P250:
		PaintKit = 551;
		break;
	case WEAPON_ELITE:
		PaintKit = 307;
		break;
	case WEAPON_FIVESEVEN:
		PaintKit = 464;
		break;
	case WEAPON_AUG:
		PaintKit = 167;
		break;
	case WEAPON_GALILAR:
		PaintKit = 428;
		break;
	case WEAPON_TEC9:
		PaintKit = 614;
		break;
	case WEAPON_NOVA:
		PaintKit = 358;
		break;
	case WEAPON_REVOLVER:
		PaintKit = 523;
		break;
	case WEAPON_HKP2000:
		PaintKit = 275;
		break;
	case WEAPON_SCAR20:
		PaintKit = 597;
		break;
	case WEAPON_G3SG1:
		PaintKit = 465;
		break;
	case WEAPON_M4A1_SILENCER:
		PaintKit = 548;
		break;
	case WEAPON_USP_SILENCER:
		PaintKit = 504;
		break;
	default:
		break;
	}
	return PaintKit;
}

void Hacks::skinChanger(const short knifeIndex, const UINT knifeSkin)
{
	if (GetAsyncKeyState(VK_F2) & 1) ForceUpdate();
	const int itemIDHigh = -1;
	const int EntityQuality = 3;
	const float Wear = 0.0001f;
	const int StatTrack = 1337;
	UINT ModelIndex = 0;
	while (!GetAsyncKeyState(VK_F8))
	{
		while (!ModelIndex)
		{
			ModelIndex = GetModelIndex(knifeIndex);
		}
		for (UINT i = 0; i < 8; i++)
		{
			DWORD Weapon = readMem<DWORD>(localPlayer + var.m_hMyWeapons + i * 0x4) & 0xFFF;
			Weapon = readMem<DWORD>((DWORD)client.modBaseAddr + dwEntityList + (Weapon - 1) * 0x10);
			if (!Weapon) { continue; }
			short WeaponID = readMem<short>(Weapon + var.m_iItemDefinitionIndex);
			UINT weaponSkin = GetWeaponSkin(WeaponID);

			if (WeaponID == WEAPON_KNIFE || WeaponID == WEAPON_KNIFE_T || WeaponID == knifeIndex)
			{
				writeMem<short>(Weapon + var.m_iItemDefinitionIndex, knifeIndex);
				writeMem<UINT>(Weapon + var.m_nModelIndex, ModelIndex);
				writeMem<UINT>(Weapon + var.m_iViewModelIndex, ModelIndex);
				writeMem<int>(Weapon + var.m_iEntityQuality, EntityQuality);
				weaponSkin = knifeSkin;
			}
			if (weaponSkin)
			{
				writeMem<int>(Weapon + var.m_iItemIDHigh, itemIDHigh);
				writeMem<UINT>(Weapon + var.m_nFallbackPaintKit, weaponSkin);
				writeMem<float>(Weapon + var.m_flFallbackWear, Wear);
				writeMem<int>(Weapon + var.m_nFallbackStatTrak, StatTrack);
			}
		}

		DWORD activeWeapon = readMem<DWORD>(localPlayer + var.m_hActiveWeapon) & 0xfff;
		activeWeapon = readMem<DWORD>((DWORD)client.modBaseAddr + dwEntityList + (activeWeapon - 1) * 0x10);
		if (!activeWeapon) { continue; }

		short weaponIndex = readMem<short>(activeWeapon + var.m_iItemDefinitionIndex);
		if (weaponIndex != knifeIndex) { continue; }

		DWORD knifeViewModel = readMem<DWORD>(localPlayer + var.m_hViewModel) & 0xfff;
		knifeViewModel = readMem<DWORD>((DWORD)client.modBaseAddr + dwEntityList + (knifeViewModel - 1) * 0x10);
		if (knifeViewModel == 0) { continue; }

		writeMem<UINT>(knifeViewModel + var.m_nModelIndex, ModelIndex);
	}
}

Vector3 Hacks::PlayerVelocity() {
	return readMem<Vector3>(localPlayer + var.m_vecVelocity);
}

