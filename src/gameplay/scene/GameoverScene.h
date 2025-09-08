#pragma once
#include "Scene.h"

class GameoverScene : public Scene {

public:
	void Init() override;
	void Update() override;
	void Uninit() override;
};