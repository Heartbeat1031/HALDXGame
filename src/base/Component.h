#pragma once

class Component
{

protected:

	class  GameObject* m_Parent = nullptr;

public:
	Component(GameObject* parent) { m_Parent = parent; }
	virtual ~Component() {}

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
};