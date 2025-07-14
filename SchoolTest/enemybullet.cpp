#include	<iostream>
#include    <memory>
#include	"system/CSprite.h"
#include    "animationsystem.h"
#include    "enemybullet.h"

// �X�v���C�g
static std::unique_ptr<CSprite> g_bulletsprite;		// �X�v���C�g	

// �e�\���̔z��
static EnemyBullet g_EnemyBullet[ENEMYBULLETMAX];	// �v���C���e�\����

// �e�摜�̃e�N�X�`�����W��`
static UVQuad BulletPicture[3] = 
{
	// ��O����
	{	Vector2(0.0f,			96.0f / 256.0f),Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(0.0f,			128.0f / 256.0f),Vector2(32.0f / 256.0f,	128.0f / 256.0f) },
	{	Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(32.0f / 256.0f,	127.0f / 256.0f),Vector2(64.0f / 256.0f,	127.0f / 256.0f) },
	{	Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(96.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	128.0f / 256.0f),Vector2(96.0f / 256.0f,	128.0f / 256.0f) }
};

// �e�A�j���[�V�����e�[�u��
static AnimationTbl	g_EnemyAnimBullet[]
{
	{0,4},
	{1,4},
	{2,4},
	{-1,-1}
};

// �A�j���[�V�����e�[�u���S��
static AnimationTbl* g_AnimTableAll
{
	g_EnemyAnimBullet
};

// �e�\���̂�����������
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

// �����ɍ��킹���p�x���Z�b�g
void SetRotation(int direction, EnemyBullet& pb) 
{

	Vector3 rotation[4] =
	{
		{0,0, 90.0f * PI / 180.0f},			// ��
		{0,0,180.0f * PI / 180.0f},			// ��
		{0,0,0},							// �E
		{ 0,0,-90.0f * PI / 180.0f }		// ��
	};

	// �p�����Z�b�g
	pb.rotation = rotation[direction];
}

// �����ɍ��킹�����x�x�N�g�����Z�b�g����
void SetVelocity(int direction, EnemyBullet& eb) 
{

	Vector3 velocity[4] =
	{
		{ 0,					ENEMYBULLET_SPEED,		0	},			// ��
		{-ENEMYBULLET_SPEED,	0,						0	},			// ��
		{ ENEMYBULLET_SPEED,	0,						0	},			// �E
		{ 0,					-ENEMYBULLET_SPEED,		0	}			// ��
	};

	eb.velocity = velocity[direction];

}

// �v���C���e�@��������
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

// �v���C���e�@�I������
void DisposeEnemyBullet() 
{
}

// �v���C���e�@�X�V����
void UpdateEnemyBullet() 
{

	// �g�p���̒e�����X�V����
	for (int i = 0; i < ENEMYBULLETMAX; i++) {

		EnemyBullet& eb = g_EnemyBullet[i];
		if (eb.sts > 0) {

			// �ʒu�̍X�V
			eb.position.x += eb.velocity.x;
			eb.position.y += eb.velocity.y;
			eb.position.z += eb.velocity.z;

			// �������Ԃ̍X�V
			eb.lifetime--;
			if (eb.lifetime <= 0) {
				eb.sts = -1;
			}
		}
	}
}

// �v���C���e�@�\������
void DrawEnemyBullet() 
{

	// �e�`��
	for (int i = 0; i < ENEMYBULLETMAX; i++) {

		EnemyBullet& eb = g_EnemyBullet[i];

		// �g�p���̒e�̂ݕ`��
		if (eb.sts >= 0) {
			// �w�肵���s�N�`���[�e�[�u���ƃA�j���[�V�����e�[�u���̃C���f�b�N�X�l���g�p���ĉ摜��\������
			UVQuad nowuv;
			nowuv = GetPictureUV(
				eb.currentAnimIdx,		// ���ݎg�p���Ă���A�j���[�V�����e�[�u���̃C���f�b�N�X�l
				eb.AnimCounter,			// ���݂̕\���R�}��
				eb.currentAnimTble,		// �g�p���Ă���A�j���[�V�����e�[�u��
				BulletPicture);

			g_bulletsprite->ModifyUV(nowuv.tex);

			g_bulletsprite->Draw(eb.scale,eb.rotation,eb.position);	// �X�v���C�g�`��	

		}
	}
}

// �G�e�@���ˏ���
void FireEnemyBullet(int direction, SRT srt) {

	static DWORD lastFireTime = 0;

	// ���ݎ������擾����
	DWORD nowtime = timeGetTime();

	if ((nowtime - lastFireTime) <= ENEMYBULLET_FIREINTERVAL) {
//		std::cout << nowtime - lastFireTime << std::endl;
		return;
	}

	// ���g�p�̒e���擾
	for (int i = 0; i < ENEMYBULLETMAX; i++) {

		EnemyBullet& eb = g_EnemyBullet[i];
		if (eb.sts < 0) {
			// ����������
			eb = InitializeBulletStruct();

			// �g�p���ɂ���
			eb.sts = 1;

			// �p�x���Z�b�g
			SetRotation(direction, eb);

			// ���x���Z�b�g
			SetVelocity(direction, eb);

			// ���˕������Z�b�g
			eb.direction = direction;

			// ���ˈʒu���Z�b�g
			eb.position = srt.pos;

			// �X�P�[�����Z�b�g
			eb.scale = srt.scale;	// �X�v���C�g�̃X�P�[�����Z�b�g	

			// �ŏI���ˎ�����ۑ�
			lastFireTime = nowtime;

			break;
		}
	}
}

void resetenemybullet() {

	// �e��S�Ė��g�p�ɂ���
	for (int i = 0; i < ENEMYBULLETMAX; i++) {
		EnemyBullet& eb = g_EnemyBullet[i];

		// ����������
		eb = InitializeBulletStruct();

		// ���g�p�ɂ���
		eb.sts = -1;
	}
}
