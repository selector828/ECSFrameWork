#pragma once

#include <ecs.h>

class Player : public Entity
{
public:
	Player(Entity * parent) : Entity(parent)
	{
		
	}
public:
	void $Update(void) override {  }
	void $Pause(void) override {  }
	void $Always(void) override {  }
};