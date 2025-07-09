#include <array>
#include "system/collision.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "explosion.h"
#include "system/BoxDrawer.h"

void CollisionCheck() 
{
	// 敵とプレイヤの弾の当たり判定
	std::array<Enemy,ENEMYMAX> enemies = GetEnemies();
	std::array <Bullet, PLAYERBULLETMAX> bullets = GetPlayerBullets();

	// プレイヤの弾数分ループ
	for(int pbidx = 0;pbidx<bullets.size();pbidx++)
	{
		const Bullet& pb = bullets[pbidx];

		if (pb.sts == 1)
		{
			// プレイヤの弾のAABBを取得
			BoundingBoxAABB aabbplayerbullet = SetAABB(
				pb.position, 
				PLAYERBULLET_WIDTH, 
				PLAYERBULLET_HEIGHT, 
				0.0f);

			for (int eidx = 0; eidx < enemies.size();eidx++) {

				const Enemy& enemy = enemies[eidx];

				if (enemy.sts == 1)
				{
					BoundingBoxAABB aabbenemy = SetAABB(
						enemy.srt.pos,
						enemy.srt.scale.x * ENEMYWIDTH,
						enemy.srt.scale.y * ENEMYHEIGHT, 0.0f);

					if (CollisionAABB(aabbenemy, aabbplayerbullet)) {

						changeplayerbulletstatus(pbidx, -1);
						changeenemystatus(eidx, -1);

						// 爆発エフェクトを発生させる
						startexplosion(enemy.srt.pos, enemy.srt.scale);

					}
				}
			}
		}
	}
}

void CollisionDraw()
{
	// 敵とプレイヤの弾の当たり判定
	std::array<Enemy, ENEMYMAX> enemies = GetEnemies();

	// 敵情報表示
	for (const auto& enemy : enemies) {
		if (enemy.sts > 0) {
			BoundingBoxAABB aabbenemy = SetAABB(
				enemy.srt.pos,
				enemy.srt.scale.x * ENEMYWIDTH,
				enemy.srt.scale.y * ENEMYHEIGHT, 0.0f);

			Color col{ 1,0,0,0.3f };

			Vector3 bbsize = { aabbenemy.max - aabbenemy.min };
			BoxDrawerDraw(
				bbsize.x,
				bbsize.y,
				bbsize.z,
				col,
				enemy.srt.pos.x,
				enemy.srt.pos.y,
				enemy.srt.pos.z);
		}
	}

	// プレイヤ弾情報表示
	std::array <Bullet, PLAYERBULLETMAX> bullets = GetPlayerBullets();
	for (const auto& pb : bullets)
	{
		if (pb.sts > 0) {
			BoundingBoxAABB aabb = SetAABB(
				pb.position,
				pb.scale.x * PLAYERBULLET_WIDTH,
				pb.scale.y * PLAYERBULLET_HEIGHT,
				0.0f);

			Color col{ 0,1,0,0.3f };

			Vector3 bbsize = { aabb.max - aabb.min };
			BoxDrawerDraw(
				bbsize.x,
				bbsize.y,
				bbsize.z,
				col,
				pb.position.x,
				pb.position.y,
				pb.position.z);
		}
	}
}