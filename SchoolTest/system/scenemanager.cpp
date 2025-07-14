#include "SceneManager.h"

/// @brief 登録されたシーンの一覧
std::unordered_map<std::string, std::unique_ptr<IScene>> SceneManager::m_scenes;

/// @brief 現在アクティブなシーンの名前
std::string SceneManager::m_currentSceneName;

/// @brief 現在のシーン遷移演出
std::unique_ptr<SceneTransition> SceneManager::m_transition = nullptr;

/// @brief シーン遷移中フラグ
bool SceneManager::m_isTransitioning = false;

/// @brief 遷移後に切り替えるシーン名
std::string SceneManager::m_nextSceneName;

/**
 * @brief シーンを追加する
 * @param name シーンの名前
 * @param scene 登録するシーンインスタンス（所有権を移動）
 *
 * AddScene を使って、ゲーム開始前にすべてのシーンを登録しておく。
 */
void SceneManager::AddScene(const std::string& name, std::unique_ptr<IScene> scene) {
    m_scenes[name] = std::move(scene);
    m_scenes[name]->init();
}

/**
 * @brief 現在のシーンを切り替える
 * @param name 遷移先のシーン名
 * @param transition オプション：遷移演出（フェードなど）
 *
 * 即座に切り替えるか、`SceneTransition` を用いて遷移演出後に変更される。
 */
void SceneManager::SetCurrentScene(const std::string& name, std::unique_ptr<SceneTransition> transition) {
    if (transition) {
        m_transition = std::move(transition);
        m_transition->start(name);
        m_isTransitioning = true;
        m_nextSceneName = name;
    }
    else {
        InternalChangeScene(name);
    }
}

/**
 * @brief 登録されている全シーンを初期化する
 *
 * ゲーム起動時などに使用。すべてのシーンに対して `init()` を呼び出す。
 */
void SceneManager::Init() {
//    for (auto& [name, scene] : m_scenes) {
//        scene->init();
//    }
}

/**
 * @brief 現在のシーンおよび遷移演出の更新処理
 * @param deltaTime 前フレームからの経過時間（マイクロ秒など）
 *
 * 遷移中は `SceneTransition` の更新が優先され、完了後にシーンが切り替わる。
 */
void SceneManager::Update(uint64_t deltaTime) {
    if (m_isTransitioning && m_transition) {
        m_transition->update(deltaTime);
        if (m_transition->isFinished()) {
            InternalChangeScene(m_nextSceneName);
            m_transition = nullptr;
            m_isTransitioning = false;
        }
    }
    else if (!m_currentSceneName.empty()) {
        m_scenes[m_currentSceneName]->update(deltaTime);
    }
}

/**
 * @brief 現在のシーンと遷移演出の描画処理
 * @param deltaTime 前フレームからの経過時間（マイクロ秒など）
 *
 * 通常のシーン描画に加えて、遷移中であれば演出も重ねて描画される。
 */
void SceneManager::Draw(uint64_t deltaTime) {
    if (!m_currentSceneName.empty()) {
        m_scenes[m_currentSceneName]->draw(deltaTime);
    }
    if (m_isTransitioning && m_transition) {
        m_transition->draw();
    }
}

/**
 * @brief すべてのシーンの終了処理とリソース解放
 *
 * 登録されたシーンをすべて `dispose()` し、リストから削除する。
 */
void SceneManager::Dispose() {
    for (auto& [name, scene] : m_scenes) {
        scene->dispose();
    }
    m_scenes.clear();
}

/**
 * @brief 内部的にシーンを切り替える（演出完了後などに呼ばれる）
 * @param name 切り替えるシーンの名前
 *
 * 現在のシーンの `dispose()` を呼び、切り替え先シーンの `init()` を実行。
 */
void SceneManager::InternalChangeScene(const std::string& name) {
    if (m_scenes.count(name)) {
        if (!m_currentSceneName.empty()) {
            m_scenes[m_currentSceneName]->dispose();
        }
        m_currentSceneName = name;
//      m_scenes[m_currentSceneName]->init();
    }
}
