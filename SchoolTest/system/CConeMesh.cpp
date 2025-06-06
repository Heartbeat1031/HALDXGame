#include	<iostream>
#include	"CConeMesh.h"

/**
 * @brief �~�����b�V��������������
 *
 * �w�肳�ꂽ�������A���a�A�����A���_�J���[�Ɋ�Â��āA�~���̒��_�E�C���f�b�N�X�𐶐����܂��B
 * @param divx �������̕�����
 * @param radius �~����ʂ̔��a
 * @param height �~���̍���
 * @param color ���_�̐F
 * @param bottomup true: ��ʁ����_�̏��Œ��_�����Afalse: ���_����ʂ̏��Œ��_����
 */
void CConeMesh::Init(
	int		divx,
	float	radius,
	float	height,
	Color color,
	bool bottomup)
{
	// �T�C�Y�Z�b�g�i���ƍ����j�iXY���ʁj
	m_height = height;
	m_width = 2.0f * PI * radius;		// ���a�~�~����
	m_radius = radius;

	// ������
	m_division_x = divx;

	// ���_�J���[
	m_color = color;

	// ���_�f�^����
	if (bottomup) {
		CreateVertex();
	}
	else {
		CreateVertexTopDown();
	}
}

/**
 * @brief ���_���u��ʁ����_�v�̏��Ő�������
 *
 * �~���̒��_�z����ʂ���R���Ɍ������č\�����A�C���f�b�N�X���ǉ����܂��B
 * ��ʎ��ӂ̎O�p�`����ђ�ʎ��̂̃|���S�����܂߂č\�z���܂��B
 */
void CConeMesh::CreateVertex()
{
	// ���_�f�[�^�N���A
	m_vertices.clear();

	// �Ă��؂�̒��_	
	VERTEX_3D v{};

	v.Position = Vector3(0.0f, m_height, 0.0f);
	v.Normal = Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;

	m_vertices.emplace_back(v);

	// ��ʂ̉~����̒��_�f�[�^���쐬
	float azimuth = 0.0f;			// ���ʊp
	for (unsigned int i = 0; i <= m_division_x; i++) {

		azimuth = (2 * PI * (float)i) / (float)m_division_x;	// ���ʊp���Z�b�g

		VERTEX_3D	v{};

		v.Position.x = m_radius * cosf(azimuth);
		v.Position.y = 0.0f;
		v.Position.z = m_radius * sinf(azimuth);

		v.Normal = Vector3(0.0f, 1.0f, 0.0f);		// �@���x�N�g���𒸓_���W�Ɠ����ɂ���	
		v.Diffuse = m_color;

		m_vertices.emplace_back(v);
	}

	// ���_�����ʂ̒��_�ւ̃C���f�b�N�X��ǉ�
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(0);						// ���_
		m_indices.push_back(i + 2);					// ���̒�ʒ��_
		m_indices.push_back(i + 1);					// ���݂̒�ʒ��_
	}

	// ��ʂ̒��S���W
	v.Position = Vector3(0.0f, 0.0f, 0.0f);
	v.Normal = Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;
	m_vertices.emplace_back(v);

	size_t bottomcenteridx = m_vertices.size() - 1;	// ��ʂ̒��S���W�̃C���f�b�N�X

	// ��ʂ̎O�p�`���C���f�b�N�X��
	for (unsigned int i = 0; i <= m_division_x; i++) {
		m_indices.push_back(static_cast<int>(bottomcenteridx));		// ��ʂ̒��S
		m_indices.push_back(i + 1);					// ���̒�ʒ��_
		m_indices.push_back(i + 2);					// ���݂̒�ʒ��_
	}
}

/**
 * @brief ���_���u���_����ʁv�̏��Ő�������
 *
 * �~���̒��_�z����R�������ʂɌ������č\�����A�C���f�b�N�X���ǉ����܂��B
 * ��ʎ��ӂ̎O�p�`����ђ�ʎ��̂̃|���S�����܂߂č\�z���܂��B
 */
void CConeMesh::CreateVertexTopDown()
{
	// ���_�f�[�^�N���A
	m_vertices.clear();

	// �Ă��؂�̒��_	
	VERTEX_3D v{};

	v.Position = Vector3(0.0f, 0.0f, 0.0f);
	v.Normal = Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;

	m_vertices.emplace_back(v);

	// ��ʂ̉~����̒��_�f�[�^���쐬
	float azimuth = 0.0f;			// ���ʊp
	for (unsigned int i = 0; i <= m_division_x; i++) {

		azimuth = (2 * PI * (float)i) / (float)m_division_x;	// ���ʊp���Z�b�g

		VERTEX_3D	v{};

		v.Position.x = m_radius * cosf(azimuth);
		v.Position.y = m_height;
		v.Position.z = m_radius * sinf(azimuth);

		v.Normal = Vector3(0.0f, 1.0f, 0.0f);		// �@���x�N�g���𒸓_���W�Ɠ����ɂ���	
		v.Diffuse = m_color;

		m_vertices.emplace_back(v);
	}

	// ���_����e�Z�O�����g�̒�ʂ̒��_�ւ̃C���f�b�N�X��ǉ�
	for (unsigned int i = 0; i < m_division_x; i++) {
		m_indices.push_back(0);						// ���_
		m_indices.push_back(i + 2);					// ���̒�ʒ��_
		m_indices.push_back(i + 1);					// ���݂̒�ʒ��_
	}

	// ��ʂ̒��S���W
	v.Position = Vector3(0.0f, m_height, 0.0f);
	v.Normal = Vector3(0.0f, 1.0f, 0.0f);
	v.Diffuse = m_color;
	m_vertices.emplace_back(v);

	size_t bottomcenteridx = m_vertices.size() - 1;	// ��ʂ̒��S���W�̃C���f�b�N�X

	// ��ʂ̎O�p�`���C���f�b�N�X��
	for (unsigned int i = 0; i <= m_division_x; i++) {
		m_indices.push_back(static_cast<int>(bottomcenteridx));		// ��ʂ̒��S
		m_indices.push_back(i + 1);					// ���̒�ʒ��_
		m_indices.push_back(i + 2);					// ���݂̒�ʒ��_
	}
}
