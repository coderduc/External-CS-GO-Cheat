#include "Hacks.h"

class data_cv : Hacks
{
public:
	DWORD base;
	data_cv(DWORD lpBase) : base(lpBase) {}
	data_cv(LPCVOID lpBase) : base((DWORD)lpBase) {}

	template <class _T>
	const data_cv& operator >> (_T& var) {
		return readMem<_T>(base);
	}

	template <class _W>
	const data_cv& operator << (_W var) {
		return writeMem<_W>(base, var);
	}
};

class FakeClass
{
public:
	FakeClass(DWORD client) : lpBase(0) {}

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