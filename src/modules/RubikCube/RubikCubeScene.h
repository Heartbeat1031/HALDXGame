#pragma once
#include <memory>
#include <array>
#include "system/IScene.h"
#include "system/C3DShape.h"
#include "system/DebugUI.h"
#include "Camera.h"

class RubikCubeScene : public IScene 
{
	// キューブサイズ
	static constexpr float CUBE_SIZE = 10.0f;
	// ルービックキューブ段数
	static constexpr uint8_t CUBELAYERNUM = 3;
	// 回転角度
	Vector3 m_Rotation{}; 
	// 回転行列
	Matrix4x4 m_RotationMtx{};
public:
	explicit RubikCubeScene();
	void update(uint64_t deltatime) override;
	void draw(uint64_t deltatime) override;
	void init() override;
	void dispose() override;
	void debugRubikCubeRotation();
private:
	Camera m_camera;									// 固定カメラ
	std::unique_ptr<Box> m_shapecube;					// 立方体
	std::array<std::unique_ptr<Segment>,3> m_segments;	// ローカル軸表示用線分
};
