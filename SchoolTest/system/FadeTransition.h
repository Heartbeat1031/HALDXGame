#pragma once
#include "scenemanager.h"
#include "SceneTransition.h"

/**
 * @brief �t�F�[�h���o�ɂ��V�[���J�ڂ��s���N���X
 *
 * ���[�h�ɉ����āA�t�F�[�h�C���̂݁^�t�F�[�h�A�E�g�̂݁^���̗����̉��o���\�B
 * �V�[���؂�ւ����ɍ�����`���g���ĉ�ʂ̖��Â𒲐�����B
 */
class FadeTransition : public SceneTransition {
public:
    /**
    * @brief �t�F�[�h���o�̃��[�h
     */
    enum class Mode {
        FadeInOnly,
        FadeOutOnly,
        FadeInOut
    };

private:
    float m_alpha = 0.0f;
    float m_duration;
    uint64_t m_elapsed = 0;
    std::string m_nextScene;
    Mode m_mode;

    /**
    * @brief ���݂̃t�F�[�h�t�F�[�Y
    */
    enum class Phase {
        None,
        FadeOut,
        ChangeScene,
        FadeIn
    } m_phase = Phase::None;

public:
    /**
    * @brief �R���X�g���N�^
     *
    * @param durationMs �t�F�[�h���ԁi�~���b�j
    * @param mode �t�F�[�h���[�h�i�f�t�H���g�� FadeInOut�j
    */
    explicit FadeTransition(float durationMs, Mode mode = Mode::FadeInOut)
        : m_duration(durationMs), m_mode(mode) {
    }

    /**
     * @brief �t�F�[�h���o�̊J�n����
    *
    * @param nextSceneName �J�ڐ�̃V�[����
    */
    void start(const std::string& nextSceneName) override;

    /**
     * @brief �t�F�[�h���o�̍X�V����
    *
    * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
    */
    void update(uint64_t deltaTime) override;

    /**
    * @brief ���t�F�[�h��`�̕`�揈��
    */
    void draw() override;

    /**
     * @brief �t�F�[�h���o�̊�������
    *
    * @return true �t�F�[�h���o���I�����Ă���
    * @return false ���o��
    */
    bool isFinished() const override;
};
