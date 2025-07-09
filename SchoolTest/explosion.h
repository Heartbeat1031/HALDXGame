#pragma once

#include	"system/Transform.h"
#include	"system/CommonTypes.h"
#include	"animationsystem.h"

// �����̍ő吔
constexpr int EXPLOSIONMAX = 100;
// �����̕��A����
constexpr int EXPLOSION_WIDTH = 100;
constexpr int EXPLOSION_HEIGHT = 100;

// �����t���[����
constexpr int EXPLOSION_LIFETIME = 30;						//�@120frame = 2�b

// �����\���̒�`
struct Explosion {
	int				sts = -1;								// �X�e�[�^�X�i�[�P�F���g�p�@�P�F�g�p���@�j

	Vector3			position = { 0.0f,0.0f,0.0f };			// �ʒu
	Vector3			rotation = { 0.0f,0.0f,0.0f };			// �p��
	Vector3			scale = { 1.0f,1.0f,1.0f };				// �g�k

	int				currentAnimIdx = 0;						// �A�j���[�V�����e�[�u���̃C���f�b�N�X
	int				AnimCounter = 0;						// ���݂̕\���R�}��	

	AnimationTbl* currentAnimTble = nullptr;				// ���ݎg�p���̃A�j���[�V�����e�[�u��
	int				lifetime = EXPLOSION_LIFETIME;			// �����t���[����
};

// �����@��������	
void initexplosion();
// �����@�I������
void disposeexplosion();
// �����@�X�V����
void updateexplosion();
// �����@�\������
void drawexplosion();
// �����J�n
void startexplosion(Vector3 pos, Vector3 scale, int dir = 0);
// �������Z�b�g
void resetexplosion();

