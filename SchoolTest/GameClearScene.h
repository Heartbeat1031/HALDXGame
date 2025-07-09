#pragma once

#include "system/IScene.h"
#include "system/CSprite.h"
#include "system/camera.h"
#include "system/FadeTransition.h"

/**
 * @brief �^�C�g����ʂ�\���V�[��
 *
 * �Q�[���N�����ɕ\�������^�C�g�����S�A���j���[�A��������Ȃǂ�S���B
 * ���͂��󂯂ă��C���Q�[���V�[���ւ̑J�ڏ����Ȃǂ����������B
 */
class GameClearScene : public IScene {
public:
	/// @brief �R�s�[�R���X�g���N�^�͎g�p�s��
	GameClearScene(const GameClearScene&) = delete;

	/// @brief ������Z�q���g�p�s��
	GameClearScene& operator=(const GameClearScene&) = delete;

	/**
	 * @brief �R���X�g���N�^
	 *
	 * �J������摜�X�v���C�g�A�J�ډ��o�̏��������s���B
	 */
	explicit GameClearScene();

	/**
	 * @brief ���t���[���̍X�V����
	 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
	 *
	 * ���͏����A�A�j���[�V�����A�J�ڃ^�C�~���O�Ȃǂ̐�����s���B
	 */
	void update(uint64_t deltatime) override;

	/**
	 * @brief ���t���[���̕`�揈��
	 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
	 *
	 * �^�C�g�����S��w�i�Ȃǂ̃X�v���C�g�`����s���B
	 */
	void draw(uint64_t deltatime) override;

	/**
	 * @brief �V�[���̏���������
	 *
	 * �X�v���C�g�̐����A�J�����ݒ�A�����Đ��ȂǁA�\���ɕK�v�ȏ������s���B
	 */
	void init() override;

	/**
	 * @brief �V�[���̏I������
	 *
	 * ���\�[�X�̉���ȂǁA���̃V�[���ւ̑J�ڑO�ɕK�v�ȏ������s���B
	 */
	void dispose() override;

private:
	/// @brief �^�C�g����ʗp�̃J����
	Camera m_camera;

	/// @brief �V�[���J�ډ��o�i��F�t�F�[�h�A�E�g�j
	std::unique_ptr<SceneTransition> m_transition;

	/// @brief �^�C�g�����S��w�i�Ȃǂ̃X�v���C�g
	std::unique_ptr<CSprite> m_image;
};
