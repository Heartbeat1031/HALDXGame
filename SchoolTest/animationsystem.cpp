#include	"animationsystem.h"

// �w�肵���s�N�`���[�e�[�u���ƃA�j���[�V�����e�[�u���̃C���f�b�N�X�l���g�p���ĉ摜��\������
UVQuad GetPictureUV(
	int& nowindex,					// ���ݎg�p���Ă���A�j���[�V�����e�[�u���̃C���f�b�N�X�l
	int& nowcounter,				// ���݂̕\���R�}��
	AnimationTbl animtbl[],			// �g�p���Ă���A�j���[�V�����e�[�u��
	UVQuad PictureTable[]) {		// �g�p����s�N�`���[�e�[�u��

	// �A�j�[���[�V�����e�[�u���̕\���R�}�����擾
	int maxtime = animtbl[nowindex].Time;

	// �\���R�}��������ɒB������@����
	if (nowcounter >= maxtime) {
		nowindex++;
		int pictureidx = animtbl[nowindex].PictureNo;

		// �擾�����C���f�b�N�X�l���`�F�b�N
		if (pictureidx == -1) {
			nowindex = 0;			// �擪��(���[�v�Đ�)
		}
		nowcounter = 0;				// �\�����Ԃ��O��
	}

	// �\���R�}���J�E���g�A�b�v
	nowcounter++;

	// �����_�̃s�N�`��INDEX�擾
	int pictureidx = animtbl[nowindex].PictureNo;

	return PictureTable[pictureidx];
}
