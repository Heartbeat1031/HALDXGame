#include "main.h"
#include "titlescene.h"
#include "system/CDirectInput.h"
#include "system/scenemanager.h"
#include "GameClearScene.h"
/**
 * @brief タイトルシーンのコンストラクタ
 *
 * 
 */
GameClearScene::GameClearScene()
{
}

/**
 * @brief クリアシーンの更新処理
 *
 * Enterキー（リターンキー）が押されたら、`TitleScene` へフェードアウト遷移する。
 *
 * @param deltatime 前フレームからの経過時間（マイクロ秒）
 */
void GameClearScene::update(uint64_t deltatime)
{
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    {
        SceneManager::SetCurrentScene(
            "TitleScene",
            std::make_unique<FadeTransition>(1000.0f, FadeTransition::Mode::FadeOutOnly)
        );
    }
}

/**
 * @brief クリアシーンの描画処理
 *
 * クリア画像を画面中央に描画する。
 *
 * @param deltatime 前フレームからの経過時間（マイクロ秒）
 */
void GameClearScene::draw(uint64_t deltatime)
{
    m_image->Draw(
        Vector3(1.0f, 1.0f, 1.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f)
    );
}

/**
 * @brief タイトルシーンの初期化処理
 *
 * シーンがアクティブになる際に一度だけ呼ばれる初期化処理。
 */
void GameClearScene::init()
{
    static bool first = true;
    if (first) {
        m_image = std::make_unique<CSprite>(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            "assets/texture/gameclear.png"
        );
        first = false;
    }
}

/**
 * @brief クリアシーンの終了処理
 *
 * シーンが破棄される前に呼ばれるリソース解放処理。
 */
void GameClearScene::dispose()
{
}
