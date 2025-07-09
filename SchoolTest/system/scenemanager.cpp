#include "SceneManager.h"

/// @brief �o�^���ꂽ�V�[���̈ꗗ
std::unordered_map<std::string, std::unique_ptr<IScene>> SceneManager::m_scenes;

/// @brief ���݃A�N�e�B�u�ȃV�[���̖��O
std::string SceneManager::m_currentSceneName;

/// @brief ���݂̃V�[���J�ډ��o
std::unique_ptr<SceneTransition> SceneManager::m_transition = nullptr;

/// @brief �V�[���J�ڒ��t���O
bool SceneManager::m_isTransitioning = false;

/// @brief �J�ڌ�ɐ؂�ւ���V�[����
std::string SceneManager::m_nextSceneName;

/**
 * @brief �V�[����ǉ�����
 * @param name �V�[���̖��O
 * @param scene �o�^����V�[���C���X�^���X�i���L�����ړ��j
 *
 * AddScene ���g���āA�Q�[���J�n�O�ɂ��ׂẴV�[����o�^���Ă����B
 */
void SceneManager::AddScene(const std::string& name, std::unique_ptr<IScene> scene) {
    m_scenes[name] = std::move(scene);
    m_scenes[name]->init();
}

/**
 * @brief ���݂̃V�[����؂�ւ���
 * @param name �J�ڐ�̃V�[����
 * @param transition �I�v�V�����F�J�ډ��o�i�t�F�[�h�Ȃǁj
 *
 * �����ɐ؂�ւ��邩�A`SceneTransition` ��p���đJ�ډ��o��ɕύX�����B
 */
void SceneManager::SetCurrentScene(const std::string& name, std::unique_ptr<SceneTransition> transition) {
    if (transition) {
        m_transition = std::move(transition);
        m_transition->start(name);
        m_isTransitioning = true;
        m_nextSceneName = name;
    }
    else {
        InternalChangeScene(name);
    }
}

/**
 * @brief �o�^����Ă���S�V�[��������������
 *
 * �Q�[���N�����ȂǂɎg�p�B���ׂẴV�[���ɑ΂��� `init()` ���Ăяo���B
 */
void SceneManager::Init() {
//    for (auto& [name, scene] : m_scenes) {
//        scene->init();
//    }
}

/**
 * @brief ���݂̃V�[������ёJ�ډ��o�̍X�V����
 * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�Ȃǁj
 *
 * �J�ڒ��� `SceneTransition` �̍X�V���D�悳��A������ɃV�[�����؂�ւ��B
 */
void SceneManager::Update(uint64_t deltaTime) {
    if (m_isTransitioning && m_transition) {
        m_transition->update(deltaTime);
        if (m_transition->isFinished()) {
            InternalChangeScene(m_nextSceneName);
            m_transition = nullptr;
            m_isTransitioning = false;
        }
    }
    else if (!m_currentSceneName.empty()) {
        m_scenes[m_currentSceneName]->update(deltaTime);
    }
}

/**
 * @brief ���݂̃V�[���ƑJ�ډ��o�̕`�揈��
 * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�Ȃǁj
 *
 * �ʏ�̃V�[���`��ɉ����āA�J�ڒ��ł���Ή��o���d�˂ĕ`�悳���B
 */
void SceneManager::Draw(uint64_t deltaTime) {
    if (!m_currentSceneName.empty()) {
        m_scenes[m_currentSceneName]->draw(deltaTime);
    }
    if (m_isTransitioning && m_transition) {
        m_transition->draw();
    }
}

/**
 * @brief ���ׂẴV�[���̏I�������ƃ��\�[�X���
 *
 * �o�^���ꂽ�V�[�������ׂ� `dispose()` ���A���X�g����폜����B
 */
void SceneManager::Dispose() {
    for (auto& [name, scene] : m_scenes) {
        scene->dispose();
    }
    m_scenes.clear();
}

/**
 * @brief �����I�ɃV�[����؂�ւ���i���o������ȂǂɌĂ΂��j
 * @param name �؂�ւ���V�[���̖��O
 *
 * ���݂̃V�[���� `dispose()` ���ĂсA�؂�ւ���V�[���� `init()` �����s�B
 */
void SceneManager::InternalChangeScene(const std::string& name) {
    if (m_scenes.count(name)) {
        if (!m_currentSceneName.empty()) {
            m_scenes[m_currentSceneName]->dispose();
        }
        m_currentSceneName = name;
//      m_scenes[m_currentSceneName]->init();
    }
}
