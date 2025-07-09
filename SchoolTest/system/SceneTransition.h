#pragma once
#include <string>

/**
 * @brief �V�[���J�ډ��o�̃C���^�[�t�F�[�X
 *
 * �t�F�[�h�C���E�t�F�[�h�A�E�g��N���X�t�F�[�h�ȂǁA���o�I�ȃV�[���؂�ւ����o�𒊏ۉ������N���X�B
 * SceneManager �ɂ���ė��p����A�V�[���̐؂�ւ����ɉ��o���������ނ��߂̊��N���X�B
 */
class SceneTransition {
public:
    /// @brief �f�t�H���g�̉��z�f�X�g���N�^�i�p���N���X�̈��S�Ȕj���̂��߁j
    virtual ~SceneTransition() = default;

    /**
     * @brief �J�ډ��o�̊J�n����
     * @param nextSceneName �J�ڐ�̃V�[����
     *
     * �J�ڂ��J�n���ꂽ�Ƃ��Ɉ�x�����Ă΂��B��������ݒ�ɗ��p�B
     */
    virtual void start(const std::string& nextSceneName) = 0;

    /**
     * @brief �J�ډ��o�̍X�V����
     * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�Ȃǁj
     *
     * �A�j���[�V�������Ԃ̍X�V�Ɏg�p����B
     */
    virtual void update(uint64_t deltaTime) = 0;

    /**
     * @brief �J�ډ��o�̕`�揈��
     *
     * �ʏ�̃V�[���`���ɏd�˂ĕ\�������B
     */
    virtual void draw() = 0;

    /**
     * @brief ���o�������������ǂ�����Ԃ�
     * @return true ���������i�V�[���؂�ւ��j
     * @return false ���o���i�܂��؂�ւ��s�j
     *
     * SceneManager �͂�����Ď����āA�V�[����؂�ւ���^�C�~���O�𐧌䂷��B
     */
    virtual bool isFinished() const = 0;
};
