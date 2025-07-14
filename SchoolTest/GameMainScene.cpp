#include	"GameMainScene.h"
#include    <thread>

/**
 * @brief GameMainScene �̃R���X�g���N�^
 *
 * �J������X�v���C�g�Ȃǂ̏������� `init()` ���ōs���B
 */
GameMainScene::GameMainScene()
{
}

/**
 * @brief ���t���[���̍X�V����
 *
 * �v���C���[��e�A�{�X�̏�Ԃ��X�V���A�Q�[���̐i�s�𐧌䂷��B
 * �G���^�[�L�[�iRETURN�j�������ꂽ�ꍇ�� `TitleScene` �փt�F�[�h�J�ڂ���B
 *
 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void GameMainScene::update(uint64_t deltatime)
{
	updatebg();
	updateplayer();
	UpdatePlayerBullet();

	UpdateEnemies();
	UpdateEnemyBullet();

	CollisionCheck();

	updateexplosion();

	if (isallDead()) {
		SceneManager::SetCurrentScene("GameClearScene", std::make_unique<FadeTransition>(500.0f));

		// 3000�~���b���resetallobject()���Ăяo��
		std::thread([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(3000));
			resetallobject();
			}).detach(); // detach���ăX���b�h��������i���p�r�ɂ�蒍�Ӂj
	}
}

/**
 * @brief ���t���[���̕`�揈��
 *
 * �w�i�A�v���C���[�A�{�X�A�e�Ȃǂ�`�悷��B
 * �Q�[���̎��o�I�ȓ��e����ʂɔ��f����B
 *
 * @param deltatime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�j
 */
void GameMainScene::draw(uint64_t deltatime)
{
	drawbg();
	drawplayer();
	DrawPlayerBullet();
	DrawEnemies();
	DrawEnemyBullet();
	drawexplosion();
	CollisionDraw();
}

/**
 * @brief �V�[���̏���������
 *
 * �Q�[���v���C�ɕK�v�Ȋe�v�f�i�w�i�A�v���C���[�A�{�X�A�e�j�̏��������s���B
 * ���\�[�X�̓ǂݍ��݂⏉���ʒu�̐ݒ�Ȃǂ��s���B
 */
void GameMainScene::init()
{
	initplayer();
	initbg();
	initexplosion();
	InitPlayerBullet();
	InitEnemies();
	InitEnemyBullet();
}

/**
 * @brief �V�[���̏I������
 *
 * �������̉�����ԃ��Z�b�g�ȂǁA�V�[���I�����ɕK�v�ȏ������s���B
 * ���V�[���ւ̑J�ڎ��ɌĂяo�����B
 */
void GameMainScene::dispose()
{
	disposeplayer();
	disposebg();
	disposeexplosion();
	DisposePlayerBullet();
	DisposeEnemyBullet();
	DisposeEnemies();
}
