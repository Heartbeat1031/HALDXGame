#include "main.h"
#include "titlescene.h"
#include "system/CDirectInput.h"
#include "system/scenemanager.h"
#include "GameClearScene.h"
/**
 * @brief �^�C�g���V�[���̃R���X�g���N�^
 *
 * 
 */
GameClearScene::GameClearScene()
{
}

/**
 * @brief �N���A�V�[���̍X�V����
 *
 * Enter�L�[�i���^�[���L�[�j�������ꂽ��A`TitleScene` �փt�F�[�h�A�E�g�J�ڂ���B
 *
 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void GameClearScene::update(uint64_t deltatime)
{
    if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN))
    {
        SceneManager::SetCurrentScene(
            "TitleScene",
            std::make_unique<FadeTransition>(1000.0f, FadeTransition::Mode::FadeOutOnly)
        );
    }
}

/**
 * @brief �N���A�V�[���̕`�揈��
 *
 * �N���A�摜����ʒ����ɕ`�悷��B
 *
 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void GameClearScene::draw(uint64_t deltatime)
{
    m_image->Draw(
        Vector3(1.0f, 1.0f, 1.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f)
    );
}

/**
 * @brief �^�C�g���V�[���̏���������
 *
 * �V�[�����A�N�e�B�u�ɂȂ�ۂɈ�x�����Ă΂�鏉���������B
 */
void GameClearScene::init()
{
    static bool first = true;
    if (first) {
        m_image = std::make_unique<CSprite>(
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            "assets/texture/gameclear.png"
        );
        first = false;
    }
}

/**
 * @brief �N���A�V�[���̏I������
 *
 * �V�[�����j�������O�ɌĂ΂�郊�\�[�X��������B
 */
void GameClearScene::dispose()
{
}
