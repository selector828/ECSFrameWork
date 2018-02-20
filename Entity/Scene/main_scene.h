#pragma once

#include <ecs.h>
#include <Entity\GameObject\player.h>
#include <Entity\GameObject\enemy.h>

class MainScene : public Entity
{
public:
	MainScene(Entity * parent) : Entity(parent)
	{
		printf("%s\n", __FUNCTION__);
		this->AddChild<Player>();
		this->AddChild<Enemy>();
		this->AddChild<Player>();
	}

public:
	void $Update(void) override { printf("%s\n", __FUNCTION__); }
	void $Pause(void) override { printf("%s\n", __FUNCTION__); }
	void $Always(void) override { printf("%s\n", __FUNCTION__); }
};