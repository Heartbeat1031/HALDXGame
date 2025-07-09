#include <array>
#include <memory>

#include "system/scenemanager.h"
#include "GameMainScene.h"
#include "TitleScene.h"
#include "GameClearScene.h"

/**
 * @brief ゲームの初期化処理
 *
 * タイトルシーンとメインゲームシーンを SceneManager に登録し、
 * 初期シーンとして TitleScene を設定する。
 */
void GameInit()
{
    // シーン登録
    SceneManager::AddScene("TitleScene", std::make_unique<TitleScene>());
    SceneManager::AddScene("GameMainScene", std::make_unique<GameMainScene>());
    SceneManager::AddScene("GameClearScene", std::make_unique<GameClearScene>());
    // カレントシーンをセット
    SceneManager::SetCurrentScene("TitleScene");
}

/**
 * @brief ゲーム全体の更新処理
 *
 * 現在アクティブなシーンを更新する。
 *
 * @param delta 前フレームからの経過時間（マイクロ秒）
 */
void GameUpdate(uint64_t delta)
{
    SceneManager::Update(delta);
}

/**
 * @brief ゲーム全体の描画処理
 *
 * 現在アクティブなシーンを描画する。
 *
 * @param delta 前フレームからの経過時間（マイクロ秒）
 */
void GameDraw(uint64_t delta)
{
    SceneManager::Draw(delta);
}

/**
 * @brief ゲーム終了時のリソース解放処理
 *
 * 登録されたすべてのシーンを破棄し、メモリを解放する。
 */
void GameDispose()
{
    SceneManager::Dispose();
}

