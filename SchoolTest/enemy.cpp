#include	<memory>
#include	<random>
#include	<array>
#include    "main.h"
#include	"system/CDirectInput.h"
#include	"system/CommonTypes.h"
#include	"system/CSprite.h"
#include	"system/Transform.h"
#include	"animationsystem.h"
#include	"enemy.h"
#include	"enemybullet.h"

// 敵データ
static std::array<Enemy, ENEMYMAX>		g_enemies;

// スプライト
static std::unique_ptr<CSprite> g_spr;			// スプライト	

// ドラゴン画像のテクスチャ座標定義
static UVQuad DragonPicture[12] = {
	// 手前向き
	{	Vector2(0.0f,			0.0f),	Vector2(1.0f / 3.0f,0.0f),	Vector2(0.0f,			1.0f / 4.0f),	Vector2(1.0f / 3.0f,1.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	0.0f),	Vector2(2.0f / 3.0f,0.0f),	Vector2(1.0f / 3.0f,	1.0f / 4.0f),	Vector2(2.0f / 3.0f,1.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	0.0f),	Vector2(3.0f / 3.0f,0.0f),	Vector2(2.0f / 3.0f,	1.0f / 4.0f),	Vector2(3.0f / 3.0f,1.0f / 4.0f) },

	// 左向き
	{	Vector2(0.0f,			1.0f / 4.0f),	Vector2(1.0f / 3.0f,1.0f / 4.0f),	Vector2(0.0f,		2.0f / 4.0f),	Vector2(1.0f / 3.0f,2.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	1.0f / 4.0f),	Vector2(2.0f / 3.0f,1.0f / 4.0f),	Vector2(1.0f / 3.0f,	2.0f / 4.0f),	Vector2(2.0f / 3.0f,2.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	1.0f / 4.0f),	Vector2(3.0f / 3.0f,1.0f / 4.0f),	Vector2(2.0f / 3.0f,	2.0f / 4.0f),	Vector2(3.0f / 3.0f,2.0f / 4.0f) },

	// 右向き
	{	Vector2(0.0f,			2.0f / 4.0f),	Vector2(1.0f / 3.0f,2.0f / 4.0f),	Vector2(0.0f,		3.0f / 4.0f),	Vector2(1.0f / 3.0f,3.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	2.0f / 4.0f),	Vector2(2.0f / 3.0f,2.0f / 4.0f),	Vector2(1.0f / 3.0f,	3.0f / 4.0f),	Vector2(2.0f / 3.0f,3.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	2.0f / 4.0f),	Vector2(3.0f / 3.0f,2.0f / 4.0f),	Vector2(2.0f / 3.0f,	3.0f / 4.0f),	Vector2(3.0f / 3.0f,3.0f / 4.0f) },

	// 奥向き
	{	Vector2(0.0f,			3.0f / 4.0f),	Vector2(1.0f / 3.0f,3.0f / 4.0f),	Vector2(0.0f,		4.0f / 4.0f),	Vector2(1.0f / 3.0f,4.0f / 4.0f) },
	{	Vector2(1.0f / 3.0f,	3.0f / 4.0f),	Vector2(2.0f / 3.0f,3.0f / 4.0f),	Vector2(1.0f / 3.0f,	4.0f / 4.0f),	Vector2(2.0f / 3.0f,4.0f / 4.0f) },
	{	Vector2(2.0f / 3.0f,	3.0f / 4.0f),	Vector2(3.0f / 3.0f,3.0f / 4.0f),	Vector2(2.0f / 3.0f,	4.0f / 4.0f),	Vector2(3.0f / 3.0f,4.0f / 4.0f) }
};

// ドラゴンアニメーションテーブル(下向き)
static AnimationTbl	g_DragonAnimDOWN[]
{
	{0,10},
	{1,10},
	{2,10},
	{-1,-1}
};

// ドラゴンアニメーションテーブル(左向き)
static AnimationTbl	g_DragonAnimLEFT[]
{
	{3,10},
	{4,10},
	{5,10},
	{-1,-1}
};

// ドラゴンアニメーションテーブル(右向き)
static AnimationTbl	g_DragonAnimRIGHT[]
{
	{6,10},
	{7,10},
	{8,10},
	{-1,-1}
};

// ドラゴンアニメーションテーブル(上向き)
static AnimationTbl	g_DragonAnimUP[]
{
	{9,10},
	{10,10},
	{11,10},
	{-1,-1}
};

// アニメーションテーブル全て
static AnimationTbl* g_AnimTableAll[4]
{
	g_DragonAnimDOWN,
	g_DragonAnimLEFT,
	g_DragonAnimRIGHT,
	g_DragonAnimUP
};

// プレイヤ初期処理
static void initenemy()
{
	// ドラゴンUV座標
	std::array<Vector2,4> uvs = {
		Vector2(0,				0),
		Vector2(1.0f / 3.0f,	0),
		Vector2(0,				1.0f / 4.0f),
		Vector2(1.0f / 3.0f,	1.0f / 4.0f)
	};

	// マテリアル
	MATERIAL mtrl{};
	mtrl.Ambient = { 0.0f,0.0f,0.0f,0.0f };
	mtrl.Diffuse = { 0.0f,1.0f,0.0f,1.0f };
	mtrl.Emission = { 0.0f,0.0f,0.0f,0.0f };
	mtrl.Shiness = 0.0f;
	mtrl.Specular = { 0.0f,0.0f,0.0f,0.0f };
	mtrl.TextureEnable = TRUE;

	// ドラゴン
	g_spr = std::make_unique<CSprite>(ENEMYWIDTH, ENEMYHEIGHT, "assets/texture/dora01.png", uvs);

	g_spr->ModifyMtrl(mtrl);	// マテリアルをセット
}

static void disposeenemy(int idx)
{

}

static void updateenemy(int idx) 
{
	if (g_enemies[idx].sts < 0) return;	// ステータスが未使用なら何もしない)

	static std::random_device rd;				// 乱数生成器
	static std::mt19937 mt(rd());				// メルセンヌ・ツイスタ法

	static std::uniform_real_distribution<float> speed(1, 4);
	static std::uniform_int_distribution<int> dir(0, 3);
	static std::uniform_int_distribution<int> counter(10, 20);

	bool keyinputflag = false;
	Vector3	velocity = { 0.0f,0.0f,0.0f };		// 移動速度

	Enemy& enemy = g_enemies[idx];

	enemy.srt.pos += (enemy.velocity * enemy.speed);	// スプライトの位置を更新する

	enemy.currentAnimTbl = g_AnimTableAll[enemy.direction];

	std::bernoulli_distribution b(0.08f);				// 8%の確率で弾発射

	if (b(mt))
	{
		FireEnemyBullet(enemy.direction, enemy.srt);
	}
}

static void drawenemy(int idx)
{
	Enemy& enemy = g_enemies[idx];

	if (enemy.sts < 0) return;	// ステータスが未使用なら何もしない

	// 指定したピクチャーテーブルとアニメーションテーブルのインデックス値を使用して画像を表示する
	UVQuad nowuv;
	nowuv = GetPictureUV(
		enemy.currentAnimIdx,		// 現在使用しているアニメーションテーブルのインデックス値
		enemy.AnimCounter,			// 現在の表示コマ数
		enemy.currentAnimTbl,		// 使用しているアニメーションテーブル
		DragonPicture);

	g_spr->ModifyUV(nowuv.tex);
	g_spr->Draw(enemy.srt.scale, enemy.srt.rot, enemy.srt.pos);
}


void InitEnemies()
{

	std::random_device rd;										// 乱数生成器
	std::mt19937 mt(rd());										// メルセンヌ・ツイスタ法
	std::uniform_real_distribution<float> randX(0, SCREEN_WIDTH);
	std::uniform_real_distribution<float> randY(0, SCREEN_HEIGHT);
	std::uniform_int_distribution<int> randanimcounter(0, 2);

	std::normal_distribution<float> d(	1.0f,					// 平均1.0
										0.3f);					// 標準偏差0.3	

	initenemy();

	// 敵情報の初期化
	resetenemies();

}

void UpdateEnemies()
{
	// 敵情報の初期化
	for (int i = 0; i < ENEMYMAX; i++)
	{
		updateenemy(i);
	}
}

void DrawEnemies()
{
	// 敵情報の初期化
	for (int i = 0; i < ENEMYMAX; i++)
	{
		drawenemy(i);
	}
}

void DisposeEnemies()
{
	// 敵情報の初期化
	for (int i = 0; i < ENEMYMAX; i++)
	{
		disposeenemy(i);
	}
}

std::array<Enemy, ENEMYMAX>& GetEnemies() {
	return g_enemies;
}

void changeenemystatus(int idx, int sts)
{
	g_enemies[idx].sts = sts;		// ステータス変更
}

// すべて敵が死にましたか？
bool isallDead() {

	bool sts = true;

	for (auto& e : g_enemies) 
	{
		if (e.sts != -1) {
			sts = false;
			break;
		}
	}

	return sts;
}

// すべて敵が死にましたか？
void resetenemies() 
{

	std::random_device rd;										// 乱数生成器
	std::mt19937 mt(rd());										// メルセンヌ・ツイスタ法
	std::uniform_real_distribution<float> randX(0, SCREEN_WIDTH);
	std::uniform_real_distribution<float> randY(0, SCREEN_HEIGHT);
	std::uniform_int_distribution<int> randanimcounter(0, 2);

	std::normal_distribution<float> d(1.0f,					// 平均1.0
		0.3f);					// 標準偏差0.3	

	// 敵情報の初期化
	for (auto& e : g_enemies)
	{
		e.sts = 1;						// ステータス（ー１：未使用　１：使用中　）	
		e.direction = 1;				// 方向
		e.speed = 0;					// 移動スピード
		e.velocity = Vector3(0, 0, 0);	// 移動速度

		e.srt.pos.x = randX(mt);		// スプライトの位置
		e.srt.pos.y = randY(mt);		// スプライトの位置
		e.srt.pos.z = 0.0f;

		e.srt.rot = Vector3(0, 0, 0);
		e.srt.scale.x = e.srt.scale.y = e.srt.scale.z = d(mt);		// スプライトの拡大率	

		// アニメーション情報
		e.currentAnimIdx = 0;					// アニメーションテーブルのインデックス
		e.currentAnimTbl = g_AnimTableAll[0];	// 現在使用中のアニメーションテーブル
		e.AnimCounter = randanimcounter(mt);	// 表示コマ数

		e.movecount = 0;						// 移動カウント
	}
}