#include	"GameMainScene.h"
#include    <thread>

/**
 * @brief GameMainScene のコンストラクタ
 *
 * カメラやスプライトなどの初期化は `init()` 内で行う。
 */
GameMainScene::GameMainScene()
{
}

/**
 * @brief 毎フレームの更新処理
 *
 * プレイヤーや弾、ボスの状態を更新し、ゲームの進行を制御する。
 * エンターキー（RETURN）が押された場合は `TitleScene` へフェード遷移する。
 *
 * @param deltatime 前フレームからの経過時間（マイクロ秒）
 */
void GameMainScene::update(uint64_t deltatime)
{
	updatebg();
	updateplayer();
	UpdatePlayerBullet();

	UpdateEnemies();
	UpdateEnemyBullet();

	CollisionCheck();

	updateexplosion();

	if (isallDead()) {
		SceneManager::SetCurrentScene("GameClearScene", std::make_unique<FadeTransition>(500.0f));

		// 3000ミリ秒後にresetallobject()を呼び出す
		std::thread([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			resetallobject();
			}).detach(); // detachしてスレッドを手放す（※用途により注意）
	}
}

/**
 * @brief 毎フレームの描画処理
 *
 * 背景、プレイヤー、ボス、弾などを描画する。
 * ゲームの視覚的な内容を画面に反映する。
 *
 * @param deltatime 前フレームからの経過時間（マイクロ秒）
 */
void GameMainScene::draw(uint64_t deltatime)
{
	drawbg();
	drawplayer();
	DrawPlayerBullet();
	DrawEnemies();
	DrawEnemyBullet();
	drawexplosion();
	CollisionDraw();
}

/**
 * @brief シーンの初期化処理
 *
 * ゲームプレイに必要な各要素（背景、プレイヤー、ボス、弾）の初期化を行う。
 * リソースの読み込みや初期位置の設定などを行う。
 */
void GameMainScene::init()
{
	initplayer();
	initbg();
	initexplosion();
	InitPlayerBullet();
	InitEnemies();
	InitEnemyBullet();
}

/**
 * @brief シーンの終了処理
 *
 * メモリの解放や状態リセットなど、シーン終了時に必要な処理を行う。
 * 他シーンへの遷移時に呼び出される。
 */
void GameMainScene::dispose()
{
	disposeplayer();
	disposebg();
	disposeexplosion();
	DisposePlayerBullet();
	DisposeEnemyBullet();
	DisposeEnemies();
}
