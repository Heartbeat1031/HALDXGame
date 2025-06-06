#pragma once
#include	"CommonTypes.h"
#include	"CMesh.h"

/**
 * @brief �~�����b�V���𐶐��E�Ǘ�����N���X
 *
 * �w�肳�ꂽ�������A���a�A�����Ɋ�Â��āA�~���̒��_�f�[�^�𐶐����܂��B
 * CMesh ���p�����A��{�I�ȃ��b�V������i���_�E�C���f�b�N�X�̐����j��񋟂��܂��B
 */
class CCylinderMesh : public CMesh {
public:
	/**
	 * @brief �~�����b�V���̏��������s��
	 *
	 * @param divx �~�������̕�����
	 * @param radius �~���̔��a
	 * @param height �~���̍���
	 * @param color ���_�̕`��F
	 */
	void Init(
		int		divx,
		float	radius,
		float	height,
		Color color);

	/**
	 * @brief ���_�f�[�^�𐶐�����
	 *
	 * �~���̏㉺�~�Ղ���ё��ʂ̒��_���𐶐����܂��B
	 */
	void CreateVertex();

private:
	/**
	 * @brief �~�������̕�����
	 */
	unsigned int m_division_x = 1;

	/**
	 * @brief ���������̕������i���g�p�j
	 */
	unsigned int m_division_y = 1;

	/**
	 * @brief �~���̔��a
	 */
	float  m_radius = 100.0f;

	/**
	 * @brief �~���̍���
	 */
	float  m_height = 100.0f;

	/**
	 * @brief �~�����i= ���a���玩���v�Z�j
	 */
	float  m_width;

	/**
	 * @brief ���_�J���[
	 */
	Color m_color;
};
