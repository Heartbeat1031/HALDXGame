#pragma once

#include	"system/Transform.h"
#include	"system/CommonTypes.h"
#include	"animationsystem.h"

// 爆発の最大数
constexpr int EXPLOSIONMAX = 100;
// 爆発の幅、高さ
constexpr int EXPLOSION_WIDTH = 100;
constexpr int EXPLOSION_HEIGHT = 100;

// 生存フレーム数
constexpr int EXPLOSION_LIFETIME = 30;						//　120frame = 2秒

// 爆発構造体定義
struct Explosion {
	int				sts = -1;								// ステータス（ー１：未使用　１：使用中　）

	Vector3			position = { 0.0f,0.0f,0.0f };			// 位置
	Vector3			rotation = { 0.0f,0.0f,0.0f };			// 姿勢
	Vector3			scale = { 1.0f,1.0f,1.0f };				// 拡縮

	int				currentAnimIdx = 0;						// アニメーションテーブルのインデックス
	int				AnimCounter = 0;						// 現在の表示コマ数	

	AnimationTbl* currentAnimTble = nullptr;				// 現在使用中のアニメーションテーブル
	int				lifetime = EXPLOSION_LIFETIME;			// 生存フレーム数
};

// 爆発　初期処理	
void initexplosion();
// 爆発　終了処理
void disposeexplosion();
// 爆発　更新処理
void updateexplosion();
// 爆発　表示処理
void drawexplosion();
// 爆発開始
void startexplosion(Vector3 pos, Vector3 scale, int dir = 0);
// 爆発リセット
void resetexplosion();

