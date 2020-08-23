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
	
	static Vector3 PlayerVelocity();

};

