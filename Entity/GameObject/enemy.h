#pragma once

#include <ecs.h>

class Enemy : public Entity
{
public:
	Enemy(Entity * parent) : Entity(parent)
	{
		
	}
public:
	void $Update(void) override {  }
	void $Pause(void) override {  }
	void $Always(void) override {  }
};