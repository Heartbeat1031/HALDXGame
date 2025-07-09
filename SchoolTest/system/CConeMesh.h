#pragma once
#include	"CommonTypes.h"
#include	<random>
#include	"CMesh.h"

class CConeMesh : public CMesh {
	struct Face {
		unsigned int idx[3];
	};
public:
	void Init(
		int		divx,							// ������			
		float	radius,							// ���a	
		float	height,							// ����	
		Color	color,		// ���_�J���[			
		bool	bottomup = true);					// �������Ɍ������Ē��_�𐶐�����
												//  (��ʁ���vertex true
												//  (vertex������� false

	void CreateVertex();						// ���_�f�[�^����
	void CreateVertexTopDown();					// ���_�f�[�^�����i�������Ɍ������āj

//	void CreateIndex();							// �C���f�b�N�X�𐶐�

	// �ʃf�[�^		
	std::vector<Face> m_faces;
private:
	unsigned int m_division_x = 1;
	unsigned int m_division_y = 1;

	float  m_radius = 100.0f;					// ���a	

	float  m_height = 100.0f;
	float  m_width;								// ���a���玩���v�Z����

	Color m_color;			// ���_�J���[
};