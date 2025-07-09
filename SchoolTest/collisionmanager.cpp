#include <array>
#include "system/collision.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "explosion.h"
#include "system/BoxDrawer.h"

void CollisionCheck() 
{
	// �G�ƃv���C���̒e�̓����蔻��
	std::array<Enemy,ENEMYMAX> enemies = GetEnemies();
	std::array <Bullet, PLAYERBULLETMAX> bullets = GetPlayerBullets();

	// �v���C���̒e�������[�v
	for(int pbidx = 0;pbidx<bullets.size();pbidx++)
	{
		const Bullet& pb = bullets[pbidx];

		if (pb.sts == 1)
		{
			// �v���C���̒e��AABB���擾
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

						// �����G�t�F�N�g�𔭐�������
						startexplosion(enemy.srt.pos, enemy.srt.scale);

					}
				}
			}
		}
	}
}

void CollisionDraw()
{
	// �G�ƃv���C���̒e�̓����蔻��
	std::array<Enemy, ENEMYMAX> enemies = GetEnemies();

	// �G���\��
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

	// �v���C���e���\��
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