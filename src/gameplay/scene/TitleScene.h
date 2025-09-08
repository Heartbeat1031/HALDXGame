#pragma once
#include "Scene.h"

class TitleScene : public Scene {
private:
	bool isStart = false;
	int startFlagIndex = 0;
public:
	void Init() override;
	void Update() override;
	void Uninit() override;
};