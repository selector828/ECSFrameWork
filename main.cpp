#include <stdio.h>
#include "ecs.h"
#include "scene_manager.h"
#include "enemy.h"
#include "player.h"

class MainScene : public Entity
{
public:
	MainScene(void)
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

void main(void)
{
	printf("%d\n", _window::width_<int>);

	Game::AddManager<SceneManager>();
	
	Game::GetManager<SceneManager>()->SetCurrentScene<MainScene>();
	
	Game::Run();
}