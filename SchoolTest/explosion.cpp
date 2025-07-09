#include	<iostream>
#include    <memory>
#include    <array>
#include    "animationsystem.h"
#include    "explosion.h"
#include    "system/renderer.h"
#include    "system/cshader.h"	
#include    "system/CTexture.h"
#include    "system/CVertexBuffer.h"
#include    "system/CIndexBuffer.h"
#include    "system/CMaterial.h"

// 爆発構造体配列
static Explosion g_explosion[EXPLOSIONMAX];				// 爆発構造体

// 爆発用シェーダ
static CShader g_explosionshader;		// 爆発用シェーダ
static CTexture g_explosiontexture;		// 爆発用テクスチャ	

// 爆発のテクスチャ座標定義
static UVQuad g_explosionpicture[16] = {
	{	Vector2(0.0f / 8.0f,		1.0f / 4.0f),		Vector2(1.0f / 8.0f,	1.0f / 4.0f),	Vector2(0.0f / 8.0f,		2.0f / 4.0f),	Vector2(1.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(1.0f / 8.0f,		1.0f / 4.0f),		Vector2(2.0f / 8.0f,	1.0f / 4.0f),	Vector2(1.0f / 8.0f,		2.0f / 4.0f),	Vector2(2.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(2.0f / 8.0f,		1.0f / 4.0f),		Vector2(3.0f / 8.0f,	1.0f / 4.0f),	Vector2(2.0f / 8.0f,		2.0f / 4.0f),	Vector2(3.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(3.0f / 8.0f,		1.0f / 4.0f),		Vector2(4.0f / 8.0f,	1.0f / 4.0f),	Vector2(3.0f / 8.0f,		2.0f / 4.0f),	Vector2(4.0f / 8.0f,	2.0f / 4.0f) },

	{	Vector2(4.0f / 8.0f,		1.0f / 4.0f),		Vector2(5.0f / 8.0f,	1.0f / 4.0f),	Vector2(4.0f / 8.0f,		2.0f / 4.0f),	Vector2(5.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(5.0f / 8.0f,		1.0f / 4.0f),		Vector2(6.0f / 8.0f,	1.0f / 4.0f),	Vector2(5.0f / 8.0f,		2.0f / 4.0f),	Vector2(6.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(6.0f / 8.0f,		1.0f / 4.0f),		Vector2(7.0f / 8.0f,	1.0f / 4.0f),	Vector2(6.0f / 8.0f,		2.0f / 4.0f),	Vector2(7.0f / 8.0f,	2.0f / 4.0f) },
	{	Vector2(7.0f / 8.0f,		1.0f / 4.0f),		Vector2(8.0f / 8.0f,	1.0f / 4.0f),	Vector2(7.0f / 8.0f,		2.0f / 4.0f),	Vector2(8.0f / 8.0f,	2.0f / 4.0f) },

	{	Vector2(0.0f / 8.0f,		2.0f / 4.0f),		Vector2(1.0f / 8.0f,	2.0f / 4.0f),	Vector2(0.0f / 8.0f,		3.0f / 4.0f),	Vector2(1.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(1.0f / 8.0f,		2.0f / 4.0f),		Vector2(2.0f / 8.0f,	2.0f / 4.0f),	Vector2(1.0f / 8.0f,		3.0f / 4.0f),	Vector2(2.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(2.0f / 8.0f,		2.0f / 4.0f),		Vector2(3.0f / 8.0f,	2.0f / 4.0f),	Vector2(2.0f / 8.0f,		3.0f / 4.0f),	Vector2(3.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(3.0f / 8.0f,		2.0f / 4.0f),		Vector2(4.0f / 8.0f,	2.0f / 4.0f),	Vector2(3.0f / 8.0f,		3.0f / 4.0f),	Vector2(4.0f / 8.0f,	3.0f / 4.0f) },

	{	Vector2(4.0f / 8.0f,		2.0f / 4.0f),		Vector2(5.0f / 8.0f,	2.0f / 4.0f),	Vector2(4.0f / 8.0f,		3.0f / 4.0f),	Vector2(5.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(5.0f / 8.0f,		2.0f / 4.0f),		Vector2(6.0f / 8.0f,	2.0f / 4.0f),	Vector2(5.0f / 8.0f,		3.0f / 4.0f),	Vector2(6.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(6.0f / 8.0f,		2.0f / 4.0f),		Vector2(7.0f / 8.0f,	2.0f / 4.0f),	Vector2(6.0f / 8.0f,		3.0f / 4.0f),	Vector2(7.0f / 8.0f,	3.0f / 4.0f) },
	{	Vector2(7.0f / 8.0f,		2.0f / 4.0f),		Vector2(8.0f / 8.0f,	2.0f / 4.0f),	Vector2(7.0f / 8.0f,		3.0f / 4.0f),	Vector2(8.0f / 8.0f,	3.0f / 4.0f) }

};

// 描画用
static CMaterial g_material{};
static CIndexBuffer g_indexbuffer;
static CVertexBuffer<VERTEX_3D> g_vertexbuffer;

static std::vector<VERTEX_3D> g_vertices = {};

// 爆発テーブル
static AnimationTbl	g_explosionanim[]{
	{ 0,2 },
	{ 1,2 },
	{ 2,2 },
	{ 3,2 },
	{ 4,2 },
	{ 5,2 },
	{ 6,2 },
	{ 7,2 },
	{ 8,2 },
	{ 9,2 },
	{ 10,2 },
	{ 11,2 },
	{ 12,2 },
	{ 13,2 },
	{ 14,2 },
	{ 15,2 },
	{ -1,-1 }
};

// アニメーションテーブル全て
static AnimationTbl* g_AnimTableAll[4]{
	g_explosionanim
};

// 弾構造体を初期化する
Explosion InitializeExplosionStruct()
{
	Explosion obj;

	obj.sts = -1;
	obj.position = Vector3(0, 0, 0);
	obj.rotation = Vector3(0, 0, 0);
	obj.scale = Vector3(1, 1, 1);
	obj.currentAnimIdx = 0;
	obj.currentAnimTble = g_explosionanim;
	obj.lifetime = EXPLOSION_LIFETIME;

	return obj;
}

void initexplosion()
{

	std::array<Vector2,4> uvs = {
		Vector2(0.0f,			0.0f),
		Vector2(1.0f / 3.0f,	0.0f),
		Vector2(0.0f,			1.0f / 4.0f),
		Vector2(1.0f / 3.0f,	1.0f / 4.0f)
	};

	// 弾を全て未使用にする
	for (int i = 0; i < EXPLOSIONMAX; i++) {
		Explosion& expl = g_explosion[i];

		// 初期化する
		expl = InitializeExplosionStruct();

		// 未使用にする
		expl.sts = -1;
	}

	// マテリアル生成
	MATERIAL mtrl;
	mtrl.Ambient = Color(0, 0, 0, 0);
	mtrl.Diffuse = Color(1, 1, 1, 1);
	mtrl.Emission = Color(0, 0, 0, 0);
	mtrl.Specular = Color(0, 0, 0, 0);
	mtrl.Shiness = 0;
	mtrl.TextureEnable = TRUE;

	g_material.Create(mtrl);

	g_explosionshader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");

	bool sts = g_explosiontexture.Load("assets/texture/explosion.png");
	assert(sts == true);

	g_vertices.clear();
	g_vertices.resize(4);

	float width = 1.0f;
	float height = 1.0f;

	g_vertices[0].Position = Vector3(-width / 2.0f, -height / 2.0f, 0);
	g_vertices[1].Position = Vector3(width / 2.0f, -height / 2.0f, 0);
	g_vertices[2].Position = Vector3(-width / 2.0f, height / 2.0f, 0);
	g_vertices[3].Position = Vector3(width / 2.0f, height / 2.0f, 0);

	for (int i = 0; i < 4; ++i) {
		g_vertices[i].Diffuse = Color(1, 1, 1, 1);
		g_vertices[i].TexCoord = g_explosionpicture[0].tex[i];
	}
	g_vertexbuffer.Create(g_vertices);

	std::vector<unsigned int> indices = { 0, 1, 2, 3 };
	g_indexbuffer.Create(indices);

}

// 方向に合わせた角度をセット
void SetRotation(int direction, Explosion& expl)
{

	Vector3 rotation[4] =
	{
		{0,0, -90.0f * PI / 180.0f},			// 上
		{0,0,  90.0f * PI / 180.0f},			// 下
		{0,0, 180.0f * PI / 180.0f},			// 左
		{0,0,0}								    // 右
	};

	// 姿勢をセット
	expl.rotation = rotation[direction];
}

void drawexplosion()
{
	// レンダリングの準備
	g_explosiontexture.SetGPU();
	g_explosionshader.SetGPU();
	g_material.SetGPU();

	Renderer::SetWorldViewProjection2D();

	// 弾描画
	for (int i = 0; i < EXPLOSIONMAX; i++) {

		Explosion& expl = g_explosion[i];

		// 使用中の弾のみ描画
		if (expl.sts >= 0) {
			// 指定したピクチャーテーブルとアニメーションテーブルのインデックス値を使用して画像を表示する
			UVQuad nowuv;
			nowuv = GetPictureUV(
				expl.currentAnimIdx,		// 現在使用しているアニメーションテーブルのインデックス値
				expl.AnimCounter,			// 現在の表示コマ数
				expl.currentAnimTble,		// 使用しているアニメーションテーブル
				g_explosionpicture);		// 使用しているピクチャーテーブル

			g_vertices[0].TexCoord = nowuv.tex[0];
			g_vertices[1].TexCoord = nowuv.tex[1];
			g_vertices[2].TexCoord = nowuv.tex[2];
			g_vertices[3].TexCoord = nowuv.tex[3];

			g_vertexbuffer.Modify(g_vertices);

			SRT srt;
			srt.scale = Vector3(EXPLOSION_WIDTH,EXPLOSION_HEIGHT,1);
			srt.rot = Vector3(0, 0, 0); // 回転は不要
			srt.pos = expl.position;

			Matrix4x4 worldmtx = srt.GetMatrix();
			Renderer::SetWorldMatrix(&worldmtx);

			ID3D11DeviceContext* devicecontext = Renderer::GetDeviceContext();
			devicecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			g_vertexbuffer.SetGPU();
			g_indexbuffer.SetGPU();

			devicecontext->DrawIndexed(
				4, // インデックス数
				0, // 開始インデックス
				0  // 頂点バッファのオフセット
			);
		}
	}
}

void updateexplosion()
{

	// 使用中の弾情報を更新する
	for (int i = 0; i < EXPLOSIONMAX; i++) {

		Explosion& expl = g_explosion[i];
		if (expl.sts > 0) {

			// 生存時間の更新
			expl.lifetime--;
			if (expl.lifetime <= 0) {
				expl.sts = -1;
			}
		}
	}
}

void disposeexplosion()
{
}

// プレイヤ弾　発射処理
void startexplosion(Vector3 pos, Vector3 scale, int dir)
{

	static DWORD lastFireTime = 0;

	// 現在時刻を取得する
	DWORD nowtime = timeGetTime();

	// 未使用の弾を取得
	for (int i = 0; i < EXPLOSIONMAX; i++) {

		Explosion& expl = g_explosion[i];
		if (expl.sts < 0) {
			// 初期化する
			expl = InitializeExplosionStruct();

			// 使用中にする
			expl.sts = 1;

			// 角度をセット
			SetRotation(dir, expl);

			// 発射位置をセット
			expl.position = pos;

			// 最終発射時刻を保存
			lastFireTime = nowtime;

			break;
		}
	}
}

void resetexplosion() 
{

	// 弾を全て未使用にする
	for (int i = 0; i < EXPLOSIONMAX; i++) {
		Explosion& expl = g_explosion[i];

		// 初期化する
		expl = InitializeExplosionStruct();

		// 未使用にする
		expl.sts = -1;
	}
}