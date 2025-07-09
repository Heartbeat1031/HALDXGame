#include	<iostream>
#include    <memory>
#include    <array>
#include    "animationsystem.h"
#include    "explosion.h"
#include    "playerbullet.h"
#include    "system/CSprite.h"

// スプライト
static std::unique_ptr<CSprite> g_bulletsprite;					// スプライト	

// 弾構造体配列
static std::array<Bullet, PLAYERBULLETMAX> g_PlayerBullets;		// プレイヤ弾構造体

// 弾画像のテクスチャ座標定義
static UVQuad BulletPicture[3] =
{
	{	Vector2(0.0f,			96.0f / 256.0f),Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(0.0f,			128.0f / 256.0f),Vector2(32.0f / 256.0f,	128.0f / 256.0f) },
	{	Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(32.0f / 256.0f,	127.0f / 256.0f),Vector2(64.0f / 256.0f,	127.0f / 256.0f) },
	{	Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(96.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	128.0f / 256.0f),Vector2(96.0f / 256.0f,	128.0f / 256.0f) }
};

// 弾アニメーションテーブル
static AnimationTbl	g_PlayerAnimBullet[]
{
	{0,4},
	{1,4},
	{2,4},
	{-1,-1}
};

// アニメーションテーブル全て
static AnimationTbl* g_AnimTableAll
{
	g_PlayerAnimBullet
};

// 弾構造体を初期化する
static Bullet InitializeBulletStruct()
{
	Bullet obj;

	obj.sts = -1;
	obj.position = Vector3(0, 0, 0);
	obj.rotation = Vector3(0, 0, 0);
	obj.scale = Vector3(1, 1, 1);
	obj.direction = 0;
	obj.speed = PLAYERBULLET_SPEED;
	obj.velocity = Vector3(0, 0, 0);
	obj.currentAnimIdx = 0;
	obj.currentAnimTble = g_PlayerAnimBullet;
	obj.lifetime = PLAYERBULLET_LIFETIME;

	return obj;
}

// 方向に合わせた角度をセット
static void SetRotation(int direction, Bullet& pb)
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
static void SetVelocity(int direction, Bullet& pb)
{
	// 方向に合わせた速度ベクトル
	Vector3 velocity[4] =
	{
		{0,						PLAYERBULLET_SPEED,		0},			// 下
		{-PLAYERBULLET_SPEED,	0,						0},			// 左
		{ PLAYERBULLET_SPEED,	0,						0},			// 右
		{ 0,					-PLAYERBULLET_SPEED,	0 }			// 上
	};

	pb.velocity = velocity[direction];

}

// プレイヤ弾　初期処理
void InitPlayerBullet()
{
	g_bulletsprite = std::make_unique<CSprite>(
		PLAYERBULLET_WIDTH, PLAYERBULLET_HEIGHT, 
		"assets/texture/GranadaTexture.png");

	// 弾を全て未使用にする
	resetplayerbullet();
}

// プレイヤ弾　終了処理
void DisposePlayerBullet()
{
}

// プレイヤ弾　更新処理
void UpdatePlayerBullet() {

	// 使用中の弾情報を更新する
	for (int i = 0; i < PLAYERBULLETMAX; i++) {

		Bullet& pb = g_PlayerBullets[i];
		if (pb.sts > 0) {

			// 位置の更新
			pb.position.x += pb.velocity.x;
			pb.position.y += pb.velocity.y;
			pb.position.z += pb.velocity.z;

			// 生存時間の更新
			pb.lifetime--;
			if (pb.lifetime <= 0) {
				pb.sts = -1;
			}
		}
	}
}

// プレイヤ弾　表示処理
void DrawPlayerBullet()
{
	// 弾描画
	for (auto& pb : g_PlayerBullets)
	{
		// 使用中の弾のみ描画
		if (pb.sts > 0) {
			// 指定したピクチャーテーブルとアニメーションテーブルのインデックス値を使用して画像を表示する
			UVQuad nowuv;
			nowuv = GetPictureUV(
				pb.currentAnimIdx,		// 現在使用しているアニメーションテーブルのインデックス値
				pb.AnimCounter,			// 現在の表示コマ数
				pb.currentAnimTble,		// 使用しているアニメーションテーブル
				BulletPicture);

			g_bulletsprite->ModifyUV(nowuv.tex);						// UV座標更新

			g_bulletsprite->Draw(pb.scale, pb.rotation, pb.position);	// スプライト描画	

		}
	}
}

// プレイヤ弾　発射処理
void FirePlayerBullet(int direction, Vector3 pos) {

	static DWORD lastFireTime = 0;

	// 現在時刻を取得する
	DWORD nowtime = timeGetTime();

	if ((nowtime - lastFireTime) <= PLAYERBULLET_FIREINTERVAL) {
		//		std::cout << nowtime - lastFireTime << std::endl;
		return;
	}

	// 未使用の弾を取得
	for (int i = 0; i < PLAYERBULLETMAX; i++) {

		Bullet& pb = g_PlayerBullets[i];
		if (pb.sts < 0) {
			// 初期化する
			pb = InitializeBulletStruct();

			// 使用中にする
			pb.sts = 1;

			// 角度をセット
			SetRotation(direction, pb);

			// 速度をセット
			SetVelocity(direction, pb);

			// 発射方向をセット
			pb.direction = direction;

			// 発射位置をセット
			pb.position = pos;

			// 最終発射時刻を保存
			lastFireTime = nowtime;

			break;
		}
	}
}

const std::array <Bullet, PLAYERBULLETMAX>& GetPlayerBullets()
{
	return g_PlayerBullets;
}

void changeplayerbulletstatus(int idx, int sts)
{
	g_PlayerBullets[idx].sts = sts;		// ステータス変更
}

void resetplayerbullet() {

	// 弾を全て未使用にする
	for (auto& pb : g_PlayerBullets)
	{

		// 初期化する
		pb = InitializeBulletStruct();

		// 未使用にする
		pb.sts = -1;
	}
}