#include    "../main.h"
#include    "scenemanager.h"
#include    "FadeTransition.h"
#include    "BoxDrawer.h"

/**
 * @brief �t�F�[�h�J�ڂ̊J�n����
 *
 * ���[�h�ɉ����ăt�F�[�Y��ݒ肵�A�K�v�ɉ����Đ�ɃV�[����؂�ւ���B
 *
 * @param nextSceneName �J�ڐ�̃V�[����
 */
void FadeTransition::start(const std::string& nextSceneName) {
    m_nextScene = nextSceneName;
    m_elapsed = 0;

    switch (m_mode) {
    case Mode::FadeOutOnly:
        m_phase = Phase::FadeOut;
        m_alpha = 0.0f;
        break;
    case Mode::FadeInOnly:
        SceneManager::InternalChangeScene(m_nextScene); // ��ɃV�[���ύX
        m_phase = Phase::FadeIn;
        m_alpha = 1.0f;
        break;
    case Mode::FadeInOut:
        m_phase = Phase::FadeOut;
        m_alpha = 0.0f;
        break;
    }
}

/**
 * @brief �t�F�[�h�̐i�s���X�V����
 *
 * �o�ߎ��ԂɊ�Â��ē����x�im_alpha�j��ω�������B
 * �t�F�[�h�A�E�g�I����̓V�[����؂�ւ��A�t�F�[�h�C���Ɉڍs����B
 *
 * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void FadeTransition::update(uint64_t deltaTime) {
    m_elapsed += deltaTime;

    switch (m_phase) {
    case Phase::FadeOut:
        m_alpha = static_cast<float>(m_elapsed) / m_duration;
        if (m_elapsed >= m_duration) {
            m_alpha = 1.0f;
            m_elapsed = 0;

            if (m_mode == Mode::FadeOutOnly) {
                m_phase = Phase::None;
            }
            else {
                SceneManager::InternalChangeScene(m_nextScene);
                m_phase = Phase::FadeIn;
            }
        }
        break;

    case Phase::FadeIn:
        m_alpha = 1.0f - static_cast<float>(m_elapsed) / m_duration;
        if (m_elapsed >= m_duration) {
            m_alpha = 0.0f;
            m_phase = Phase::None;
        }
        break;

    default:
        break;
    }
}

/**
 * @brief �t�F�[�h�p�̍�����`����ʂɕ`�悷��
 *
 * ���݂̓����x�ɉ����������S��ʃI�[�o�[���C��\�����A���R�ȃt�F�[�h���ʂ����o����B
 */
void FadeTransition::draw() {
    if (m_phase != Phase::None) {
        BoxDrawerDraw(
            SCREEN_WIDTH, SCREEN_HEIGHT, 0,
            Color(0, 0, 0, m_alpha),
            SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0
        );
    }
}

/**
 * @brief �t�F�[�h���o�������������𔻒肷��
 *
 * ���݂̃t�F�[�Y�� None �ɂȂ��Ă���Ή��o�����B
 *
 * @return true �t�F�[�h���������Ă���
 * @return false �܂����o���ł���
 */
bool FadeTransition::isFinished() const {
    return m_phase == Phase::None;
}
