#pragma once

#include	<array>
#include	"system/CommonTypes.h"
#include	"animationsystem.h"

// 弾の最大数
constexpr int ENEMYBULLETMAX = 100;
// 弾の幅、高さ
constexpr int ENEMYBULLET_WIDTH = 10;
constexpr int ENEMYBULLET_HEIGHT = 10;
// 弾の速さ
constexpr int ENEMYBULLET_SPEED = 4;

// 生存フレーム数
constexpr int ENEMYBULLET_LIFETIME = 120;						//　120frame = 2秒

// 再発射時間間隔
constexpr int ENEMYBULLET_FIREINTERVAL = 100;					//  100ミリ秒

// プレイヤの弾構造体定義
struct EnemyBullet {
	int				sts = -1;									// ステータス（ー１：未使用　１：使用中　）

	Vector3			position = { 0.0f,0.0f,0.0f };				// 位置
	Vector3			rotation = { 0.0f,0.0f,0.0f };				// 姿勢
	Vector3			scale = { 1.0f,1.0f,1.0f };					// 拡縮
	int				direction = 0;								// 方向（上：０　下：１　左：２　右：３）
	int				speed = ENEMYBULLET_SPEED;					// 移動スピード

	Vector3			velocity = { 0.0f,0.0f,0.0f };				// 移動速度
	int				currentAnimIdx = 0;							// アニメーションテーブルのインデックス
	int				AnimCounter = 0;							// 現在の表示コマ数	

	AnimationTbl*	currentAnimTble = nullptr;					// 現在使用中のアニメーションテーブル
	int				lifetime = ENEMYBULLET_LIFETIME;			// 生存フレーム数
};

// プレイヤ弾　初期処理	
void InitEnemyBullet();
// プレイヤ弾　終了処理
void DisposeEnemyBullet();
// プレイヤ弾　更新処理
void UpdateEnemyBullet();
// プレイヤ弾　表示処理
void DrawEnemyBullet();
// プレイヤ弾　発射処理
void FireEnemyBullet(int direction, SRT srt);
void resetenemybullet();



