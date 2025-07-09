#pragma once

#include "main.h"
#include "bg.h"
#include "player.h"
#include "playerbullet.h"
#include "boss.h"
#include "enemy.h"
#include "enemybullet.h"
#include "explosion.h"
#include "collisionmanager.h"
#include "system/camera.h"
#include "system/CDirectInput.h"
#include "system/scenemanager.h"
#include "system/fadetransition.h"
#include "system/CSprite.h"

/**
 * @brief ���C���Q�[���V�[��
 *
 * ���ۂ̃Q�[���v���C��S������V�[���B
 * �v���C���[����A�G�o���A�X�R�A�Ǘ��Ȃǂ̃Q�[���i�s���W�b�N�ƕ`����s���B
 */
class GameMainScene : public IScene {
public:
	/// @brief �R�s�[�R���X�g���N�^�͋֎~
	GameMainScene(const GameMainScene&) = delete;

	/// @brief ������Z�q���֎~
	GameMainScene& operator=(const GameMainScene&) = delete;

	/**
	 * @brief �R���X�g���N�^
	 *
	 * �Q�[���p�̃J������X�v���C�g�A�J�ډ��o�̏��������s���B
	 */
	explicit GameMainScene();

	/**
	 * @brief ���t���[���̍X�V����
	 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
	 *
	 * �Q�[���̃��W�b�N�i�L�����N�^�[�̓����A�����蔻��A�X�R�A�Ȃǁj���X�V����B
	 */
	void update(uint64_t deltatime) override;

	/**
	 * @brief ���t���[���̕`�揈��
	 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
	 *
	 * �w�i�A�L�����N�^�[�AUI �Ȃǂ̕`����s���B
	 */
	void draw(uint64_t deltatime) override;

	/**
	 * @brief �V�[������������
	 *
	 * �Q�[���J�n���̃X�v���C�g�z�u�⃊�\�[�X�̓ǂݍ��݂��s���B
	 */
	void init() override;

	/**
	 * @brief �V�[���I������
	 *
	 * �������̉�����ԃ��Z�b�g�ȂǁA�V�[�����E���ɕK�v�ȏ������s���B
	 */
	void dispose() override;


	/**
	 * @brief �V�[�����̂��ׂẴQ�[���I�u�W�F�N�g�����Z�b�g����
	 *
	 * 
	 */
	void resetallobject() {
		resetplayer();
		resetenemies();
		resetplayerbullet();
		resetenemybullet();
		resetexplosion();
	}

private:
	/// @brief ���C���J�����i���_����p�j
	Camera m_camera;

	/// @brief �V�[���J�ډ��o�i��F�Q�[���I�����̃t�F�[�h�A�E�g�Ȃǁj
	std::unique_ptr<SceneTransition> m_transition;

	/// @brief �Q�[�����ŕ\�������摜�X�v���C�g�i�w�i�Ȃǁj
	std::unique_ptr<CSprite> m_image;
};
