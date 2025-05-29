#include	"system/renderer.h"
#include    "system/DebugUI.h"
#include	"scenemanager.h"
#include	"fpscontrol.h"

void gameinit() 
{
	// レンダラの初期化
	Renderer::Init();

	// シーンマネージャの初期化
	SceneManager::Init();

	// デバッグUIの初期化
	DebugUI::Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

}

void gameupdate(uint64_t deltatime)
{
	// シーンマネージャの更新
	SceneManager::Update(deltatime);

}

void gamedraw(uint64_t deltatime) 
{
	// レンダリング前処理
	Renderer::Begin();

	// シーンマネージャの描画
	SceneManager::Draw(deltatime);

	// デバッグUIの描画
	DebugUI::Render();

	// レンダリング後処理
	Renderer::End();
}

void gamedispose() 
{
	// デバッグUIの終了処理
	DebugUI::DisposeUI();

	// シーンマネージャの終了処理
	SceneManager::Dispose();	// デバッグUIの描画

	// レンダラの終了処理
	Renderer::Uninit();

}

void gameloop()
{
	uint64_t delta_time = 0;

	// フレームの待ち時間を計算する
	static FPS fpsrate(65);

	// 前回実行されてからの経過時間を計算する
	delta_time = fpsrate.CalcDelta();

	// 更新処理、描画処理を呼び出す
	gameupdate(delta_time);
	gamedraw(delta_time);

	// 規定時間までWAIT
	fpsrate.Tick();
}