#pragma once
#include	"system/CommonTypes.h"

// �e�N�X�`����`���W�i�X�g���b�v�j
struct	UVQuad {
	Vector2	tex[4];
};

// �A�j���[�V�����e�[�u��
struct AnimationTbl {
	int	PictureNo;
	int Time;
};

// �w�肵���s�N�`���[�e�[�u���ƃA�j���[�V�����e�[�u����
// �g�p���ĊY������e�N�X�`���f�[�^��UV���擾����
UVQuad GetPictureUV(
	int& nowindex,					// ���ݎg�p���Ă���A�j���[�V�����e�[�u���̃C���f�b�N�X�l
	int& nowcounter,				// ���݂̕\���R�}��
	AnimationTbl animtbl[],			// �g�p���Ă���A�j���[�V�����e�[�u��
	UVQuad PictureTable[]);
