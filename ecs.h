#pragma once

#include <unordered_map>
#include <vector>
#include <typeinfo>

using std::vector;
using std::unordered_map;
using std::string;

class Component
{
	friend class Entity;

public:
	Component(Entity * parent) : parent_(parent) {}
	virtual ~Component(void) {}

private:
	Entity * parent_ = nullptr;

private:
	virtual void $Update(void) = 0;
	virtual void $Pause(void) = 0;
	virtual void $Always(void) = 0;

public:
	Entity * const GetParent(void)
	{
		return this->parent_;
	}
};

class Entity
{
	friend class System;

public:
	Entity(Entity * parent) : parent_(parent) {}

	virtual ~Entity(void)
	{
		for (auto component : this->components_)
			delete component.second;

		for (auto child : this->children_)
			delete child;
	}

private:
	unordered_map<string, Component*> components_;
	vector<Entity*> children_;

private:
	Entity * parent_ = nullptr;
	bool destroy_ = false;
	
private:
	virtual void $Update(void) = 0;
	virtual void $Pause(void) = 0;
	virtual void $Always(void) = 0;

public:
	void Update(void)
	{
		this->$Update();
		for (auto itr = this->components_.begin(); itr != this->components_.end();)
			((*itr).second) ? (*itr++).second->$Update() : itr = this->components_.erase(itr);
		for(auto itr = this->children_.begin(); itr != this->children_.end();)
			(!(*itr)->destroy_) ? (*itr++)->Update() : delete *(itr = this->children_.erase(itr));
	}
	void Pause(void)
	{
		this->$Pause();
		for (auto itr = this->components_.begin(); itr != this->components_.end();)
			((*itr).second) ? (*itr++).second->$Pause() : itr = this->components_.erase(itr);
		for (auto itr = this->children_.begin(); itr != this->children_.end();)
			(!(*itr)->destroy_) ? (*itr++)->Pause() : delete *(itr = this->children_.erase(itr));
	}
	void Always(void)
	{
		this->$Always();
		for (auto itr = this->components_.begin(); itr != this->components_.end();)
			((*itr).second) ? (*itr++).second->$Always() : itr = this->components_.erase(itr);
		for (auto itr = this->children_.begin(); itr != this->children_.end();)
			(!(*itr)->destroy_) ? (*itr++)->Always() : delete *(itr = this->children_.erase(itr));
	}

public:
	template<class T> T * const GetComponent(void)
	{
		auto name = typeid(T).name();
		return static_cast<T*>(this->components_[name]);
	}
	template<class T> T * const AddComponent(void)
	{
		auto name = typeid(T).name();
		this->component_[name] = new T(this);
		return static_cast<T*>(this->components_[name]);
	}
	template<class T> void RemoveComponent(void)
	{
		auto name = typeid(T).name();
		delete this->components_[name];
		this->components_[name] = nullptr;
	}
	
public:
	template<class T> void AddChild(void)
	{
		this->children_.emplace_back(new T(this));
	}

public:
	Entity * const GetParent(void)
	{
		return this->parent_;
	}
	void Destroy(void)
	{
		this->destroy_ = true;
	}
};

class System
{
	friend class Game;

public:
	virtual ~System(void) {}

private:
	virtual void $Begin(void) = 0;
	virtual void $Run(void) = 0;
	virtual void $End(void) = 0;
};

class Game
{
private:
	static unordered_map<string, System*> system_;
	static bool running_;

public:
	template<class T> static T * const GetSystem(void)
	{
		auto name = typeid(T).name();
		return static_cast<T*>(Game::system_[name]);
	}
	template<class T> static T * const AddSystem(void)
	{
		auto name = typeid(T).name();
		Game::system_[name] = new T;
		return static_cast<T*>(Game::system_[name]);
	}
	static void ShutDown(void)
	{
		Game::running_ = false;
	}
	static void Run(void)
	{
		while (Game::running_)
		{
			for (auto sys : Game::system_)
			{
				sys.second->$Begin();
				sys.second->$Run();
				sys.second->$End();
			}
		}

		for (auto sys : Game::system_)
			delete sys.second;
	}
};

unordered_map<string, System*> Game::system_ = {};
bool Game::running_ = true;