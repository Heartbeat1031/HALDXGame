#pragma once
#include <cstdint>

/**
 * @brief �Q�[���̃V�[���i��ʁj��\����{�C���^�t�F�[�X
 *
 * �^�C�g����ʁA���C���Q�[���A���U���g��ʂȂǁA��Ԃɉ��������W�b�N��`����Ǘ����邽�߂̒��ۃN���X�B
 * ���ׂẴV�[���͂��̃C���^�t�F�[�X����������K�v������B
 */
class IScene {
public:
    /// @brief �f�t�H���g�R���X�g���N�^
    IScene() = default;

    /// @brief ���z�f�X�g���N�^�i�p����̈��S�Ȕj���̂��߁j
    virtual ~IScene() = default;

    /**
     * @brief ���t���[���̍X�V����
     *
     * ���͏����A�L�����N�^�[�̓����A�Q�[�����W�b�N�Ȃǂ����̊֐��ŋL�q����B
     *
     * @param delta �o�ߎ��ԁi�}�C�N���b�Ȃǁj
     */
    virtual void update(uint64_t delta) = 0;

    /**
     * @brief ���t���[���̕`�揈��
     *
     * �L�����N�^�[�A�w�i�AUI�Ȃǂ̕`����s���B
     *
     * @param delta �o�ߎ��ԁi�}�C�N���b�Ȃǁj
     */
    virtual void draw(uint64_t delta) = 0;

    /**
     * @brief �V�[���̏���������
     *
     * ���\�[�X�̓ǂݍ��݁A�����z�u�Ȃǂ��s���B
     * �V�[�����L���ɂȂ�O�Ɉ�x�����Ă΂��B
     */
    virtual void init() = 0;

    /**
     * @brief �V�[���̏I������
     *
     * ����������⃊�\�[�X�̔j���Ȃǂ��s���B
     * ���̃V�[���ɑJ�ڂ���O�ɌĂ΂��B
     */
    virtual void dispose() = 0;
};
