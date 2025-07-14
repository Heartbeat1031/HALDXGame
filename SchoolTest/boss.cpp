#include <array>
#include <memory>
#include "system/DebugUI.h"
#include "system/CSprite.h"
#include "main.h"
#include "boss.h"

// ボス用スプライト
static std::unique_ptr<CSprite> g_spriteboss;

// ボスの触手の位置と姿勢
static std::array<Vector3, TentacleNum> g_pos;
static std::array<Vector3, TentacleNum> g_rot;

// ボスの触手のローカルポーズ
static std::array<Matrix4x4, TentacleNum> g_localpose;
// ボスの触手のグローバルポーズ
static std::array<Matrix4x4, TentacleNum> g_globalpose;

// ボスの触手の動きを作るためのパラメータ
static float g_swingAngle = 0.0f;					// 角度
static float g_swingRange = 10.0f * PI / 180.0f;	// 触手の揺れ幅(30度)
static float g_swingDelay = 0.0f;					// 遅れ
static float g_swingAngleDelta = 0.01f;				// 足し算する角度

static void DebugTentacles()
{
	ImGui::Begin("DebugTentacles");

	ImGui::SliderFloat("X Swing Range", &g_swingRange, 0.0f, (50.0f*PI)/180.0f);
	ImGui::SliderFloat("Y Swing Delay", &g_swingDelay, 0.0f, 10.0f);
	ImGui::SliderFloat("angle delta", &g_swingAngleDelta, 0.01f, 1.0f);

	ImGui::End();
}


// 初期処理
void initboss()
{
	g_spriteboss = std::make_unique<CSprite>(
		1,
		1,
		"assets/texture/white.bmp");

	MATERIAL mtrl{};
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 0, 0, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.TextureEnable = TRUE;
	mtrl.Shiness = 0;
	g_spriteboss->ModifyMtrl(mtrl);

	g_globalpose.fill(Matrix4x4::Identity);
	g_localpose.fill(Matrix4x4::Identity);

	g_rot.fill(Vector3(0, 0, 0));
	g_pos.fill(Vector3(BOSSWIDTH+10, 0, 0));

	// ボスの先頭
	g_pos[0] = Vector3(800, 300, 0);

	DebugUI::RedistDebugFunction(DebugTentacles);

}

// 更新処理
void updateboss()
{
	std::array< SRT,TentacleNum> srt;			// SRT情報（触手毎）

	// ローカルポーズを設定
	for (int i = 0; i < TentacleNum; i++) {
		srt[i].pos = g_pos[i];
		srt[i].rot = g_rot[i];
		srt[i].scale = Vector3(1, 1, 1);
	}

	// ローカルポーズ行列を作成
	for (int i = 0; i < TentacleNum; i++) {
		g_localpose[i] = srt[i].GetMatrix();
	}

	// 親子関係を考慮しグローバルポーズ行列作る
	for (int i = 0; i < TentacleNum; i++) {
		g_globalpose[i] = g_localpose[i];

		for (int j = i-1; j >= 0; j--) {
			g_globalpose[i] = g_globalpose[i] * g_localpose[j];
		}
	}

	// スケールを設定
	Matrix4x4 smtx = Matrix4x4::CreateScale(BOSSWIDTH, BOSSHEIGHT, 1);
	for (int i = 0; i < TentacleNum; i++) {
		g_globalpose[i] = smtx * g_globalpose[i];
	}

	// 角度を足す
	g_swingAngle += g_swingAngleDelta;

	for (int i = 0; i < TentacleNum; i++) {
		g_rot[i].z = sinf(g_swingAngle - (g_swingDelay * i))*g_swingRange;
	}
}

// 描画処理
void drawboss()
{
	for (int i = 0; i < TentacleNum; i++)
	{
		g_spriteboss->Draw(g_globalpose[i]);
	}
}

// 解放処理
void disposeboss()
{

}
