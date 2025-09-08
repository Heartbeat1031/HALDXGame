#pragma once
#include "Scene.h"

class WinScene : public Scene {

public:
	void Init() override;
	void Update(float dt) override;
	void Uninit() override;
};