#pragma once
#include <cstdint>

/**
 * @brief ゲームのシーン（画面）を表す基本インタフェース
 *
 * タイトル画面、メインゲーム、リザルト画面など、状態に応じたロジックや描画を管理するための抽象クラス。
 * すべてのシーンはこのインタフェースを実装する必要がある。
 */
class IScene {
public:
    /// @brief デフォルトコンストラクタ
    IScene() = default;

    /// @brief 仮想デストラクタ（継承先の安全な破棄のため）
    virtual ~IScene() = default;

    /**
     * @brief 毎フレームの更新処理
     *
     * 入力処理、キャラクターの動き、ゲームロジックなどをこの関数で記述する。
     *
     * @param delta 経過時間（マイクロ秒など）
     */
    virtual void update(uint64_t delta) = 0;

    /**
     * @brief 毎フレームの描画処理
     *
     * キャラクター、背景、UIなどの描画を行う。
     *
     * @param delta 経過時間（マイクロ秒など）
     */
    virtual void draw(uint64_t delta) = 0;

    /**
     * @brief シーンの初期化処理
     *
     * リソースの読み込み、初期配置などを行う。
     * シーンが有効になる前に一度だけ呼ばれる。
     */
    virtual void init() = 0;

    /**
     * @brief シーンの終了処理
     *
     * メモリ解放やリソースの破棄などを行う。
     * 他のシーンに遷移する前に呼ばれる。
     */
    virtual void dispose() = 0;
};
