#pragma once
#include "GameObject.h"

class BulletEmitter : public GameObject {
public:
	BulletEmitter();
	~BulletEmitter() override;
	void Init()override;
	void Update(float dt) override;
	void Uninit() override;

};