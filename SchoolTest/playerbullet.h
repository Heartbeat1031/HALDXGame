#pragma once

#include	"system/CommonTypes.h"
#include	"animationsystem.h"

// 弾の最大数
constexpr int PLAYERBULLETMAX = 100;
// 弾の幅、高さ
constexpr int PLAYERBULLET_WIDTH = 30;
constexpr int PLAYERBULLET_HEIGHT = 30;
// 弾の速さ
constexpr int PLAYERBULLET_SPEED = 8;

// 生存フレーム数
constexpr int PLAYERBULLET_LIFETIME = 120;						//　120frame = 2秒

// 再発射時間間隔
constexpr int PLAYERBULLET_FIREINTERVAL = 100;					//  100ミリ秒

// プレイヤの弾構造体定義
struct Bullet {
	int				sts = -1;								// ステータス（ー１：未使用　１：使用中　）

	Vector3			position = { 0.0f,0.0f,0.0f };			// 位置
	Vector3			rotation = { 0.0f,0.0f,0.0f };			// 姿勢
	Vector3			scale = { 1.0f,1.0f,1.0f };				// 拡縮
	int				direction = 0;							// 方向（上：０　下：１　左：２　右：３）
	int				speed = PLAYERBULLET_SPEED;				// 移動スピード

	Vector3			velocity = { 0.0f,0.0f,0.0f };			// 移動速度
	int				currentAnimIdx = 0;						// アニメーションテーブルのインデックス
	int				AnimCounter = 0;						// 現在の表示コマ数	

	AnimationTbl*	currentAnimTble = nullptr;				// 現在使用中のアニメーションテーブル
	int				lifetime = PLAYERBULLET_LIFETIME;		// 生存フレーム数
};

// プレイヤ弾　初期処理	
void InitPlayerBullet();
// プレイヤ弾　終了処理
void DisposePlayerBullet();
// プレイヤ弾　更新処理
void UpdatePlayerBullet();
// プレイヤ弾　表示処理
void DrawPlayerBullet();
// プレイヤ弾　発射処理
void FirePlayerBullet(int direction, Vector3 pos);

// プレイヤ弾　取得
const std::array <Bullet, PLAYERBULLETMAX>& GetPlayerBullets();

// プレイヤ弾の状態を変更する
void changeplayerbulletstatus(int idx, int sts);

// プレイヤの弾をリセット
void resetplayerbullet();

