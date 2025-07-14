#pragma once
#include	"CommonTypes.h"

// �R�c�ɍ��W�n
class CPolor3D {
	float m_radius;				// ���a		
	float m_elevation;			// �p
	float m_azimuth;			// ���ʊp
public:
	CPolor3D() = delete;
	CPolor3D(float radius,
		float elevation,
		float azimuth) : m_radius(radius), m_elevation(elevation), m_azimuth(azimuth) {}
	~CPolor3D() {}

	Vector3 ToOyler() {
		Vector3 euler;

		euler.x = m_radius * sinf(m_elevation) * cosf(m_azimuth);
		euler.y = m_radius * cosf(m_elevation);
		euler.z = m_radius * sinf(m_elevation) * sinf(m_azimuth);

		return euler;
	}
};
