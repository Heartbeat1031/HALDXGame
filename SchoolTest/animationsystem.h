#pragma once
#include	"system/CommonTypes.h"

// テクスチャ矩形座標（ストリップ）
struct	UVQuad {
	Vector2	tex[4];
};

// アニメーションテーブル
struct AnimationTbl {
	int	PictureNo;
	int Time;
};

// 指定したピクチャーテーブルとアニメーションテーブルを
// 使用して該当するテクスチャデータのUVを取得する
UVQuad GetPictureUV(
	int& nowindex,					// 現在使用しているアニメーションテーブルのインデックス値
	int& nowcounter,				// 現在の表示コマ数
	AnimationTbl animtbl[],			// 使用しているアニメーションテーブル
	UVQuad PictureTable[]);
