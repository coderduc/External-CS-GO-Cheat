#include "FindAddress.h"

struct Vector3
{
	float x, y, z;
};

class Hacks : public FindAddress
{
public:
	static void EntryPoint();
	static void TriggerBot();
	static void Bunnyhop();
	static void NoFlash();
	static void GlowESP();
	static void skinChanger(const short knifeIndex, const UINT knifeSkin);
	static UINT GetModelIndexByName(const char* modelName);
	static UINT GetWeaponSkin(const short Index);
	static UINT GetModelIndex(const short Index);
	static void ForceUpdate();
	static Vector3 PlayerVelocity();

};

