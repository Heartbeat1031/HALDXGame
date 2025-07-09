#include	"animationsystem.h"

// 指定したピクチャーテーブルとアニメーションテーブルのインデックス値を使用して画像を表示する
UVQuad GetPictureUV(
	int& nowindex,					// 現在使用しているアニメーションテーブルのインデックス値
	int& nowcounter,				// 現在の表示コマ数
	AnimationTbl animtbl[],			// 使用しているアニメーションテーブル
	UVQuad PictureTable[]) {		// 使用するピクチャーテーブル

	// アニーメーションテーブルの表示コマ数を取得
	int maxtime = animtbl[nowindex].Time;

	// 表示コマ数が上限に達したら　次へ
	if (nowcounter >= maxtime) {
		nowindex++;
		int pictureidx = animtbl[nowindex].PictureNo;

		// 取得したインデックス値をチェック
		if (pictureidx == -1) {
			nowindex = 0;			// 先頭へ(ループ再生)
		}
		nowcounter = 0;				// 表示時間を０へ
	}

	// 表示コマ数カウントアップ
	nowcounter++;

	// 現時点のピクチャINDEX取得
	int pictureidx = animtbl[nowindex].PictureNo;

	return PictureTable[pictureidx];
}
