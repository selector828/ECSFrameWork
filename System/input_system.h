#pragma once

#include <ecs.h>

class InputSystem final : public System
{
public:
	bool Press(int key)
	{
		return (GetKeyState(key) & 0x0080) ? true : false;
	}

private:
	void $Begin(void) override {};
	void $Run(void) override {};
	void $End(void) override {};
};