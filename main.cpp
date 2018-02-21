#include <stdio.h>
#include <ecs.h>
#include <System\window_system.h>
#include <System\d3d11_system.h>
#include <System\scene_system.h>
#include <System\input_system.h>
#include <Entity\Scene\main_scene.h>

int __stdcall WinMain(HINSTANCE, HINSTANCE, char *, int)
{
	Game::Add<WindowSystem>();
	Game::Add<D3D11System>();
	Game::Add<SceneSystem>()->SetCurrentScene<MainScene>();
	Game::Add<InputSystem>();
	Game::$Run();
}