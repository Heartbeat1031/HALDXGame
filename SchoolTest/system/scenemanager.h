#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <cstdint>
#include "noncopyable.h"
#include "IScene.h"
#include "SceneTransition.h"
#include "fadetransition.h"

/**
 * @brief シーン全体を管理するクラス
 *
 * シーンの登録・更新・描画・切り替えなど、ゲーム中の状態管理を行う。
 * フェードやクロスフェードなどの視覚効果付きのシーン遷移もサポートする。
 */
class SceneManager : NonCopyable {
    /// 登録されているシーンのマップ（名前→シーンインスタンス）
    static std::unordered_map<std::string, std::unique_ptr<IScene>> m_scenes;

    /// 現在有効なシーンの名前
    static std::string m_currentSceneName;

    /// 遷移演出を行うためのオブジェクト（例：フェードイン・アウト）
    static std::unique_ptr<SceneTransition> m_transition;

    /// 現在遷移中かどうか
    static bool m_isTransitioning;

    /// 遷移先のシーン名（遷移が完了したらこのシーンに切り替える）
    static std::string m_nextSceneName;

public:
    /**
     * @brief シーンをマネージャーに追加する
     * @param name シーン識別用の名前
     * @param scene 追加するシーンのインスタンス（所有権を移動）
     */
    static void AddScene(const std::string& name, std::unique_ptr<IScene> scene);

    /**
     * @brief シーンを切り替える
     * @param name 切り替える先のシーン名
     * @param transition 遷移演出（省略可）
     *
     * 即時には切り替えられず、遷移演出が終了した後にシーンが変更される。
     */
    static void SetCurrentScene(const std::string& name, std::unique_ptr<SceneTransition> transition = nullptr);

    /**
     * @brief 現在のシーンの初期化を実行する
     *
     * 通常はゲーム起動時やリセット時に使用。
     */
    static void Init();

    /**
     * @brief 現在のシーンおよび遷移演出の更新処理を行う
     * @param deltaTime 前フレームからの経過時間（マイクロ秒など）
     */
    static void Update(uint64_t deltaTime);

    /**
     * @brief 現在のシーンおよび遷移演出の描画処理を行う
     * @param deltaTime 前フレームからの経過時間（マイクロ秒など）
     */
    static void Draw(uint64_t deltaTime);

    /**
     * @brief すべてのシーンと遷移演出の終了処理を行う
     *
     * 使用終了時にシーンのリソースを明示的に破棄する。
     */
    static void Dispose();

    /**
     * @brief 遷移完了後に実際にシーンを切り替える内部関数
     * @param name 切り替えるシーン名
     *
     * 通常は外部から呼び出さない。
     */
    static void InternalChangeScene(const std::string& name);
};

