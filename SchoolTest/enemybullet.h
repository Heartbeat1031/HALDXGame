#pragma once

#include	<array>
#include	"system/CommonTypes.h"
#include	"animationsystem.h"

// �e�̍ő吔
constexpr int ENEMYBULLETMAX = 100;
// �e�̕��A����
constexpr int ENEMYBULLET_WIDTH = 10;
constexpr int ENEMYBULLET_HEIGHT = 10;
// �e�̑���
constexpr int ENEMYBULLET_SPEED = 4;

// �����t���[����
constexpr int ENEMYBULLET_LIFETIME = 120;						//�@120frame = 2�b

// �Ĕ��ˎ��ԊԊu
constexpr int ENEMYBULLET_FIREINTERVAL = 100;					//  100�~���b

// �v���C���̒e�\���̒�`
struct EnemyBullet {
	int				sts = -1;									// �X�e�[�^�X�i�[�P�F���g�p�@�P�F�g�p���@�j

	Vector3			position = { 0.0f,0.0f,0.0f };				// �ʒu
	Vector3			rotation = { 0.0f,0.0f,0.0f };				// �p��
	Vector3			scale = { 1.0f,1.0f,1.0f };					// �g�k
	int				direction = 0;								// �����i��F�O�@���F�P�@���F�Q�@�E�F�R�j
	int				speed = ENEMYBULLET_SPEED;					// �ړ��X�s�[�h

	Vector3			velocity = { 0.0f,0.0f,0.0f };				// �ړ����x
	int				currentAnimIdx = 0;							// �A�j���[�V�����e�[�u���̃C���f�b�N�X
	int				AnimCounter = 0;							// ���݂̕\���R�}��	

	AnimationTbl*	currentAnimTble = nullptr;					// ���ݎg�p���̃A�j���[�V�����e�[�u��
	int				lifetime = ENEMYBULLET_LIFETIME;			// �����t���[����
};

// �v���C���e�@��������	
void InitEnemyBullet();
// �v���C���e�@�I������
void DisposeEnemyBullet();
// �v���C���e�@�X�V����
void UpdateEnemyBullet();
// �v���C���e�@�\������
void DrawEnemyBullet();
// �v���C���e�@���ˏ���
void FireEnemyBullet(int direction, SRT srt);
void resetenemybullet();



