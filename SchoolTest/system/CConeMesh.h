#pragma once
#include "CommonTypes.h"
#include "CMesh.h"

/**
 * @brief �~���`��̃��b�V���𐶐��E�Ǘ�����N���X
 *
 * CMesh ���p�����A�������E���a�E�����ɉ����Ē��_�E�C���f�b�N�X�i�ʁj�f�[�^�𐶐����܂��B
 */
class CConeMesh : public CMesh {
	/**
	 * @brief �O�p�`�ʂ��\�����钸�_�C���f�b�N�X���
	 */
	struct Face {
		unsigned int idx[3]; ///< �O�p�`���\������3�̒��_�C���f�b�N�X
	};

public:
	/**
	 * @brief �~�����b�V��������������
	 *
	 * @param divx �������̕�����
	 * @param radius �~����ʂ̔��a
	 * @param height �~���̍���
	 * @param color ���_�J���[
	 * @param bottomup true: �������ցi��ʁ����_�j�����Afalse: �ォ�牺�ցi���_����ʁj����
	 */
	void Init(
		int divx,
		float radius,
		float height,
		Color color,
		bool bottomup = true);

	/**
	 * @brief ���_�f�[�^�𐶐��i�ォ�牺�j
	 *
	 * �ʏ�͂�������g�p�B���_�i�R���j����ʕ����ɒ��_�f�[�^���\���B
	 */
	void CreateVertex();

	/**
	 * @brief ���_�f�[�^�𐶐��i�������j
	 *
	 * ��ʁ��R���̏��Œ��_�f�[�^���\���������ꍇ�Ɏg�p�B
	 */
	void CreateVertexTopDown();

	// �C���f�b�N�X�����֐��͖������i�K�v�ɉ����Ēǉ��j

	/**
	 * @brief �ʏ��i�C���f�b�N�X�j�̔z��
	 *
	 * ���_�C���f�b�N�X3����\�������O�p�`�̈ꗗ�ł��B
	 */
	std::vector<Face> m_faces;

private:
	/**
	 * @brief �������i�o�x�j�̕�����
	 */
	unsigned int m_division_x = 1;

	/**
	 * @brief �c�����i���������j�̕������i���g�p�j
	 */
	unsigned int m_division_y = 1;

	/**
	 * @brief �~����ʂ̔��a
	 */
	float m_radius = 100.0f;

	/**
	 * @brief �~���̍���
	 */
	float m_height = 100.0f;

	/**
	 * @brief �~���̕��i�K�v�ɉ����Ĕ��a����v�Z�j
	 */
	float m_width;

	/**
	 * @brief ���_�J���[
	 */
	Color m_color;
};
