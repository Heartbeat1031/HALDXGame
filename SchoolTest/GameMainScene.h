#pragma once

#include "main.h"
#include "bg.h"
#include "player.h"
#include "playerbullet.h"
#include "boss.h"
#include "enemy.h"
#include "enemybullet.h"
#include "explosion.h"
#include "collisionmanager.h"
#include "system/camera.h"
#include "system/CDirectInput.h"
#include "system/scenemanager.h"
#include "system/fadetransition.h"
#include "system/CSprite.h"

/**
 * @brief メインゲームシーン
 *
 * 実際のゲームプレイを担当するシーン。
 * プレイヤー操作、敵出現、スコア管理などのゲーム進行ロジックと描画を行う。
 */
class GameMainScene : public IScene {
public:
	/// @brief コピーコンストラクタは禁止
	GameMainScene(const GameMainScene&) = delete;

	/// @brief 代入演算子も禁止
	GameMainScene& operator=(const GameMainScene&) = delete;

	/**
	 * @brief コンストラクタ
	 *
	 * ゲーム用のカメラやスプライト、遷移演出の初期化を行う。
	 */
	explicit GameMainScene();

	/**
	 * @brief 毎フレームの更新処理
	 * @param deltatime 前フレームからの経過時間（マイクロ秒）
	 *
	 * ゲームのロジック（キャラクターの動き、当たり判定、スコアなど）を更新する。
	 */
	void update(uint64_t deltatime) override;

	/**
	 * @brief 毎フレームの描画処理
	 * @param deltatime 前フレームからの経過時間（マイクロ秒）
	 *
	 * 背景、キャラクター、UI などの描画を行う。
	 */
	void draw(uint64_t deltatime) override;

	/**
	 * @brief シーン初期化処理
	 *
	 * ゲーム開始時のスプライト配置やリソースの読み込みを行う。
	 */
	void init() override;

	/**
	 * @brief シーン終了処理
	 *
	 * メモリの解放や状態リセットなど、シーン離脱時に必要な処理を行う。
	 */
	void dispose() override;


	/**
	 * @brief シーン内のすべてのゲームオブジェクトをリセットする
	 *
	 * 
	 */
	void resetallobject() {
		resetplayer();
		resetenemies();
		resetplayerbullet();
		resetenemybullet();
		resetexplosion();
	}

private:
	/// @brief メインカメラ（視点制御用）
	Camera m_camera;

	/// @brief シーン遷移演出（例：ゲーム終了時のフェードアウトなど）
	std::unique_ptr<SceneTransition> m_transition;

	/// @brief ゲーム内で表示される画像スプライト（背景など）
	std::unique_ptr<CSprite> m_image;
};
