#include <array>
#include <memory>

#include "system/scenemanager.h"
#include "GameMainScene.h"
#include "TitleScene.h"
#include "GameClearScene.h"

/**
 * @brief �Q�[���̏���������
 *
 * �^�C�g���V�[���ƃ��C���Q�[���V�[���� SceneManager �ɓo�^���A
 * �����V�[���Ƃ��� TitleScene ��ݒ肷��B
 */
void GameInit()
{
    // �V�[���o�^
    SceneManager::AddScene("TitleScene", std::make_unique<TitleScene>());
    SceneManager::AddScene("GameMainScene", std::make_unique<GameMainScene>());
    SceneManager::AddScene("GameClearScene", std::make_unique<GameClearScene>());
    // �J�����g�V�[�����Z�b�g
    SceneManager::SetCurrentScene("TitleScene");
}

/**
 * @brief �Q�[���S�̂̍X�V����
 *
 * ���݃A�N�e�B�u�ȃV�[�����X�V����B
 *
 * @param delta �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void GameUpdate(uint64_t delta)
{
    SceneManager::Update(delta);
}

/**
 * @brief �Q�[���S�̂̕`�揈��
 *
 * ���݃A�N�e�B�u�ȃV�[����`�悷��B
 *
 * @param delta �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void GameDraw(uint64_t delta)
{
    SceneManager::Draw(delta);
}

/**
 * @brief �Q�[���I�����̃��\�[�X�������
 *
 * �o�^���ꂽ���ׂẴV�[����j�����A���������������B
 */
void GameDispose()
{
    SceneManager::Dispose();
}

