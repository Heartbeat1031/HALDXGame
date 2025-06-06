#define NOMINMAX // �ݕ�� windows.h �V�O���C�h�~����? max/min �G
#include <windows.h>
#include <algorithm>
#include    "commontypes.h"
#include	"collision.h"

namespace GM31 {
	namespace GE {
		namespace {}
		namespace Collision {

			// OBB�̏d�Ȃ�𔻒肷��
			bool CompareLengthOBB(
				const BoundingBoxOBB& ObbA,		// OBB-A
				const BoundingBoxOBB& ObbB,		// OBB-B
				const Vector3& vecSeparate,		// ������
				const Vector3& vecDistance)		// ���S���W�����񂾃x�N�g��
			{
				// ����������������	(�ۑ�No3�@start)
				float fDistance{};
				fDistance = fabs(vecDistance.Dot(vecSeparate));	// ���S���W�����񂾃x�N�g���ƕ������̓��ς����߂�

				float fShadowA = 0;
				float fShadowB = 0;

				// OBB-A�̉e�̒��������߂�
				float fShadowAx{};
				float fShadowAy{};
				float fShadowAz{};

				// X���𕪗����Ɏˉe
				fShadowAx = vecSeparate.Dot(ObbA.axisX);
				fShadowAx = fabsf(fShadowAx * (ObbA.lengthx / 2.0f));

				// Y���𕪗����Ɏˉe
				fShadowAy = vecSeparate.Dot(ObbA.axisY);
				fShadowAy = fabsf(fShadowAy * (ObbA.lengthy / 2.0f));

				// Z���𕪗����Ɏˉe
				fShadowAz = vecSeparate.Dot(ObbA.axisZ);
				fShadowAz = fabsf(fShadowAz * (ObbA.lengthz / 2.0f));

				//�������Ɏˉe�����e�̒��������߂�
				fShadowA = fShadowAx + fShadowAy + fShadowAz;

				// OBB-B�̉e�̒��������߂�
				float fShadowBx{};
				float fShadowBy{};
				float fShadowBz{};

				// X���𕪗����Ɏˉe
				fShadowBx = vecSeparate.Dot(ObbB.axisX);
				fShadowBx = fabsf(fShadowBx * (ObbB.lengthx / 2.0f));

				// Y���𕪗����Ɏˉe
				fShadowBy = vecSeparate.Dot(ObbB.axisY);
				fShadowBy = fabsf(fShadowBy * (ObbB.lengthy / 2.0f));

				// Z���𕪗����Ɏˉe
				fShadowBz = vecSeparate.Dot(ObbB.axisZ);
				fShadowBz = fabsf(fShadowBz * (ObbB.lengthz / 2.0f));

				//�������Ɏˉe�����e�̒��������߂�
				fShadowB = fShadowBx + fShadowBy + fShadowBz;
				

				if(fDistance > fShadowA + fShadowB) {	// �������Ɏˉe�����e�̒��������߂�
					return false;	// �����������������̂ŏՓ˂��Ă��Ȃ�
				}

				return true;
			}

			// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
			void CalcQuadOrientedLine(
				Vector3 startpos,
				Vector3 endpos,
				Vector3* v,
				float width)
			{

				// �����ȃx�N�g�������߂�	
				Vector3 zaxis(0, 0, 1);
				Vector3 line = endpos - startpos;

				Vector3 ans;
				ans = line.Cross(zaxis);					// �O��
				ans.Normalize();							// ���K��

				Vector3 startposplus = startpos + ans * width / 2.0f;
				Vector3 startposminus = startpos - ans * width / 2.0f;

				Vector3 endposplus = endpos + ans * width / 2.0f;
				Vector3 endposminus = endpos - ans * width / 2.0f;

				v[0] = startposplus;
				v[1] = endposplus;

				v[2] = startposminus;
				v[3] = endposminus;

			}

			// �����ɕ��s�ȗ^����ꂽ���̂S�p�`�̍��W�����߂�
			void CalcQuadOrientedLine(
				Vector3 startpos,
				Vector3 endpos,
				Vector3 up,
				Vector3* v,
				float width)
			{

				// �����ȃx�N�g�������߂�	
				Vector3 zaxis = up;
				Vector3 line = endpos - startpos;



				Vector3 ans;
				ans = line.Cross(zaxis);					// �O��
				ans.Normalize();							// ���K��

				Vector3 startposplus = startpos + ans * width / 2.0f;
				Vector3 startposminus = startpos - ans * width / 2.0f;

				Vector3 endposplus = endpos + ans * width / 2.0f;
				Vector3 endposminus = endpos - ans * width / 2.0f;

				v[0] = startposplus;
				v[1] = endposplus;

				v[2] = startposminus;
				v[3] = endposminus;

			}



			// �����Ɠ_�̋��������߂�
			float calcPointLineDist(
				const Vector3& point,
				const Segment& segment,
				Vector3& intersectionpoint,
				float& t) {

				float distance = 0.0f;

				// �����̃x�N�g�������߂�
				double ABx = segment.endpoint.x - segment.startpoint.x;
				double ABy = segment.endpoint.y - segment.startpoint.y;
				double ABz = segment.endpoint.z - segment.startpoint.z;

				// �����̎n�_�Ɠ_�����ԃx�N�g�������߂�
				double APx = point.x - segment.startpoint.x;
				double APy = point.y - segment.startpoint.y;
				double APz = point.z - segment.startpoint.z;

				// ����AB�̒��������߂�
				double AB2 = ABx * ABx + ABy * ABy + ABz * ABz;

				// AB�x�N�g����AP�x�N�g���̓��ς��v�Z����i�ˉe�������������߂�j
				double ABdotAP = ABx * APx + ABy * APy + ABz * APz;

				// tt���v�Z
				double tt = ABdotAP / AB2;

				// �����̑�
				intersectionpoint.x = static_cast<float>(segment.startpoint.x + ABx * tt);
				intersectionpoint.y = static_cast<float>(segment.startpoint.y + ABy * tt);
				intersectionpoint.z = static_cast<float>(segment.startpoint.z + ABz * tt);

				t = static_cast<float>(tt);
				// �����̑��̒���
				distance = (intersectionpoint - point).Length();
				return distance;
			}

			// �����ƒ����̒��������߂�
			float calcPointSegmentDist(
				const Vector3& p,
				const Segment& segment,
				Vector3& intersectionpoint,
				float& t) {

				float distance = calcPointLineDist(p, segment, intersectionpoint, t);

				// ��_�������̊O�ɂ���i�n�_�ɋ߂��j
				if (t < 0.0f) {

					intersectionpoint = segment.startpoint;		// �J�n�_����_

					float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

					return l;
				}

				// ��_�������̊O�ɂ���i�I�_�ɋ߂��j
				if (t > 1.0f) {

					intersectionpoint = segment.endpoint;		// �I�_����_

					float l = (p - intersectionpoint).Length();	// ��_�Ƃ̋��������߂�

					return l;
				}

				// ��_��������ɂ���i�O�������P�j
				return distance;
			}

			// �����Ɠ_�̋��������߂���@���g�p����
			bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder)
			{
				// �����̑�	
				Vector3 intersectionpoint;
				float t{};

				Segment seg;
				seg.startpoint = cylinder.bottom;
				seg.endpoint = cylinder.top;

				// ���̒��S�Ɛ����̋��������߂�
				float length = calcPointSegmentDist(
					sphere.center,
					seg,
					intersectionpoint,
					t);

				// ���̒��S�Ɛ����̋��������a��菬�����ꍇ�͓������Ă���
				if (length > sphere.radius + cylinder.radius) {
					return false;
				}

				// �����̎n�_�ɋ߂�
				if (t < 0) {
					BoundingBoxAABB aabb;

					aabb.max.x = cylinder.top.x + cylinder.radius;
					aabb.max.y = cylinder.top.y;
					aabb.max.z = cylinder.top.z + cylinder.radius;

					aabb.min.x = cylinder.bottom.x - cylinder.radius;
					aabb.min.y = cylinder.bottom.y;
					aabb.min.z = cylinder.bottom.z - cylinder.radius;

					float length = SqDistPointAABB(sphere.center, aabb);
					length = sqrt(length);

					if (length < sphere.radius) {
						return true;
					}
				}
				// �����̏I�_�ɋ߂�
				else if (t > 1) {
					BoundingBoxAABB aabb;

					aabb.max.x = cylinder.top.x + cylinder.radius;
					aabb.max.y = cylinder.top.y;
					aabb.max.z = cylinder.top.z + cylinder.radius;

					aabb.min.x = cylinder.bottom.x - cylinder.radius;
					aabb.min.y = cylinder.bottom.y;
					aabb.min.z = cylinder.bottom.z - cylinder.radius;

					float length = SqDistPointAABB(sphere.center, aabb);
					length = sqrt(length);

					if (length < sphere.radius) {
						return true;
					}
				}
				// �����̎n�_�ƏI�_�̊Ԃɂ��邩�H
				else {
					return true;
				}

				return false;
			}

			// ��
			bool CollisionSphere(BoundingSphere p1, BoundingSphere p2)
			{

				double length = (p1.center - p2.center).Length();

				double radiussum = p1.radius + p2.radius;

				return length <= radiussum;
			}

			// OBB�̓����蔻����s��
			bool CollisionOBB(
				const BoundingBoxOBB& obbA,
				const BoundingBoxOBB& obbB) {

				// ����������������	(�ۑ�No3�@start)
				//������
				Vector3 vecSeparate;

				Vector3 vecDistance = obbB.worldcenter - obbA.worldcenter;	// ���S���W�����ԃx�N�g��

				bool sts;
				// OBB-A�����X�g
				const Vector3* OBB_A_Axis[3] = {
					&obbA.axisX,
					&obbA.axisY,
					&obbA.axisZ
				};
				// OBB-B�����X�g
				const Vector3* OBB_B_Axis[3] = {
					&obbB.axisX,
					&obbB.axisY,
					&obbB.axisZ
				};
				// OBB-A��3���𕪗����ɂ��Ĕ���
				for (int i = 0; i < 3; i++) {
					vecSeparate = *OBB_A_Axis[i];	// ��������ݒ�

					// OBB���m�̏d�Ȃ�𔻒�
					sts = CompareLengthOBB(obbA, obbB, vecSeparate, vecDistance);
					if (!sts) {
						return false;	// �����������������̂ŏՓ˂��Ă��Ȃ�
					}
				}
				// OBB-B��3���𕪗����ɂ��Ĕ���
				for (int i = 0; i < 3; i++) {
					vecSeparate = *OBB_B_Axis[i];  // ��������ݒ�

					// OBB���m�̏d�Ȃ�𔻒�
					sts = CompareLengthOBB(obbA, obbB, vecSeparate, vecDistance);
					if (!sts) {
						return false;  // �����������������̂ŏՓ˂��Ă��Ȃ�
					}
				}
				//�O�ς𕪗����ɂ��Čv�Z
				for (int p1 = 0; p1 < 3; p1++) {
					for (int p2 = 0; p2 < 3; p2++) {
						
						Vector3 obbAaxis = *OBB_A_Axis[p1];	// OBB-A�̎�
						Vector3 obbBaxis = *OBB_B_Axis[p2];	// OBB-B�̎�

						Vector3 crossseparate = obbAaxis.Cross(obbBaxis);	// �O�ς����߂�

						// OBB���m�̏d�Ȃ�𔻒�
						sts = CompareLengthOBB(obbA, obbB, crossseparate, vecDistance);
						if (!sts) {
							return false;	// �����������������̂ŏՓ˂��Ă��Ȃ�
						}
					}
				}
				
				return true;
			}

			// AABB
			bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2) {

				// X���W
				if (p1.max.x < p2.min.x) {
					return false;
				}

				if (p1.min.x > p2.max.x) {
					return false;
				}

				// Y���W
				if (p1.max.y < p2.min.y) {
					return false;
				}

				if (p1.min.y > p2.max.y) {
					return false;
				}

				// Z���W
				if (p1.max.z < p2.min.z) {
					return false;
				}

				if (p1.min.z > p2.max.z) {
					return false;
				}

				return true;
			}


			BoundingBoxAABB SetAABB(Vector3 centerposition, float width, float height, float depth) {

				BoundingBoxAABB aabb{};

				width = fabs(width);
				height = fabs(height);
				depth = fabs(depth);

				aabb.min.x = centerposition.x - width / 2.0f;
				aabb.min.y = centerposition.y - height / 2.0f;
				aabb.min.z = centerposition.z - depth / 2.0f;

				aabb.max.x = centerposition.x + width / 2.0f;
				aabb.max.y = centerposition.y + height / 2.0f;
				aabb.max.z = centerposition.z + depth / 2.0f;

				return aabb;
			}

			// AABB�Ɠ_�̂����Ƌ߂��_�����߂�
			void ClosestPtPointAABB(Vector3 p, BoundingBoxAABB aabb, Vector3& q)
			{
				q.x = std::max(aabb.min.x, std::min(p.x, aabb.max.x));
				q.y = std::max(aabb.min.y, std::min(p.y, aabb.max.y));
				q.z = std::max(aabb.min.z, std::min(p.z, aabb.max.z));
			}

			// AABB�Ɠ_�̋��������߂�i�Q��j
			float SqDistPointAABB(Vector3 p, BoundingBoxAABB aabb)
			{
				float sqDist = 0.0f;

				if (p.x < aabb.min.x) sqDist += (aabb.min.x - p.x) * (aabb.min.x - p.x);
				if (p.x > aabb.max.x) sqDist += (p.x - aabb.max.x) * (p.x - aabb.max.x);

				if (p.y < aabb.min.y) sqDist += (aabb.min.y - p.y) * (aabb.min.y - p.y);
				if (p.y > aabb.max.y) sqDist += (p.y - aabb.max.y) * (p.y - aabb.max.y);

				if (p.z < aabb.min.z) sqDist += (aabb.min.z - p.z) * (aabb.min.z - p.z);
				if (p.z > aabb.max.z) sqDist += (p.z - aabb.max.z) * (p.z - aabb.max.z);

				return sqDist;
			}

			// AABB�Ƌ��̓����蔻��
			bool CollisionSphereAABB(
				BoundingSphere sphere,
				BoundingBoxAABB aabb)
			{
				// AABB�Ɠ_�̋��������߂�
				float lng = SqDistPointAABB(sphere.center, aabb);

				if (lng < sphere.radius * sphere.radius)
				{
					return true;
				}

				return false;
			}

			// OBB�Ɠ_�̂����Ƌ߂��_�����߂�
			void ClosestPtPointOBB(Vector3 point, BoundingBoxOBB obb, Vector3& answer)
			{
				// �_����OBB�̒��S�ւ̃x�N�g��(���������߂�)
				Vector3 diff = point - obb.worldcenter;

				answer = obb.worldcenter;

				// OBB�̃��[�J�����W�ɕϊ��i���͐��K������Ă��邱�Ƃ�O��Ƃ���j
				float obbaxisx = diff.Dot(obb.axisX);
				float obbaxisy = diff.Dot(obb.axisY);
				float obbaxisz = diff.Dot(obb.axisZ);

				// OBB�̊e���ɉ������ŋߐړ_�����߂�
				obbaxisx = std::clamp(obbaxisx, -0.5f * obb.lengthx, 0.5f * obb.lengthx);
				obbaxisy = std::clamp(obbaxisy, -0.5f * obb.lengthy, 0.5f * obb.lengthy);
				obbaxisz = std::clamp(obbaxisz, -0.5f * obb.lengthz, 0.5f * obb.lengthz);

				// ���[���h���W�ɕϊ�
				answer = obb.worldcenter + obb.axisX * obbaxisx + obb.axisY * obbaxisy + obb.axisZ * obbaxisz;
			}

			// OBB�Ɠ_�̋��������߂�
			float DistPointOBB(Vector3 point, BoundingBoxOBB obb)
			{
				Vector3 answer{};

				// OBB�Ɠ_�̂����Ƌ߂��_�����߂�
				ClosestPtPointOBB(point, obb, answer);
				// ���������߂�
				float length = (answer - point).Length();

				return length;
			}

			// OBB�̐ݒ�
			BoundingBoxOBB SetOBB(
				Vector3 rot,					// �p���i��]�p�x�j
				Vector3 pos,					// ���S���W�i���[���h�j
				float width,					// ��
				float height,					// ����
				float depth)					// ���s
			{
				BoundingBoxOBB obb{};

				BoundingBoxAABB aabb = SetAABB(pos, width, height, depth);
				obb.max = aabb.max;
				obb.min = aabb.min;

				Matrix4x4 mtx = Matrix4x4::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);

				obb.axisX = Vector3(mtx._11, mtx._12, mtx._13);
				obb.axisY = Vector3(mtx._21, mtx._22, mtx._23);
				obb.axisZ = Vector3(mtx._31, mtx._32, mtx._33);

				obb.lengthx = width;
				obb.lengthy = height;
				obb.lengthz = depth;

				obb.worldcenter = pos;
				obb.center = Vector3(0, 0, 0);

				return obb;
			}

			// OBB�Ƌ��̓����蔻��
			bool CollisionSphereOBB(
				BoundingSphere sphere,
				BoundingBoxOBB obb)
			{
				// OBB�Ɠ_�̋��������߂�
				float lng = DistPointOBB(sphere.center, obb);

				if (lng < sphere.radius)
				{
					return true;
				}

				return false;
			}

			// �J�v�Z�����m�̓����蔻��
			bool CollisionCapsule(BoundingCapsule caps1, BoundingCapsule caps2)
			{
				// �����̎n�_�ƏI�_�����߂�
				Segment seg1;
				seg1.startpoint = caps1.startpoint;
				seg1.endpoint = caps1.endpoint;

				Segment seg2;
				seg2.startpoint = caps2.startpoint;
				seg2.endpoint = caps2.endpoint;

				// �Q�̐����̍ŒZ���������߂�
				Vector3 p1, p2;
				float s, t;
				float length = ClosestPtSegmentSegment(seg1, seg2, s, t, p1, p2);

				// ���̔��a���l�����ē����蔻��
				if (length < caps1.radius + caps2.radius) {
					return true;
				}

				return false;
			}

			// �����Ɛ����̍ŒZ���������߂�
			float ClosestPtSegmentSegment(
				const Segment& seg1,				// �����P
				const Segment& seg2, 				// �����Q	
				float& s,							// �����P�̍ŒZ�����̃p�����[�^
				float& t,							// �����Q�̍ŒZ�����̃p�����[�^
				Vector3& p1, 						// �����P�̍ŒZ�����̓_	
				Vector3& p2) 						// �����Q�̍ŒZ�����̓_
			{
				Vector3 d1 = seg1.endpoint - seg1.startpoint;	// �����P�̕���
				Vector3 d2 = seg2.endpoint - seg2.startpoint;	// �����Q�̕���
				Vector3 r = seg1.startpoint - seg2.startpoint;	// �����P�̎n�_�Ɛ����Q�̎n�_�����ԃx�N�g��

				float a = d1.Dot(d1);	// �����P�̒����̂Q��
				float e = d2.Dot(d2);	// �����Q�̒����̂Q��
				float f = d2.Dot(r);	// �����Q�̎n�_��������P�̎n�_�ւ̃x�N�g���̒����̂Q��

				// �����P���k�ނ��Ă��Ȃ����H�@���@�����Q���k�ނ��Ă��Ȃ����H
				if (a <= 1e-5 && e <= 1e-5) {
					s = t = 0.0f;
					p1 = seg1.startpoint;
					p2 = seg2.startpoint;
					return (p1 - p2).Length();		// �k�ނ��Ă���ꍇ�͓_�̋���
				}

				if (a <= 1e-5) {					// �����P���k�ނ��Ă���ꍇ
					s = 0.0f;
					t = std::clamp(f / e, 0.0f, 1.0f);
				}
				else {
					float c = d1.Dot(r);
					if (e <= 1e-5) {	// �����Q���k�ނ��Ă���ꍇ
						t = 0.0f;
						s = std::clamp(-c / a, 0.0f, 1.0f);
					}
					else {
						// �������m�̋��������߂�
						float b = d1.Dot(d2);

						// �s�񎮂����߂�
						float denom = a * e - b * b; // Always nonnegative

						// If segments are not parallel, compute closest point on L1 to L2 and
						// clamp to segment S1. Else pick arbitrary s (here 0)

						if (denom != 0.0f) {
							s = std::clamp((b * f - c * e) / denom, 0.0f, 1.0f);
						}
						else {
							s = 0.0f;
						}

						t = (b * s + f) / e;

						if (t < 0.0f) {
							t = 0.0f;
							s = std::clamp(-c / a, 0.0f, 1.0f);
						}
						else if (t > 1.0f) {
							t = 1.0f;
							s = std::clamp((b - c) / a, 0.0f, 1.0f);
						}
					}
				}

				p1 = seg1.startpoint + d1 * s;
				p2 = seg2.startpoint + d2 * t;
				return (p1 - p2).Length();

			}

			// ���W�ϊ��������AABB��S���_���狁�߂�
			BoundingBoxAABB calcAABB(
				const std::vector<Vector3>& vertices,
				SRT transform)
			{

				Matrix4x4 mtx = Matrix4x4::CreateFromYawPitchRoll(transform.rot.y, transform.rot.x, transform.rot.z);

				std::vector<Vector3> transformedVertices(vertices.size());	

				// ���W�ϊ�
				for (const auto& v : vertices) {
					transformedVertices.push_back(Vector3::Transform(v,mtx));
				}

				// AABB�̍ŏ��l�ƍő�l�����߂�
				BoundingBoxAABB aabb{};

				Vector3 minVec = transformedVertices.front();
				Vector3 maxVec = transformedVertices.front();

				for (const auto& v : transformedVertices) {
					minVec.x = std::min(minVec.x, v.x);
					minVec.y = std::min(minVec.y, v.y);
					minVec.z = std::min(minVec.z, v.z);

					maxVec.x = std::max(maxVec.x, v.x);
					maxVec.y = std::max(maxVec.y, v.y);
					maxVec.z = std::max(maxVec.z, v.z);
				}

				aabb.min.x = minVec.x;
				aabb.min.y = minVec.y;
				aabb.min.z = minVec.z;
				aabb.max.x = maxVec.x;
				aabb.max.y = maxVec.y;
				aabb.max.z = maxVec.z;

				return aabb;
			}


			// AABB����8���_���擾
			std::vector<Vector3> GetCorners(const BoundingBoxAABB& box) {
				const auto& min = box.min;
				const auto& max = box.max;

				return {
					Vector3(min.x, min.y, min.z),
					Vector3(max.x, min.y, min.z),
					Vector3(min.x, max.y, min.z),
					Vector3(max.x, max.y, min.z),
					Vector3(min.x, min.y, max.z),
					Vector3(max.x, min.y, max.z),
					Vector3(min.x, max.y, max.z),
					Vector3(max.x, max.y, max.z)
				};
			}

			// ���W�ϊ��������AABB�����߂�
			BoundingBoxAABB TransformAABB(
				const BoundingBoxAABB& inAABB,
				SRT transform)
			{

				std::vector<Vector3> corners = GetCorners(inAABB);	// AABB��8���_���擾

				// SRT��񂩂�s����擾	
				Matrix4x4 mtx = transform.GetMatrix();

				std::vector<Vector3> transformedVertices{};

				// ���W�ϊ�
				for (const auto& v : corners) {
					transformedVertices.push_back(Vector3::Transform(v, mtx));
				}

				// AABB�̍ŏ��l�ƍő�l�����߂�
				BoundingBoxAABB aabb{};

				Vector3 minVec = transformedVertices.front();
				Vector3 maxVec = transformedVertices.front();

				for (const auto& v : transformedVertices) {
					minVec.x = std::min(minVec.x, v.x);
					minVec.y = std::min(minVec.y, v.y);
					minVec.z = std::min(minVec.z, v.z);

					maxVec.x = std::max(maxVec.x, v.x);
					maxVec.y = std::max(maxVec.y, v.y);
					maxVec.z = std::max(maxVec.z, v.z);
				}

				aabb.min.x = minVec.x;
				aabb.min.y = minVec.y;
				aabb.min.z = minVec.z;
				aabb.max.x = maxVec.x;
				aabb.max.y = maxVec.y;
				aabb.max.z = maxVec.z;

				return aabb;
			}

			BoundingCapsule SetBoundingCapsule(
				SRT transform,		// �J�v�Z���̎p�����
				float radius,		// �J�v�Z���̔��a
				float height)		// �J�v�Z���̍���
			{
				BoundingCapsule boundingcapsule{};

				boundingcapsule.radius = radius;

				// �p����\���s������	
				Matrix4x4 mtx = Matrix4x4::CreateFromYawPitchRoll(
					transform.rot.y,
					transform.rot.x,
					transform.rot.z);

				Vector3 up = mtx.Up();

				boundingcapsule.startpoint = transform.pos;
				boundingcapsule.endpoint = transform.pos + height * up;

				return boundingcapsule;
			}

			BoundingCylinder SetBoundingCylinder(
				SRT transform,		// �~���̎p�����
				Cylinder cylinder)	// �`��p�̉~�����
			{
				BoundingCylinder boundingcylinder{};

				boundingcylinder.radius = cylinder.GetRadius();
				float height = cylinder.GetHeight();

				Matrix4x4 mtx = Matrix4x4::CreateFromYawPitchRoll(
					transform.rot.y,
					transform.rot.x,
					transform.rot.z);

				Vector3 up = mtx.Up();

				boundingcylinder.bottom = transform.pos;
				boundingcylinder.top = transform.pos + height * up;

				return boundingcylinder;
			}

			bool CollisionCylinder(
				const BoundingCylinder& Cylinder1,
				const BoundingCylinder& Cylinder2)
			{
				Segment seg1;
				seg1.startpoint = Cylinder1.bottom;
				seg1.endpoint = Cylinder1.top;

				Segment seg2;
				seg2.startpoint = Cylinder2.bottom;
				seg2.endpoint = Cylinder2.top;

				// �����Ɛ����̍ŒZ���������߂�
				Vector3 p1, p2;
				float s = 0.0f;
				float t = 0.0f;

				float distance = ClosestPtSegmentSegment(
					seg1,				// �����P
					seg2, 				// �����Q	
					s,					// �����P�̍ŒZ�����̃p�����[�^
					t,					// �����Q�̍ŒZ�����̃p�����[�^
					p1, 				// �����P�̍ŒZ�����̓_	
					p2); 				// �����Q�̍ŒZ�����̓_


				// �������m�̍ŒZ�������~���̔��a���傫���ꍇ�͏Փ˂��Ă��Ȃ�
				if (distance > Cylinder1.radius + Cylinder2.radius) {
					return false;	// �Փ˂��Ă��Ȃ�
				}

				return true;	// �Փ˂��Ă���
			}

			// �~���̒f��
			struct Cap {
				Vector3 center; // �~�̒��S�ʒu
				Vector3 normal; // �~�̖@���x�N�g���i���~���̎��Ɠ����j
				float radius;
			};


			bool CapCircleIntersect(const Cap& capA, const Cap& capB) {
				// Step 1: B�̒��S��A�̖@���Ɏˉe
				Vector3 delta = capB.center - capA.center;
				float distToPlane = delta.Dot(capA.normal);
				Vector3 projectedB = capB.center - capA.normal * distToPlane;

				// Step 2: A�̒��S�Ƃ̕��ʏ㋗��
				float planarDist = (projectedB - capA.center).Length();

				// Step 3: ���a�̍��v�Ɣ�r
				return planarDist <= (capA.radius + capB.radius);
			}

			bool CylinderCapIntersect(
				const BoundingCylinder& c1, 
				const BoundingCylinder& c2) 
			{
				// ���ꂼ��̏㉺�[�����߂�
				Vector3 top1 = c1.top;
				Vector3 top2 = c2.top;

				Vector3 c1axis = c1.top - c1.bottom;
				Vector3 c2axis = c2.top - c2.bottom;

				Cap c1_bottom = { c1.bottom, c1axis, c1.radius };
				Cap c1_top = { top1,         c1axis, c1.radius };
				Cap c2_bottom = { c2.bottom, c2axis, c2.radius };
				Cap c2_top = { top2,         c2axis, c2.radius };

				// 4�ʂ�̑g�ݍ��킹���m�F�i�㉺�~�㉺�j
				return
					CapCircleIntersect(c1_bottom, c2_bottom) ||
					CapCircleIntersect(c1_bottom, c2_top) ||
					CapCircleIntersect(c1_top, c2_bottom) ||
					CapCircleIntersect(c1_top, c2_top);
			}

			bool PreciseCylinderCollision(
				const BoundingCylinder& Cylinder1,
				const BoundingCylinder& Cylinder2)
			{
				// �����Ɛ����̍ŒZ���������߂�
				Vector3 p1;
				Vector3 p2;
				float s = 0.0f;
				float t = 0.0f;

				Segment seg1;
				seg1.startpoint = Cylinder1.bottom;
				seg1.endpoint = Cylinder1.top;

				Segment seg2;
				seg2.startpoint = Cylinder2.bottom;
				seg2.endpoint = Cylinder2.top;

				float distance = ClosestPtSegmentSegment(
					seg1,				// �����P
					seg2, 				// �����Q	
					s,					// �����P�̍ŒZ�����̃p�����[�^
					t,					// �����Q�̍ŒZ�����̃p�����[�^
					p1, 				// �����P�̍ŒZ�����̓_	
					p2); 				// �����Q�̍ŒZ�����̓_

				if (distance > Cylinder1.radius * Cylinder2.radius) {
					return false;
				}

				// �����f�ʃ`�F�b�N�i�㉺��cap�j
				bool sts = CylinderCapIntersect(Cylinder1, Cylinder2);
				if (sts) {
					return true;
				}

				// Step 3: �����cap�������̑��ʂɂ߂荞��ł���
				if (CylinderSideIntersect(Cylinder1, Cylinder2))
					return true;
				if (CylinderSideIntersect(Cylinder2, Cylinder1))
					return true;

				return false;

			}

			bool isPointOnSegment(
				const Segment& seg, 
				const Vector3& point, 
				float epsilon)
			{	
				Vector3 AB = seg.endpoint - seg.startpoint;
				Vector3 AP = point - seg.startpoint;
				Vector3 BP = point - seg.endpoint;

				// ����1: ���꒼����i�N���X�ς��[���j
				Vector3 cross = AB.Cross(AP);
				if (cross.LengthSquared() > epsilon * epsilon) {
					return false; // �������ɂȂ�
				}

				// ����2: ���ςŔ͈͂𔻒�
				float dot1 = AB.Dot(AP);
				float dot2 = (-AB).Dot(BP); // = AB.dot(P - B)

				return dot1 >= 0 && dot2 >= 0;
			}

			float ClosestDistanceBetweenLines(
				const Line& line1, 
				const Line& line2,
				float& s, 			// line1 �p�����[�^�i�������j
				float& t,			// line2 �p�����[�^�i�������j
				Vector3& p1, 		// line1 �ŋߐړ_
				Vector3& p2			// line2 �ŋߐړ_
			) 
			{
				const Vector3& p = line1.point;
				const Vector3& q = line2.point;
				const Vector3& d1 = line1.direction;
				const Vector3& d2 = line2.direction;

				Vector3 r = p - q;

				float a = d1.Dot(d1);       // d1�Ed1
				float e = d2.Dot(d2);       // d2�Ed2
				float f = d2.Dot(r);        // d2�E(p - q)
				float c = d1.Dot(r);        // d1�E(p - q)
				float b = d1.Dot(d2);       // d1�Ed2
				float denom = a * e - b * b;

				// ���s or ���꒼��
				if (std::abs(denom) < 1e-6f) {
					// �C�� s = 0 �ɂ��āAt ����`�ɉ���
					s = 0.0f;
					t = f / e;
				}
				else {
					s = (b * f - c * e) / denom;
					t = (a * f - b * c) / denom;
				}

				p1 = p + d1 * s;
				p2 = q + d2 * t;

				return (p1 - p2).Length();
			}

			bool CylinderSideIntersect(
				const BoundingCylinder& cylinderSide, 
				const BoundingCylinder& other)
			{
				// cylinderSide�̎��ƍ����x�N�g��
				Vector3 axis = cylinderSide.top - cylinderSide.bottom;
				float height = axis.Length();
				Vector3 axisDir = axis / height; // ���K��

				// other�̏㉺cap
				Vector3 otherCapCenters[2] = { other.bottom, other.top };

				for (const Vector3& point : otherCapCenters) {
					// cylinderSide�̎��ɓ_���ˉe
					Vector3 baseToPoint = point - cylinderSide.bottom;
					float h = baseToPoint.Dot(axisDir); // �ˉe����

					// ���̓_��cylinderSide�̍����͈͓��ɂ��邩�H
					if (h < 0.0f || h > height)
						continue;

					// ����̍ŋߐړ_�����߂�
					Vector3 closestPoint = cylinderSide.bottom + axisDir * h;

					// ���ʂ܂ł̐��������𑪂�
					float radialDist = (point - closestPoint).Length();

					if (radialDist <= cylinderSide.radius + other.radius) {
						return true;
					}
				}

				return false;
			}
		}
	}
}