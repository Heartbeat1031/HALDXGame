#include	<iostream>
#include    <memory>
#include	"system/CSprite.h"
#include    "animationsystem.h"
#include    "enemybullet.h"

// スプライト
static std::unique_ptr<CSprite> g_bulletsprite;		// スプライト	

// 弾構造体配列
static EnemyBullet g_EnemyBullet[ENEMYBULLETMAX];	// プレイヤ弾構造体

// 弾画像のテクスチャ座標定義
static UVQuad BulletPicture[3] = 
{
	// 手前向き
	{	Vector2(0.0f,			96.0f / 256.0f),Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(0.0f,			128.0f / 256.0f),Vector2(32.0f / 256.0f,	128.0f / 256.0f) },
	{	Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(32.0f / 256.0f,	127.0f / 256.0f),Vector2(64.0f / 256.0f,	127.0f / 256.0f) },
	{	Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(96.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	128.0f / 256.0f),Vector2(96.0f / 256.0f,	128.0f / 256.0f) }
};

// 弾アニメーションテーブル
static AnimationTbl	g_EnemyAnimBullet[]
{
	{0,4},
	{1,4},
	{2,4},
	{-1,-1}
};

// アニメーションテーブル全て
static AnimationTbl* g_AnimTableAll
{
	g_EnemyAnimBullet
};

// 弾構造体を初期化する
EnemyBullet InitializeBulletStruct() 
{
	EnemyBullet obj;

	obj.sts = -1;
	obj.position = Vector3(0, 0, 0);
	obj.rotation = Vector3(0, 0, 0);
	obj.scale = Vector3(1, 1, 1);
	obj.direction = 0;
	obj.speed = ENEMYBULLET_SPEED;
	obj.velocity = Vector3(0, 0, 0);
	obj.currentAnimIdx = 0;
	obj.currentAnimTble = g_EnemyAnimBullet;
	obj.lifetime = ENEMYBULLET_LIFETIME;

	return obj;
}

// 方向に合わせた角度をセット
void SetRotation(int direction, EnemyBullet& pb) 
{

	Vector3 rotation[4] =
	{
		{0,0, 90.0f * PI / 180.0f},			// 下
		{0,0,180.0f * PI / 180.0f},			// 左
		{0,0,0},							// 右
		{ 0,0,-90.0f * PI / 180.0f }		// 上
	};

	// 姿勢をセット
	pb.rotation = rotation[direction];
}

// 方向に合わせた速度ベクトルをセットする
void SetVelocity(int direction, EnemyBullet& eb) 
{

	Vector3 velocity[4] =
	{
		{ 0,					ENEMYBULLET_SPEED,		0	},			// 下
		{-ENEMYBULLET_SPEED,	0,						0	},			// 左
		{ ENEMYBULLET_SPEED,	0,						0	},			// 右
		{ 0,					-ENEMYBULLET_SPEED,		0	}			// 上
	};

	eb.velocity = velocity[direction];

}

// プレイヤ弾　初期処理
void InitEnemyBullet()
{

	std::array<Vector2,4> uvs= {
		Vector2(0.0f,			0.0f),
		Vector2(1.0f / 3.0f,	0.0f),
		Vector2(0.0f,			1.0f / 4.0f),
		Vector2(1.0f / 3.0f,	1.0f / 4.0f)
	};

	g_bulletsprite = std::make_unique<CSprite>
		(ENEMYBULLET_WIDTH, ENEMYBULLET_HEIGHT, "assets/texture/GranadaTexture.png", uvs);

	resetenemybullet();

}

// プレイヤ弾　終了処理
void DisposeEnemyBullet() 
{
}

// プレイヤ弾　更新処理
void UpdateEnemyBullet() 
{

	// 使用中の弾情報を更新する
	for (int i = 0; i < ENEMYBULLETMAX; i++) {

		EnemyBullet& eb = g_EnemyBullet[i];
		if (eb.sts > 0) {

			// 位置の更新
			eb.position.x += eb.velocity.x;
			eb.position.y += eb.velocity.y;
			eb.position.z += eb.velocity.z;

			// 生存時間の更新
			eb.lifetime--;
			if (eb.lifetime <= 0) {
				eb.sts = -1;
			}
		}
	}
}

// プレイヤ弾　表示処理
void DrawEnemyBullet() 
{

	// 弾描画
	for (int i = 0; i < ENEMYBULLETMAX; i++) {

		EnemyBullet& eb = g_EnemyBullet[i];

		// 使用中の弾のみ描画
		if (eb.sts >= 0) {
			// 指定したピクチャーテーブルとアニメーションテーブルのインデックス値を使用して画像を表示する
			UVQuad nowuv;
			nowuv = GetPictureUV(
				eb.currentAnimIdx,		// 現在使用しているアニメーションテーブルのインデックス値
				eb.AnimCounter,			// 現在の表示コマ数
				eb.currentAnimTble,		// 使用しているアニメーションテーブル
				BulletPicture);

			g_bulletsprite->ModifyUV(nowuv.tex);

			g_bulletsprite->Draw(eb.scale,eb.rotation,eb.position);	// スプライト描画	

		}
	}
}

// 敵弾　発射処理
void FireEnemyBullet(int direction, SRT srt) {

	static DWORD lastFireTime = 0;

	// 現在時刻を取得する
	DWORD nowtime = timeGetTime();

	if ((nowtime - lastFireTime) <= ENEMYBULLET_FIREINTERVAL) {
//		std::cout << nowtime - lastFireTime << std::endl;
		return;
	}

	// 未使用の弾を取得
	for (int i = 0; i < ENEMYBULLETMAX; i++) {

		EnemyBullet& eb = g_EnemyBullet[i];
		if (eb.sts < 0) {
			// 初期化する
			eb = InitializeBulletStruct();

			// 使用中にする
			eb.sts = 1;

			// 角度をセット
			SetRotation(direction, eb);

			// 速度をセット
			SetVelocity(direction, eb);

			// 発射方向をセット
			eb.direction = direction;

			// 発射位置をセット
			eb.position = srt.pos;

			// スケールをセット
			eb.scale = srt.scale;	// スプライトのスケールをセット	

			// 最終発射時刻を保存
			lastFireTime = nowtime;

			break;
		}
	}
}

void resetenemybullet() {

	// 弾を全て未使用にする
	for (int i = 0; i < ENEMYBULLETMAX; i++) {
		EnemyBullet& eb = g_EnemyBullet[i];

		// 初期化する
		eb = InitializeBulletStruct();

		// 未使用にする
		eb.sts = -1;
	}
}
