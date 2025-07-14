#include	<iostream>
#include    <memory>
#include    <array>
#include    "animationsystem.h"
#include    "explosion.h"
#include    "playerbullet.h"
#include    "system/CSprite.h"

// �X�v���C�g
static std::unique_ptr<CSprite> g_bulletsprite;					// �X�v���C�g	

// �e�\���̔z��
static std::array<Bullet, PLAYERBULLETMAX> g_PlayerBullets;		// �v���C���e�\����

// �e�摜�̃e�N�X�`�����W��`
static UVQuad BulletPicture[3] =
{
	{	Vector2(0.0f,			96.0f / 256.0f),Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(0.0f,			128.0f / 256.0f),Vector2(32.0f / 256.0f,	128.0f / 256.0f) },
	{	Vector2(32.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(32.0f / 256.0f,	127.0f / 256.0f),Vector2(64.0f / 256.0f,	127.0f / 256.0f) },
	{	Vector2(64.0f / 256.0f,	96.0f / 256.0f),Vector2(96.0f / 256.0f,	96.0f / 256.0f),Vector2(64.0f / 256.0f,	128.0f / 256.0f),Vector2(96.0f / 256.0f,	128.0f / 256.0f) }
};

// �e�A�j���[�V�����e�[�u��
static AnimationTbl	g_PlayerAnimBullet[]
{
	{0,4},
	{1,4},
	{2,4},
	{-1,-1}
};

// �A�j���[�V�����e�[�u���S��
static AnimationTbl* g_AnimTableAll
{
	g_PlayerAnimBullet
};

// �e�\���̂�����������
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

// �����ɍ��킹���p�x���Z�b�g
static void SetRotation(int direction, Bullet& pb)
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
static void SetVelocity(int direction, Bullet& pb)
{
	// �����ɍ��킹�����x�x�N�g��
	Vector3 velocity[4] =
	{
		{0,						PLAYERBULLET_SPEED,		0},			// ��
		{-PLAYERBULLET_SPEED,	0,						0},			// ��
		{ PLAYERBULLET_SPEED,	0,						0},			// �E
		{ 0,					-PLAYERBULLET_SPEED,	0 }			// ��
	};

	pb.velocity = velocity[direction];

}

// �v���C���e�@��������
void InitPlayerBullet()
{
	g_bulletsprite = std::make_unique<CSprite>(
		PLAYERBULLET_WIDTH, PLAYERBULLET_HEIGHT, 
		"assets/texture/GranadaTexture.png");

	// �e��S�Ė��g�p�ɂ���
	resetplayerbullet();
}

// �v���C���e�@�I������
void DisposePlayerBullet()
{
}

// �v���C���e�@�X�V����
void UpdatePlayerBullet() {

	// �g�p���̒e�����X�V����
	for (int i = 0; i < PLAYERBULLETMAX; i++) {

		Bullet& pb = g_PlayerBullets[i];
		if (pb.sts > 0) {

			// �ʒu�̍X�V
			pb.position.x += pb.velocity.x;
			pb.position.y += pb.velocity.y;
			pb.position.z += pb.velocity.z;

			// �������Ԃ̍X�V
			pb.lifetime--;
			if (pb.lifetime <= 0) {
				pb.sts = -1;
			}
		}
	}
}

// �v���C���e�@�\������
void DrawPlayerBullet()
{
	// �e�`��
	for (auto& pb : g_PlayerBullets)
	{
		// �g�p���̒e�̂ݕ`��
		if (pb.sts > 0) {
			// �w�肵���s�N�`���[�e�[�u���ƃA�j���[�V�����e�[�u���̃C���f�b�N�X�l���g�p���ĉ摜��\������
			UVQuad nowuv;
			nowuv = GetPictureUV(
				pb.currentAnimIdx,		// ���ݎg�p���Ă���A�j���[�V�����e�[�u���̃C���f�b�N�X�l
				pb.AnimCounter,			// ���݂̕\���R�}��
				pb.currentAnimTble,		// �g�p���Ă���A�j���[�V�����e�[�u��
				BulletPicture);

			g_bulletsprite->ModifyUV(nowuv.tex);						// UV���W�X�V

			g_bulletsprite->Draw(pb.scale, pb.rotation, pb.position);	// �X�v���C�g�`��	

		}
	}
}

// �v���C���e�@���ˏ���
void FirePlayerBullet(int direction, Vector3 pos) {

	static DWORD lastFireTime = 0;

	// ���ݎ������擾����
	DWORD nowtime = timeGetTime();

	if ((nowtime - lastFireTime) <= PLAYERBULLET_FIREINTERVAL) {
		//		std::cout << nowtime - lastFireTime << std::endl;
		return;
	}

	// ���g�p�̒e���擾
	for (int i = 0; i < PLAYERBULLETMAX; i++) {

		Bullet& pb = g_PlayerBullets[i];
		if (pb.sts < 0) {
			// ����������
			pb = InitializeBulletStruct();

			// �g�p���ɂ���
			pb.sts = 1;

			// �p�x���Z�b�g
			SetRotation(direction, pb);

			// ���x���Z�b�g
			SetVelocity(direction, pb);

			// ���˕������Z�b�g
			pb.direction = direction;

			// ���ˈʒu���Z�b�g
			pb.position = pos;

			// �ŏI���ˎ�����ۑ�
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
	g_PlayerBullets[idx].sts = sts;		// �X�e�[�^�X�ύX
}

void resetplayerbullet() {

	// �e��S�Ė��g�p�ɂ���
	for (auto& pb : g_PlayerBullets)
	{

		// ����������
		pb = InitializeBulletStruct();

		// ���g�p�ɂ���
		pb.sts = -1;
	}
}