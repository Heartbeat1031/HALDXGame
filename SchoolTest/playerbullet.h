#pragma once

#include	"system/CommonTypes.h"
#include	"animationsystem.h"

// �e�̍ő吔
constexpr int PLAYERBULLETMAX = 100;
// �e�̕��A����
constexpr int PLAYERBULLET_WIDTH = 30;
constexpr int PLAYERBULLET_HEIGHT = 30;
// �e�̑���
constexpr int PLAYERBULLET_SPEED = 8;

// �����t���[����
constexpr int PLAYERBULLET_LIFETIME = 120;						//�@120frame = 2�b

// �Ĕ��ˎ��ԊԊu
constexpr int PLAYERBULLET_FIREINTERVAL = 100;					//  100�~���b

// �v���C���̒e�\���̒�`
struct Bullet {
	int				sts = -1;								// �X�e�[�^�X�i�[�P�F���g�p�@�P�F�g�p���@�j

	Vector3			position = { 0.0f,0.0f,0.0f };			// �ʒu
	Vector3			rotation = { 0.0f,0.0f,0.0f };			// �p��
	Vector3			scale = { 1.0f,1.0f,1.0f };				// �g�k
	int				direction = 0;							// �����i��F�O�@���F�P�@���F�Q�@�E�F�R�j
	int				speed = PLAYERBULLET_SPEED;				// �ړ��X�s�[�h

	Vector3			velocity = { 0.0f,0.0f,0.0f };			// �ړ����x
	int				currentAnimIdx = 0;						// �A�j���[�V�����e�[�u���̃C���f�b�N�X
	int				AnimCounter = 0;						// ���݂̕\���R�}��	

	AnimationTbl*	currentAnimTble = nullptr;				// ���ݎg�p���̃A�j���[�V�����e�[�u��
	int				lifetime = PLAYERBULLET_LIFETIME;		// �����t���[����
};

// �v���C���e�@��������	
void InitPlayerBullet();
// �v���C���e�@�I������
void DisposePlayerBullet();
// �v���C���e�@�X�V����
void UpdatePlayerBullet();
// �v���C���e�@�\������
void DrawPlayerBullet();
// �v���C���e�@���ˏ���
void FirePlayerBullet(int direction, Vector3 pos);

// �v���C���e�@�擾
const std::array <Bullet, PLAYERBULLETMAX>& GetPlayerBullets();

// �v���C���e�̏�Ԃ�ύX����
void changeplayerbulletstatus(int idx, int sts);

// �v���C���̒e�����Z�b�g
void resetplayerbullet();

