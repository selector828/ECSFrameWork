#pragma once

#include <ecs.h>
#include <windows.h>

class SceneSystem final: public System
{
public:
	~SceneSystem(void)
	{
		delete this->current_scene_;
	}

private:
	Entity * current_scene_ = nullptr;
	bool pause_ = false;

private:
	virtual void $Begin(void) override
	{

	}
	virtual void $Run(void) override
	{
		(this->pause_) ? this->current_scene_->Pause() : this->current_scene_->Update();

		this->current_scene_->Always();
	}
	virtual void $End(void) override
	{

	}
public:
	template<class T, class ... Args> T * const SetCurrentScene(Args ... args)
	{
		delete this->current_scene_;
		return static_cast<T*>(this->current_scene_ = new T(nullptr, args ...));
	}
};