#include "pInc.h"

class FindPattern
{
public:
	FindPattern(DWORD start, DWORD len, BYTE* pMask, char* szMask) : Base(0), Offset(0)
	{
		BYTE* data = new BYTE[len];
		unsigned long bytesMem;
		if (ReadProcessMemory(pHandle, (LPVOID)start, data, len, &bytesMem))
		{
			for (DWORD i = 0; i < len; i++)
			{
				if (DataCompare((const BYTE*)(data + i), (const BYTE*)pMask, szMask))
				{
					Base = (DWORD)(start + i);
					Offset = i;
					break;
				}
			}
		}
		delete[] data;
	}
	static HANDLE pHandle;
	DWORD Base, Offset;
private:
	bool DataCompare(const BYTE* pData, const BYTE* pMask, const char* pszMask)
	{
		for (; *pszMask; ++pData, ++pMask, ++pszMask)
		{
			if (*pszMask == '0' && *pData != *pMask)
				return false;
		}
		return (*pszMask == NULL);
	}
};

