#include "FindAddress.h"

bool			FindAddress::isgetaddress;
DWORD		    FindAddress::dwLocalPlayer = NULL,
			    FindAddress::localPlayer = NULL;
MODULEENTRY32   FindAddress::client,
				FindAddress::engine;

bool FindAddress::Prepare()
{
	client = getModule(getProcess("csgo.exe"), "client.dll");
	if (!isgetaddress)
	{
		/*dwLocalPlayer*/ {
			dwLocalPlayer = FindPattern::FindPattern((DWORD)client.modBaseAddr, (DWORD)client.modBaseSize , (PBYTE)"\x8D\x34\x85\x00\x00\x00\x00\x89\x15\x00\x00\x00\x00\x8B\x41\x08\x8B\x48\x04\x83\xF9\xFF", "000????00????000000000").Base;
			{
				dwLocalPlayer = readMem<DWORD>(dwLocalPlayer + 0x3);
				dwLocalPlayer -= (DWORD)client.modBaseAddr;
				dwLocalPlayer += 0x4;
			}
		}
		isgetaddress = true;
	}
	localPlayer = readMem<DWORD>((DWORD)client.modBaseAddr + dwLocalPlayer);
	return 0;
}
