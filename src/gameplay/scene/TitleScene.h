#pragma once
#include "Scene.h"

class TitleScene : public Scene {

public:
	void Init() override;
	void Update() override;
	void Uninit() override;
};