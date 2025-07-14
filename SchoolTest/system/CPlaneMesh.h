#pragma once
#include	"CommonTypes.h"
#include	"CMesh.h"
#include	<random>

class CPlaneMesh : public CMesh {
	struct FACE {
		unsigned int idx[3];
	};
public:
	void Init(int divx, int divy,
		float width, float height,
		DirectX::SimpleMath::Color color,
		DirectX::SimpleMath::Vector3 normal,
		bool xzflag = false,
		bool cwflag = true);

	void CreateVertex();
	void CreateVertexXZ();

	void CreateIndexCW();						// ���v���ɃC���f�b�N�X�𐶐�
	void CreateIndexCCW();						// �����v���ɃC���f�b�N�X�𐶐�		

	// ��
	float GetWidth();
	// ����
	float GetHeight();
	int GetDivX();
	int GetDivY();

private:
	unsigned int m_divX = 1;
	unsigned int m_divY = 1;

	float  m_height = 100.0f;
	float  m_width = 100.0f;

	Color m_color;
	Vector3 m_normal;
};