#pragma once

#include <ecs.h>

class Player : public Entity
{
public:
	Player(void)
	{
		printf("%s\n", __FUNCTION__);
	}
public:
	void $Update(void) override { printf("%s\n", __FUNCTION__); }
	void $Pause(void) override { printf("%s\n", __FUNCTION__); }
	void $Always(void) override { printf("%s\n", __FUNCTION__); }
};