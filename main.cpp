#include <stdio.h>
#include <System\scene_system.h>
#include <Entity\Scene\main_scene.h>

void main(void)
{
	Game::AddSystem<SceneSystem>();
	
	Game::GetSystem<SceneSystem>()->SetCurrentScene<MainScene>();
	
	Game::Run();
}