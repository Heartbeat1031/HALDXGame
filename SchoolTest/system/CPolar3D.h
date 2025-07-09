#pragma once
#include	"CommonTypes.h"

// ÇRÇcã…ç¿ïWån
class CPolor3D {
	float m_radius;				// îºåa		
	float m_elevation;			// ã¬äp
	float m_azimuth;			// ï˚à äp
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
