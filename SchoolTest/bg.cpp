#include <memory>
#include "system/CSprite.h"
#include "main.h"
#include "bg.h"

std::unique_ptr<CSprite> g_spritebg;

void initbg()
{
	
	// ドラゴンUV座標
	std::array<Vector2, 4> uvs = {
		Vector2(0,					0),
		Vector2(1280.0f / 2480.0f,	0),
		Vector2(0,					1.0f ),
		Vector2(1280.0f / 2480.0f,	1.0f)
	};

	g_spritebg = std::make_unique<CSprite>(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		"assets/texture/haikei.jpg",uvs);

	// haikei.jpg　2048,768のサイズ
	//   tex1 (0,0) tex2(1280/2048,0)
	//   tex3 (0,1) tex4(1280/2048,1)

}

void updatebg() 
{
	constexpr static float scrollspd = 0.001f;
	static uint64_t loopcnt = 0;

	// 背景
	std::array<Vector2, 4> uvs = {
		Vector2(0+scrollspd*loopcnt	,						0),
		Vector2(1280.0f / 2480.0f + scrollspd * loopcnt,	0),
		Vector2(0 + scrollspd * loopcnt,					1.0f),
		Vector2(1280.0f / 2480.0f + scrollspd * loopcnt,	1.0f)
	};

	g_spritebg->ModifyUV(uvs.data());

	loopcnt++;
}

void drawbg() 
{
	g_spritebg->Draw(
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f)
	);
}

void disposebg() 
{
}
