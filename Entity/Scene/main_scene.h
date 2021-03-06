#pragma once

#include <ecs.h>
#include <Entity\GameObject\player.h>
#include <Entity\GameObject\enemy.h>

class MainScene : public Entity
{
public:
	MainScene(Entity * parent) : Entity(parent)
	{
		
		this->AddChild<Player>();
		this->AddChild<Enemy>();
		this->AddChild<Player>();
	}

public:
	void $Update(void) override {  }
	void $Pause(void) override {  }
	void $Always(void) override {  }
};