#include <memory>
#include "system/CSprite.h"
#include "system/CDirectInput.h"
#include "main.h"
#include "player.h"
#include "playerbullet.h"

std::unique_ptr<CSprite> g_sprite;

// プレイヤの位置
Vector3		g_position = { 0.0f,0.0f,0.0f };

// プレイヤの向き
// 0:下, 1:左, 2:右, 3:上
uint32_t    g_direction = 0;

// プレイヤの移動スピード
float g_playerSpeed = 5.0f;		// プレイヤの移動速度

void initplayer()
{

	std::array<Vector2, 4> uv = {
		Vector2(0.0f,			0.0f),	
		Vector2(1.0f / 3.0f,	0.0f),	
		Vector2(0.0f,			1.0f / 4.0f),	
		Vector2(1.0f / 3.0f,	1.0f / 4.0f),
	};

	g_sprite = std::make_unique<CSprite>(
		100,
		100,
		"assets/texture/dora01.png",uv);

}

void updateplayer()
{
	bool keyinputflag = false;
	Vector3	velocity = { 0.0f,0.0f,0.0f };		// 移動速度

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_RIGHT))
	{
		velocity.x += 1.0f;
		keyinputflag = true;
		g_direction = 2;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_LEFT))
	{
		velocity.x -= 1.0f;
		keyinputflag = true;
		g_direction = 1;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_UP))
	{
		velocity.y -= 1.0f;
		keyinputflag = true;
		g_direction = 3;
	}

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_DOWN))
	{
		velocity.y += 1.0f;
		keyinputflag = true;
		g_direction = 0;
	}

	if (keyinputflag) {
		// 速度を正規化
		velocity.Normalize();

		// 位置を変更
		g_position.x += velocity.x * g_playerSpeed;
		g_position.y += velocity.y * g_playerSpeed;
		g_position.z += velocity.z * g_playerSpeed;
	}

	Vector2 uv[4][3][4] = {
	{
		{
			Vector2(0.0f,			0.0f),
			Vector2(1.0f / 3.0f,	0.0f),
			Vector2(0.0f,			1.0f / 4.0f),
			Vector2(1.0f / 3.0f,	1.0f / 4.0f)
		},
		{
			Vector2(1.0f / 3.0f,	0.0f),
			Vector2(2.0f / 3.0f,	0.0f),
			Vector2(1.0f / 3.0f,	1.0f / 4.0f),
			Vector2(2.0f / 3.0f,	1.0f / 4.0f)
		},
		{
			Vector2(2.0f / 3.0f,	0.0f),
			Vector2(3.0f / 3.0f,	0.0f),
			Vector2(2.0f / 3.0f,	1.0f / 4.0f),
			Vector2(3.0f / 3.0f,	1.0f / 4.0f)
		}
	},
	{
		{
			Vector2(0.0f,			1.0f / 4.0f),
			Vector2(1.0f / 3.0f,	1.0f / 4.0f),
			Vector2(0.0f,			2.0f / 4.0f),
			Vector2(1.0f / 3.0f,	2.0f / 4.0f)
		},
		{
			Vector2(1.0f / 3.0f,	1.0f / 4.0f),
			Vector2(2.0f / 3.0f,	1.0f / 4.0f),
			Vector2(1.0f / 3.0f,	2.0f / 4.0f),
			Vector2(2.0f / 3.0f,	2.0f / 4.0f)
		},
		{
			Vector2(2.0f / 3.0f,	1.0f / 4.0f),
			Vector2(3.0f / 3.0f,	1.0f / 4.0f),
			Vector2(2.0f / 3.0f,	2.0f / 4.0f),
			Vector2(3.0f / 3.0f,	2.0f / 4.0f)
		}
	},
	{
		{
			Vector2(0.0f,			2.0f / 4.0f),
			Vector2(1.0f / 3.0f,	2.0f / 4.0f),
			Vector2(0.0f,			3.0f / 4.0f),
			Vector2(1.0f / 3.0f,	3.0f / 4.0f)
		},
		{
			Vector2(1.0f / 3.0f,	2.0f / 4.0f),
			Vector2(2.0f / 3.0f,	2.0f / 4.0f),
			Vector2(1.0f / 3.0f,	3.0f / 4.0f),
			Vector2(2.0f / 3.0f,	3.0f / 4.0f)
		},
		{
			Vector2(2.0f / 3.0f,	2.0f / 4.0f),
			Vector2(3.0f / 3.0f,	2.0f / 4.0f),
			Vector2(2.0f / 3.0f,	3.0f / 4.0f),
			Vector2(3.0f / 3.0f,	3.0f / 4.0f)
		}
	},
	{
		{
			Vector2(0.0f,			3.0f / 4.0f),
			Vector2(1.0f / 3.0f,	3.0f / 4.0f),
			Vector2(0.0f,			4.0f / 4.0f),
			Vector2(1.0f / 3.0f,	4.0f / 4.0f)
		},
		{
			Vector2(1.0f / 3.0f,	3.0f / 4.0f),
			Vector2(2.0f / 3.0f,	3.0f / 4.0f),
			Vector2(1.0f / 3.0f,	4.0f / 4.0f),
			Vector2(2.0f / 3.0f,	4.0f / 4.0f)
		},
		{
			Vector2(2.0f / 3.0f,	3.0f / 4.0f),
			Vector2(3.0f / 3.0f,	3.0f / 4.0f),
			Vector2(2.0f / 3.0f,	4.0f / 4.0f),
			Vector2(3.0f / 3.0f,	4.0f / 4.0f)
		}
	}
	};

	static int animidx = 2;
	static int loopcnt = 0;
	g_sprite->ModifyUV(uv[g_direction][animidx]);

	if ((loopcnt % 10) == 9) {
		animidx++;
		animidx %= 3;
	}

	loopcnt++;

	// アニメーションの更新

	if (CDirectInput::GetInstance().CheckKeyBuffer(DIK_SPACE))
	{
		// 弾を発射
		Vector3 bulletpos = g_position;
		FirePlayerBullet(g_direction, bulletpos);
	}
}

void drawplayer()
{
	g_sprite->Draw(
		Vector3(1.0f, 1.0f, 1.0f),
		Vector3(0.0f, 0.0f, 0.0f),
		g_position
	);
}

void disposeplayer()
{

}

void resetplayer() {

}
