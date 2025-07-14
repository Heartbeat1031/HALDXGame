#pragma once
#include	<array>
#include	"system/Transform.h"
#include    "system/CommonTypes.h"
#include	"animationsystem.h"

constexpr int ENEMYMAX = 20;		// 敵の最大数
constexpr int ENEMYWIDTH = 50;		// 敵の幅
constexpr int ENEMYHEIGHT = 50;	// 敵の高さ

// Enemy構造体定義
struct Enemy
{
	int			sts = -1;		// ステータス（ー１：未使用　１：使用中　）
	SRT			srt;			// 姿勢情報

	int			direction;		// 方向
	float		speed;			// 移動スピード

	Vector3		velocity;		// 移動速度
	int			movecount;		// 敵の移動カウンタ

	// アニメーション情報
	int currentAnimIdx;				// アニメーションテーブルのインデックス
	AnimationTbl* currentAnimTbl;	// 現在使用中のアニメーションテーブル
	int AnimCounter;				// 表示コマ数
};

// プロタイプ宣言
void InitEnemies();
void UpdateEnemies();
void DrawEnemies();
void DisposeEnemies();
std::array<Enemy, ENEMYMAX>& GetEnemies();
// Enemyの状態を変更する
void changeenemystatus(int idx, int sts);

// すべて死んだ
bool isallDead();

// リセット
void resetenemies();