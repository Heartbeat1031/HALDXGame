#pragma once
#include "SoAStorage.h"

class Component
{
private:
	UID uid = -1;
protected:

	class  GameObject* m_Parent = nullptr;

public:
	Component(GameObject* parent) { m_Parent = parent; }
	virtual ~Component() {}
	void SetUID(UID uuid) { uid = uuid; }
	UID GetUID() const { return uid; }

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update(float dt) {};
};
