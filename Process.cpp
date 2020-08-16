#include "Process.h"

HANDLE FindPattern::pHandle = NULL;

DWORD Process::getProcess(const char* procName)
{
	DWORD pid = 0;
	HANDLE pProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (pProcess != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 entry;
		entry.dwSize = sizeof(entry);
		do
		{
			if (!_stricmp(entry.szExeFile, procName))
			{
				pid = entry.th32ProcessID;
				CloseHandle(pProcess);
				FindPattern::pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
			}
		} while (Process32Next(pProcess,&entry));
	}
	return pid;
}

MODULEENTRY32 Process::getModule(DWORD procId, const char* modName)
{
	HANDLE hModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	MODULEENTRY32 mod;
	mod.dwSize = sizeof(mod);
	do
	{
		if (!_stricmp(mod.szModule, modName))
		{
			CloseHandle(hModule);
			break;
		}
	} while (Module32Next(hModule, &mod));
	return mod;
}