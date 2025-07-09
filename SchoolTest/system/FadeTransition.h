#pragma once
#include "scenemanager.h"
#include "SceneTransition.h"

/**
 * @brief フェード演出によるシーン遷移を行うクラス
 *
 * モードに応じて、フェードインのみ／フェードアウトのみ／その両方の演出が可能。
 * シーン切り替え時に黒い矩形を使って画面の明暗を調整する。
 */
class FadeTransition : public SceneTransition {
public:
    /**
    * @brief フェード演出のモード
     */
    enum class Mode {
        FadeInOnly,
        FadeOutOnly,
        FadeInOut
    };

private:
    float m_alpha = 0.0f;
    float m_duration;
    uint64_t m_elapsed = 0;
    std::string m_nextScene;
    Mode m_mode;

    /**
    * @brief 現在のフェードフェーズ
    */
    enum class Phase {
        None,
        FadeOut,
        ChangeScene,
        FadeIn
    } m_phase = Phase::None;

public:
    /**
    * @brief コンストラクタ
     *
    * @param durationMs フェード時間（ミリ秒）
    * @param mode フェードモード（デフォルトは FadeInOut）
    */
    explicit FadeTransition(float durationMs, Mode mode = Mode::FadeInOut)
        : m_duration(durationMs), m_mode(mode) {
    }

    /**
     * @brief フェード演出の開始処理
    *
    * @param nextSceneName 遷移先のシーン名
    */
    void start(const std::string& nextSceneName) override;

    /**
     * @brief フェード演出の更新処理
    *
    * @param deltaTime 前フレームからの経過時間（マイクロ秒）
    */
    void update(uint64_t deltaTime) override;

    /**
    * @brief 黒フェード矩形の描画処理
    */
    void draw() override;

    /**
     * @brief フェード演出の完了判定
    *
    * @return true フェード演出が終了している
    * @return false 演出中
    */
    bool isFinished() const override;
};
