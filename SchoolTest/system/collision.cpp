#include	<SimpleMath.h>
#include	"collision.h"

using namespace DirectX::SimpleMath;

// 線分に平行な与えられた幅の４角形の座標を求める
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3* v,
	float width)
{

	// 垂直なベクトルを求める
	Vector3 zaxis(0, 0, 1);
	Vector3 line = endpos - startpos;

	Vector3 ans;
	ans = line.Cross(zaxis);					// 外積
	ans.Normalize();							// 正規化

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;

}

// 線分に平行な与えられた幅の４角形の座標を求める
void CalcQuadOrientedLine(
	DirectX::SimpleMath::Vector3 startpos,
	DirectX::SimpleMath::Vector3 endpos,
	DirectX::SimpleMath::Vector3 up,
	DirectX::SimpleMath::Vector3* v,
	float width)
{

	// 垂直なベクトルを求める
	Vector3 zaxis=up;
	Vector3 line = endpos - startpos;



	Vector3 ans;
	ans = line.Cross(zaxis);					// 外積
	ans.Normalize();							// 正規化

	Vector3 startposplus = startpos + ans * width / 2.0f;
	Vector3 startposminus = startpos - ans * width / 2.0f;

	Vector3 endposplus = endpos + ans * width / 2.0f;
	Vector3 endposminus = endpos - ans * width / 2.0f;

	v[0] = startposplus;
	v[1] = endposplus;

	v[2] = startposminus;
	v[3] = endposminus;

}



// 直線と点の距離を求める
float calcPointLineDist(
	const Vector3& point,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = 0.0f;

	// 線分のベクトルを求める
	double ABx = segment.endpoint.x - segment.startpoint.x;
	double ABy = segment.endpoint.y - segment.startpoint.y;
	double ABz = segment.endpoint.z - segment.startpoint.z;

	// 線分の始点と点を結ぶベクトルを求める
	double APx = point.x - segment.startpoint.x;
	double APy = point.y - segment.startpoint.y;
	double APz = point.z - segment.startpoint.z;

	// 線分ABの長さを求める
	double AB2 = ABx * ABx + ABy * ABy + ABz * ABz;

	// ABベクトルとAPベクトルの内積を計算する（射影した長さを求める）
	double ABdotAP = ABx * APx + ABy * APy + ABz * APz;

	// ttを計算
	double tt = ABdotAP / AB2;

	// 垂線の足
	intersectionpoint.x = static_cast<float>(segment.startpoint.x + ABx * tt);
	intersectionpoint.y = static_cast<float>(segment.startpoint.y + ABy * tt);
	intersectionpoint.z = static_cast<float>(segment.startpoint.z + ABz * tt);

	t = static_cast<float>(tt);
	// 垂線の足の長さ
	distance = (intersectionpoint - point).Length();
	return distance;
}

// 線分と直線の長さを求める
float calcPointSegmentDist(
	const Vector3& p,
	const Segment& segment,
	Vector3& intersectionpoint,
	float& t) {

	float distance = calcPointLineDist(p, segment, intersectionpoint, t);

	// 交点が線分の外にある（始点に近い）
	if (t < 0.0f) {

		intersectionpoint = segment.startpoint;		// 開始点が交点

		float l = (p - intersectionpoint).Length();	// 交点との距離を求める

		return l;
	}

	// 交点が線分の外にある（終点に近い）
	if (t > 1.0f) {

		intersectionpoint = segment.endpoint;		// 終点が交点

		float l = (p - intersectionpoint).Length();	// 交点との距離を求める

		return l;
	}

	// 交点が線分上にある（０＜ｔ＜１）
	return distance;
}

// 線分と点の距離を求める方法を使用する
bool CollisionSphereCylinder(BoundingSphere sphere, BoundingCylinder cylinder)
{
	// 垂線の足
	Vector3 intersectionpoint;
	float t{};

	Segment seg;
	seg.startpoint = cylinder.bottom;
	seg.endpoint = cylinder.top;

	// 球の中心と線分の距離を求める
	float length = calcPointSegmentDist(
		sphere.center,
		seg,
		intersectionpoint,
		t);

	// 球の中心と線分の距離が半径より小さい場合は当たっている
	if (length > sphere.radius + cylinder.radius) {
		return false;
	}

	// 線分の始点に近い
	if(t<0){
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
	// 線分の終点に近い
	else if(t>1){
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
	// 線分の始点と終点の間にあるか？
	else {
		return true;
	}

	return false;
}

// 球
bool CollisionSphere(BoundingSphere p1, BoundingSphere p2)
{

	float length = (p1.center - p2.center).Length();

	if (p1.radius + p2.radius > length) {
		return false;
	}

	return true;
}

// AABB
bool CollisionAABB(BoundingBoxAABB p1, BoundingBoxAABB p2){

	// X座標
	if (p1.max.x < p2.min.x) {
		return false;
	}

	if (p1.min.x > p2.max.x) {
		return false;
	}

	// Y座標
	if (p1.max.y < p2.min.y) {
		return false;
	}

	if (p1.min.y > p2.max.y) {
		return false;
	}

	// Z座標
	if (p1.max.z < p2.min.z) {
		return false;
	}

	if (p1.min.z > p2.max.z) {
		return false;
	}

	return true;
}


BoundingBoxAABB SetAABB(Vector3 centerposition,float width ,float height,float depth) {

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

// AABBと点のもっと近い点を求める
void ClosestPtPointAABB(Vector3 p, BoundingBoxAABB aabb, Vector3& q)
{
	q.x = max(aabb.min.x, min(p.x, aabb.max.x));
	q.y = max(aabb.min.y, min(p.y, aabb.max.y));
	q.z = max(aabb.min.z, min(p.z, aabb.max.z));
}

// AABBと点の距離を求める（２乗）
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

// AABBと球の当たり判定
bool CollisionSphereAABB(
	BoundingSphere sphere,
	BoundingBoxAABB aabb)
{
	// AABBと点の距離を求める
	float lng = SqDistPointAABB(sphere.center, aabb);

	if (lng < sphere.radius * sphere.radius)
	{
		return true;
	}

	return false;
}

// OBBと点のもっと近い点を求める
void ClosestPtPointOBB(Vector3 point, BoundingBoxOBB obb, Vector3& answer)
{
	// 点からOBBの中心へのベクトル(差分を求めた)
	Vector3 diff = point - obb.worldcenter;

	answer = obb.worldcenter;

	// OBBのローカル座標に変換（軸は正規化されていることを前提とする）
	float obbaxisx = diff.Dot(obb.axisX);
	float obbaxisy = diff.Dot(obb.axisY);
	float obbaxisz = diff.Dot(obb.axisZ);

	// OBBの各軸に沿った最近接点を求める
	obbaxisx = std::clamp(obbaxisx, -0.5f * obb.lengthx, 0.5f * obb.lengthx);
	obbaxisy = std::clamp(obbaxisy, -0.5f * obb.lengthy, 0.5f * obb.lengthy);
	obbaxisz = std::clamp(obbaxisz, -0.5f * obb.lengthz, 0.5f * obb.lengthz);

	// ワールド座標に変換
	answer = obb.worldcenter + obb.axisX * obbaxisx + obb.axisY * obbaxisy + obb.axisZ * obbaxisz;
}

// OBBと点の距離を求める
float DistPointOBB(Vector3 point, BoundingBoxOBB obb)
{
	Vector3 answer{};

	// OBBと点のもっと近い点を求める
	ClosestPtPointOBB(point, obb, answer);
	// 距離を求める
	float length = (answer - point).Length();

	return length;
}

// OBBの設定
BoundingBoxOBB SetOBB(
	Vector3 rot,					// 姿勢（回転角度）
	Vector3 pos,					// 中心座標（ワールド）
	float width,					// 幅
	float height,					// 高さ
	float depth)					// 奥行
{
	BoundingBoxOBB obb{};

	BoundingBoxAABB aabb = SetAABB(pos, width, height, depth);
	obb.max = aabb.max;
	obb.min = aabb.min;

	Matrix mtx = Matrix::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);

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

// OBBと球の当たり判定
bool CollisionSphereOBB(
	BoundingSphere sphere,
	BoundingBoxOBB obb)
{
	// OBBと点の距離を求める
	float lng = DistPointOBB(sphere.center, obb);

	if (lng < sphere.radius)
	{
		return true;
	}

	return false;
}

// カプセル同士の当たり判定
bool CollisionCapsule(BoundingCapsule caps1, BoundingCapsule caps2)
{
	// 線分の始点と終点を求める
	Segment seg1;
	seg1.startpoint = caps1.startpoint;
	seg1.endpoint = caps1.endpoint;

	Segment seg2;
	seg2.startpoint = caps2.startpoint;
	seg2.endpoint = caps2.endpoint;

	// ２つの線分の最短距離を求める
	Vector3 p1, p2;
	float s, t;
	float length = ClosestPtSegmentSegment(seg1, seg2, s, t, p1, p2);

	// 球の半径を考慮して当たり判定
	if (length < caps1.radius + caps2.radius) {
		return true;
	}

	return false;
}

// 線分と線分の最短距離を求める
float ClosestPtSegmentSegment(
	const Segment& seg1,				// 線分１
	const Segment& seg2, 				// 線分２
	float& s,							// 線分１の最短距離のパラメータ
	float& t,							// 線分２の最短距離のパラメータ
	Vector3& p1, 						// 線分１の最短距離の点
	Vector3& p2) 						// 線分２の最短距離の点
{
	Vector3 d1 = seg1.endpoint - seg1.startpoint;	// 線分１の方向
	Vector3 d2 = seg2.endpoint - seg2.startpoint;	// 線分２の方向
	Vector3 r = seg1.startpoint - seg2.startpoint;	// 線分１の始点と線分２の始点を結ぶベクトル

	float a = d1.Dot(d1);	// 線分１の長さの２乗
	float e = d2.Dot(d2);	// 線分２の長さの２乗
	float f = d2.Dot(r);	// 線分２の始点から線分１の始点へのベクトルの長さの２乗

	// 線分１が縮退していないか？　且つ　線分２が縮退していないか？
	if (a <= 1e-5 && e <= 1e-5) {
		s = t = 0.0f;
		p1 = seg1.startpoint;
		p2 = seg2.startpoint;
		return (p1 - p2).Length();		// 縮退している場合は点の距離
	}

	if (a <= 1e-5) {		// 線分１が縮退している場合
		s = 0.0f;
		t = std::clamp(f / e, 0.0f, 1.0f);
	}
	else {
		float c = d1.Dot(r);
		if (e <= 1e-5) {	// 線分２が縮退している場合
			t = 0.0f;
			s = std::clamp(-c / a, 0.0f, 1.0f);
		}
		else {
			// 線分同士の距離を求める
			float b = d1.Dot(d2);

			// 行列式を求める
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



