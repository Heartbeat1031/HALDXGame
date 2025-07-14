#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <cstdint>
#include "noncopyable.h"
#include "IScene.h"
#include "SceneTransition.h"
#include "fadetransition.h"

/**
 * @brief �V�[���S�̂��Ǘ�����N���X
 *
 * �V�[���̓o�^�E�X�V�E�`��E�؂�ւ��ȂǁA�Q�[�����̏�ԊǗ����s���B
 * �t�F�[�h��N���X�t�F�[�h�Ȃǂ̎��o���ʕt���̃V�[���J�ڂ��T�|�[�g����B
 */
class SceneManager : NonCopyable {
    /// �o�^����Ă���V�[���̃}�b�v�i���O���V�[���C���X�^���X�j
    static std::unordered_map<std::string, std::unique_ptr<IScene>> m_scenes;

    /// ���ݗL���ȃV�[���̖��O
    static std::string m_currentSceneName;

    /// �J�ډ��o���s�����߂̃I�u�W�F�N�g�i��F�t�F�[�h�C���E�A�E�g�j
    static std::unique_ptr<SceneTransition> m_transition;

    /// ���ݑJ�ڒ����ǂ���
    static bool m_isTransitioning;

    /// �J�ڐ�̃V�[�����i�J�ڂ����������炱�̃V�[���ɐ؂�ւ���j
    static std::string m_nextSceneName;

public:
    /**
     * @brief �V�[�����}�l�[�W���[�ɒǉ�����
     * @param name �V�[�����ʗp�̖��O
     * @param scene �ǉ�����V�[���̃C���X�^���X�i���L�����ړ��j
     */
    static void AddScene(const std::string& name, std::unique_ptr<IScene> scene);

    /**
     * @brief �V�[����؂�ւ���
     * @param name �؂�ւ����̃V�[����
     * @param transition �J�ډ��o�i�ȗ��j
     *
     * �����ɂ͐؂�ւ���ꂸ�A�J�ډ��o���I��������ɃV�[�����ύX�����B
     */
    static void SetCurrentScene(const std::string& name, std::unique_ptr<SceneTransition> transition = nullptr);

    /**
     * @brief ���݂̃V�[���̏����������s����
     *
     * �ʏ�̓Q�[���N�����⃊�Z�b�g���Ɏg�p�B
     */
    static void Init();

    /**
     * @brief ���݂̃V�[������ёJ�ډ��o�̍X�V�������s��
     * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�Ȃǁj
     */
    static void Update(uint64_t deltaTime);

    /**
     * @brief ���݂̃V�[������ёJ�ډ��o�̕`�揈�����s��
     * @param deltaTime �O�t���[������̌o�ߎ��ԁi�}�C�N���b�Ȃǁj
     */
    static void Draw(uint64_t deltaTime);

    /**
     * @brief ���ׂẴV�[���ƑJ�ډ��o�̏I���������s��
     *
     * �g�p�I�����ɃV�[���̃��\�[�X�𖾎��I�ɔj������B
     */
    static void Dispose();

    /**
     * @brief �J�ڊ�����Ɏ��ۂɃV�[����؂�ւ�������֐�
     * @param name �؂�ւ���V�[����
     *
     * �ʏ�͊O������Ăяo���Ȃ��B
     */
    static void InternalChangeScene(const std::string& name);
};

