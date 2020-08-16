#include "Function.h"

class Process
{
public:
	template <class _R> static _R readMem(DWORD addr)
	{
		_R x;
		ReadProcessMemory(FindPattern::pHandle, (LPVOID)addr, &x, sizeof(_R), 0);
		return x;
	}
	template <class _W> static _W writeMem(DWORD addr, _W buff)
	{
		WriteProcessMemory(FindPattern::pHandle, (LPVOID)addr, &buff, sizeof(_W), 0);
		return 0;
	}
	static DWORD getProcess(const char* procName);
	static MODULEENTRY32 getModule(DWORD procId, const char* modName);
};
