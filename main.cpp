#include "Hacks.h"

int main()
{
	SetConsoleTitleA("Developed by CoderDuc");
	HANDLE hCOUT = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hCOUT, 14);
	Hacks::EntryPoint();
	system("pause");
	return 0;
}