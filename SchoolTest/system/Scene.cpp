#include <memory>
#include <numbers>
#include <array>
#include "camera.h"
#include "Scene.h"
#include "renderer.h"
#include "DebugUI.h"
#include "CDirectInput.h"
#include "DirectWrite.h"
#include "SphereDrawer.h"
#include "BoxDrawer.h"
#include "CylinderDrawer.h"
#include "ConeDrawer.h"
#include "LineDrawer.h"
#include "CapsuleDrawer.h"

#include "../MyGame.h"

// DirectWriteクラスのインスタンス
static std::unique_ptr<DirectWrite> g_DirectWrite;

// カメラ
static Camera g_Camera;

void SceneInit() 
{
	Renderer::Init();

	DebugUI::Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	CDirectInput::GetInstance().Init(Application::GetHInstance(),Application::GetWindow(),Application::GetWidth(),Application::GetWidth());

	g_DirectWrite = std::make_unique<DirectWrite>();
	g_DirectWrite->Init(Renderer::GetSwapChain());

	//	カメラ初期化
	g_Camera.Init();	

	// デバッグ用のもの
	SphereDrawerInit();
	BoxDrawerInit();
	CylinderDrawerInit();
	ConeDrawerInit(true);
	LineDrawerInit();
	CapsuleDrawerInit();

	// ゲームの初期化
	GameInit();

}

void SceneUpdate(uint64_t delta) 
{
	// デバイスの状態を取得
	CDirectInput::GetInstance().GetKeyBuffer();
	CDirectInput::GetInstance().GetMouseState();

	// ゲームの更新
	GameUpdate(delta);

}

void SceneRender(uint64_t delta) 
{
	Renderer::Begin();

	g_Camera.Draw();

	// ゲームの描画
	GameDraw(delta);

	DebugUI::Render();

	Renderer::End();
}

void SceneDispose() 
{
	// ゲームの終了処理
	GameDispose();

	DebugUI::DisposeUI();
	Renderer::Uninit();

}	