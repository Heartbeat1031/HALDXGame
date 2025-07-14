#pragma once
#include	<array>
#include	"system/Transform.h"
#include    "system/CommonTypes.h"
#include	"animationsystem.h"

constexpr int ENEMYMAX = 20;		// �G�̍ő吔
constexpr int ENEMYWIDTH = 50;		// �G�̕�
constexpr int ENEMYHEIGHT = 50;	// �G�̍���

// Enemy�\���̒�`
struct Enemy
{
	int			sts = -1;		// �X�e�[�^�X�i�[�P�F���g�p�@�P�F�g�p���@�j
	SRT			srt;			// �p�����

	int			direction;		// ����
	float		speed;			// �ړ��X�s�[�h

	Vector3		velocity;		// �ړ����x
	int			movecount;		// �G�̈ړ��J�E���^

	// �A�j���[�V�������
	int currentAnimIdx;				// �A�j���[�V�����e�[�u���̃C���f�b�N�X
	AnimationTbl* currentAnimTbl;	// ���ݎg�p���̃A�j���[�V�����e�[�u��
	int AnimCounter;				// �\���R�}��
};

// �v���^�C�v�錾
void InitEnemies();
void UpdateEnemies();
void DrawEnemies();
void DisposeEnemies();
std::array<Enemy, ENEMYMAX>& GetEnemies();
// Enemy�̏�Ԃ�ύX����
void changeenemystatus(int idx, int sts);

// ���ׂĎ���
bool isallDead();

// ���Z�b�g
void resetenemies();