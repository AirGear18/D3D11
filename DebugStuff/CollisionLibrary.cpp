#include "../pch.h"
#include "../DebugStuff/CollisionLibrary.h"

// ComputePlane
//
// Calculate the plane normal and plane offset from the input points
void ComputePlane(Plane &plane, const Vector4& pointA, const Vector4& pointB, const Vector4 &pointC)
{
	plane.normal = Vector4::CrossProduct(pointB - pointA, pointC - pointB);
	plane.normal.Normalize();
	plane.offset = Vector4::Dot_Product(plane.normal, pointA);
}

// ClassifyPointToPlane
//
// Perform a half-space test. Returns 1 if the point is on or in front of the plane.
// Returns 2 if the point is behind the plane.
int ClassifyPointToPlane(const Plane& plane, const Vector4& point)
{
	if (Vector4::Dot_Product(plane.normal, point) - plane.offset > 0 || Vector4::Dot_Product(plane.normal, point) - plane.offset == 0)
		return 1;
	else
		return 2;
}

// ClassifySphereToPlane
//
// Perform a sphere-to-plane test. 
// Returns 1 if the sphere is in front of the plane.
// Returns 2 if the sphere is behind the plane.
// Returns 3 if the sphere straddles the plane.
int ClassifySphereToPlane(const Plane& plane, const Sphere& sphere)
{
	float DisToPlane = Vector4::Dot_Product(plane.normal, sphere.m_Center) - plane.offset + sphere.m_Radius * 2;
	if (DisToPlane > sphere.m_Radius)
		return 1;
	else if (DisToPlane < sphere.m_Radius)
		return 2;
	else
		return 3;
}

// ClassifyAabbToPlane
//
// Performs a AABB-to-plane test.
// Returns 1 if the aabb is in front of the plane.
// Returns 2 if the aabb is behind the plane.
// Returns 3 if the aabb straddles the plane.
int ClassifyAabbToPlane(const Plane& plane, const AABB& aabb)
{
	Vector4 CenterPoint = (aabb.min + aabb.max)*0.5f;
	Vector4 Extents = aabb.max - CenterPoint;

	float r = Extents.x * abs(plane.normal.x) + Extents.y * abs(plane.normal.y) + Extents.z * abs(plane.normal.z);
	Sphere test;
	test.m_Center = CenterPoint;
	test.m_Radius = r;

	return ClassifySphereToPlane(plane, test);

}

// ClassifyCapsuleToPlane
//
// Performs a Capsule-to-plane test.
// Returns 1 if the aabb is in front of the plane.
// Returns 2 if the aabb is behind the plane.
// Returns 3 if the aabb straddles the plane.
int ClassifyCapsuleToPlane(const Plane& plane, const Capsule& capsule)
{
	float Sphere1 = Vector4::Dot_Product(plane.normal, capsule.m_Segment.m_Start) - plane.offset + capsule.m_Radius * 2;
	float Sphere2 = Vector4::Dot_Product(plane.normal, capsule.m_Segment.m_End) - plane.offset + capsule.m_Radius * 2;

	if (Sphere1 > capsule.m_Radius&& Sphere2 > capsule.m_Radius)
		return 1;
	else if (Sphere1 < capsule.m_Radius && Sphere2 < capsule.m_Radius)
		return 2;
	else
		return 3;
}

//// BuildFrustum
////
//// Calculates the corner points and planes of the frustum based upon input values.
//// Should call ComputePlane.
//void BuildFrustum(Frustum& frustum, float fov, float nearDist, float farDist, float ratio, const matrix4f& camXform)
//{
//	//float ViewRatio; //change this
//	Vector4 NearCenter = camXform.axis_pos - camXform.axis_z*nearDist;
//	Vector4 FarCenter = camXform.axis_pos - camXform.axis_z*farDist;
//	float Hnear, Hfar, Wfar, Wnear;
//	Hnear = 2 * tan(fov / 2)*nearDist;
//	Hfar = 2 * tan(fov / 2)*farDist;
//
//	Wnear = Hnear*ratio;
//	Wfar = Hfar *ratio;
//
//	frustum.corners[FTL] = FarCenter + camXform.axis_y*(Hfar*.5f) - camXform.axis_x*(Wfar*0.5f);
//	frustum.corners[FTR] = FarCenter + camXform.axis_y*(Hfar*.5f) + camXform.axis_x*(Wfar*0.5f);
//	frustum.corners[FBL] = FarCenter - camXform.axis_y*(Hfar*.5f) - camXform.axis_x*(Wfar*0.5f);
//	frustum.corners[FBR] = FarCenter - camXform.axis_y*(Hfar*.5f) + camXform.axis_x*(Wfar*0.5f);
//
//	frustum.corners[NTL] = NearCenter + camXform.axis_y*(Hnear*.5f) - camXform.axis_x*(Wnear*0.5f);
//	frustum.corners[NTR] = NearCenter + camXform.axis_y*(Hnear*.5f) + camXform.axis_x*(Wnear*0.5f);
//	frustum.corners[NBL] = NearCenter - camXform.axis_y*(Hnear*.5f) - camXform.axis_x*(Wnear*0.5f);
//	frustum.corners[NBR] = NearCenter - camXform.axis_y*(Hnear*.5f) + camXform.axis_x*(Wnear*0.5f);
//
//
//	ComputePlane(frustum.planes[NEAR_PLANE], frustum.corners[NTR], frustum.corners[NBR], frustum.corners[NBL]);
//	ComputePlane(frustum.planes[RIGHT_PLANE], frustum.corners[FTR], frustum.corners[FBR], frustum.corners[NBR]);
//	ComputePlane(frustum.planes[FAR_PLANE], frustum.corners[FBL], frustum.corners[FBR], frustum.corners[FTR]);
//	ComputePlane(frustum.planes[LEFT_PLANE], frustum.corners[NTL], frustum.corners[NBL], frustum.corners[FBL]);
//	ComputePlane(frustum.planes[TOP_PLANE], frustum.corners[NTR], frustum.corners[NTL], frustum.corners[FTL]);
//	ComputePlane(frustum.planes[BOTTOM_PLANE], frustum.corners[FBL], frustum.corners[NBL], frustum.corners[NBR]);
//
//
//
//}
//
//// FrustumToSphere
////
//// Perform a Sphere-to-Frustum check. Returns true if the sphere is inside. False if not.
//bool FrustumToSphere(const Frustum& frustum, const Sphere& sphere)
//{
//	for (size_t i = 0; i < 6; i++)
//	{
//		if (ClassifySphereToPlane(frustum.planes[i], sphere) == 2)
//			return false;
//	}
//	return true;
//
//}
//
//// FrustumToAABB
////
//// Perform a Aabb-to-Frustum check. Returns true if the aabb is inside. False if not.
//bool FrustumToAABB(const Frustum& frustum, const AABB& aabb)
//{
//	for (size_t i = 0; i < 6; i++)
//	{
//		if (ClassifyAabbToPlane(frustum.planes[i], aabb) == 2)
//			return false;
//	}
//	return true;
//
//}
//
//// FrustumToCapsule
////
//// Perform a Capsule-to-Frustum check. Returns true if the Capsule is inside. False if not.
//bool FrustumToCapsule(const Frustum& frustum, const Capsule& capsule)
//{
//	for (size_t i = 0; i < 6; i++)
//	{
//		if (ClassifyCapsuleToPlane(frustum.planes[i], capsule) == 2)
//			return false;
//	}
//	return true;
//
//}

// AABBtoAABB
//
// Returns true if the AABBs collide. False if not.
bool AABBtoAABB(const AABB& lhs, const AABB& rhs)
{
	if (lhs.max.x < rhs.min.x || lhs.min.x >  rhs.max.x) return false;
	if (lhs.max.y < rhs.min.y || lhs.min.y >  rhs.max.y) return false;
	if (lhs.max.z < rhs.min.z || lhs.min.z >  rhs.max.z) return false;
	return true;


}

// SphereToSphere
//
// Returns true if the Spheres collide. False if not.
bool SphereToSphere(const Sphere& lhs, const Sphere& rhs)
{
	//Vector4 center = lhs.m_Center - rhs.m_Center;
	float radius = lhs.m_Radius + rhs.m_Radius;


	Vector4 DisSqr;
	DisSqr.x = lhs.m_Center.x - rhs.m_Center.x;
	DisSqr.y = lhs.m_Center.y - rhs.m_Center.y;
	DisSqr.z = lhs.m_Center.z - rhs.m_Center.z;
	radius *= radius;
	return Vector4::Dot_Product(DisSqr, DisSqr) < radius;

}

// SphereToAABB
//
// Returns true if the sphere collides with the AABB. False if not.
bool SphereToAABB(const Sphere& lhs, const AABB& rhs)
{

	if (lhs.m_Center.x + lhs.m_Radius< rhs.min.x || lhs.m_Center.x - lhs.m_Radius>  rhs.max.x) return false;
	if (lhs.m_Center.y + lhs.m_Radius< rhs.min.y || lhs.m_Center.y - lhs.m_Radius>  rhs.max.y) return false;
	if (lhs.m_Center.z + lhs.m_Radius< rhs.min.z || lhs.m_Center.z - lhs.m_Radius>  rhs.max.z) return false;
	return true;


}

// CapsuleToSphere
//
// Returns true if the capsule collides with the sphere. False if not.
bool CapsuleToSphere(const Capsule& capsule, const Sphere& sphere)
{
	Vector4 ClosestPoint;
	Vector4 cylCenterVector = capsule.m_Segment.m_End - capsule.m_Segment.m_Start;
	Vector4 N = cylCenterVector / cylCenterVector.Normalize();

	Vector4 V = sphere.m_Center - capsule.m_Segment.m_Start;
	float D = Vector4::Dot_Product(N, V);
	/*if (D < 0)
	ClosestPoint = capsule.m_Segment.m_Start;
	else if (D>cylCenterVector.Normalize())
	ClosestPoint = capsule.m_Segment.m_End;
	else
	{
	N = N*D;
	ClosestPoint = capsule.m_Segment.m_Start + N;
	}*/
	auto OtherD = Vector4::Dot_Product(V, cylCenterVector) / Vector4::Dot_Product(cylCenterVector, cylCenterVector);
	ClosestPoint = capsule.m_Segment.m_Start + (cylCenterVector * OtherD);


	auto D1 = Vector4::Dot_Product(sphere.m_Center - ClosestPoint, sphere.m_Center - ClosestPoint);
	return D1 < sphere.m_Radius*sphere.m_Radius;
	Vector4 DisSqr;



	DisSqr.x = cylCenterVector.x - sphere.m_Center.x;
	DisSqr.y = cylCenterVector.y - sphere.m_Center.y;
	DisSqr.z = cylCenterVector.z - sphere.m_Center.z;

	return Vector4::Dot_Product(DisSqr, DisSqr) < 0;



	float Thedistance = Vector4::Dot_Product(sphere.m_Center, capsule.m_Segment.m_End) / Vector4::Dot_Product(cylCenterVector, cylCenterVector);
	if (Thedistance < 0) Thedistance = 0;
	if (Thedistance > 1) Thedistance = 1;

	Vector4 closestPoint = capsule.m_Segment.m_End + (cylCenterVector * Thedistance);

	Vector4 collisionVector = sphere.m_Center - closestPoint;
	Vector4 Thedistance2 = collisionVector;
	Vector4 collisionNormal = collisionVector / Thedistance2;


	return true;
}