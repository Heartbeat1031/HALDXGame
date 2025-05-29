#pragma once
#include	"CommonTypes.h"
#include	"CMesh.h"


class CBoxMesh : public CMesh {
	// ?@???x?N?g?????v?Z
// ???_???W???m?[?}???C?Y
	void Normalize(
		Vector3 vec, 
		Vector3& Normal) {

		vec.Normalize();
		Normal = vec;

	}

public:
	void Init(float width,
		float height,
		float depth,
		Color color)
	{
		// ?T?C?Y????
		m_lengthx = width;
		m_lengthy = height;
		m_lengthz = depth;


		// ?J???[?l????
		m_color = color;

		// ????C???f?b?N?X?f?[?^????
		CreateIndex();

		// ??????_?f?[?^????
		CreateVertex();
	}

	// ???_?f?[?^????
	void CreateVertex() {
		// ???_?N???A
		m_vertices.clear();

		m_vertices.resize(12);
		m_vertices[0].Position.x = -(m_lengthx / 2.0f);
		m_vertices[0].Position.y = (m_lengthy / 2.0f);
		m_vertices[0].Position.z = -(m_lengthz / 2.0f);

		m_vertices[1].Position.x = (m_lengthx / 2.0f);
		m_vertices[1].Position.y = (m_lengthy / 2.0f);
		m_vertices[1].Position.z = -(m_lengthz / 2.0f);

		m_vertices[2].Position.x = (m_lengthx / 2.0f);
		m_vertices[2].Position.y = -(m_lengthy / 2.0f);
		m_vertices[2].Position.z = -(m_lengthz / 2.0f);

		m_vertices[3].Position.x = -(m_lengthx / 2.0f);
		m_vertices[3].Position.y = -(m_lengthy / 2.0f);
		m_vertices[3].Position.z = -(m_lengthz / 2.0f);

		m_vertices[4].Position.x = -(m_lengthx / 2.0f);
		m_vertices[4].Position.y = (m_lengthy / 2.0f);
		m_vertices[4].Position.z = (m_lengthz / 2.0f);

		m_vertices[5].Position.x = (m_lengthx / 2.0f);
		m_vertices[5].Position.y = (m_lengthy / 2.0f);
		m_vertices[5].Position.z = (m_lengthz / 2.0f);

		m_vertices[6].Position.x = (m_lengthx / 2.0f);
		m_vertices[6].Position.y = -(m_lengthy / 2.0f);
		m_vertices[6].Position.z = (m_lengthz / 2.0f);

		m_vertices[7].Position.x = -(m_lengthx / 2.0f);
		m_vertices[7].Position.y = -(m_lengthy / 2.0f);
		m_vertices[7].Position.z = (m_lengthz / 2.0f);

		for (int i = 0; i < 8; i++) {
			// ???_?J???[???
			m_vertices[i].Diffuse = m_color;

			// ?m?[?}???C?Y
			Normalize(
				m_vertices[i].Position,
				m_vertices[i].Normal);
		}
	}

	// ?C???f?b?N?X????
	void CreateIndex() {
		struct FACE {
			unsigned int idx[3];
		};

		// ?C???f?b?N?X?f?[?^?N???A
		m_indices.clear();

		// ?C???f?b?N?X???
		FACE faces[12];

		faces[0].idx[0] = 0;		// ??O  ok
		faces[0].idx[1] = 1;
		faces[0].idx[2] = 2;

		faces[1].idx[0] = 2;
		faces[1].idx[1] = 3;
		faces[1].idx[2] = 0;

		faces[2].idx[0] = 5;		// ??  ok
		faces[2].idx[1] = 4;
		faces[2].idx[2] = 7;

		faces[3].idx[0] = 7;
		faces[3].idx[1] = 6;
		faces[3].idx[2] = 5;

		faces[4].idx[0] = 4;		// ???? ok
		faces[4].idx[1] = 0;
		faces[4].idx[2] = 7;

		faces[5].idx[0] = 0;
		faces[5].idx[1] = 3;
		faces[5].idx[2] = 7;

		faces[6].idx[0] = 1;		// ?E?? ok
		faces[6].idx[1] = 5;
		faces[6].idx[2] = 6;

		faces[7].idx[0] = 1;
		faces[7].idx[1] = 6;
		faces[7].idx[2] = 2;

		faces[8].idx[0] = 0;		// ?? ok
		faces[8].idx[1] = 4;
		faces[8].idx[2] = 1;

		faces[9].idx[0] = 4;
		faces[9].idx[1] = 5;
		faces[9].idx[2] = 1;

		faces[10].idx[0] = 3;		// ????
		faces[10].idx[1] = 2;
		faces[10].idx[2] = 7;

		faces[11].idx[0] = 6;
		faces[11].idx[1] = 7;
		faces[11].idx[2] = 2;

		// ?C???f?b?N?X?o?^
		for (int i = 0; i < 12; i++) {
			m_indices.emplace_back(faces[i].idx[0]);
			m_indices.emplace_back(faces[i].idx[1]);
			m_indices.emplace_back(faces[i].idx[2]);
		}

	}


private:
	Color m_color;
	float m_lengthx = 0.0f;
	float m_lengthy = 0.0f;
	float m_lengthz = 0.0f;
};