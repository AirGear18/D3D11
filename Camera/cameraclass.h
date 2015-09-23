////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_


//////////////
// INCLUDES //
//////////////
#include <DirectXPackedVector.h>
#include<DirectXMath.h>
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: CameraClass
////////////////////////////////////////////////////////////////////////////////
class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	void SetupViewMat();
	void Render();
	void GetViewMatrix(XMMATRIX&);

	void RenderBaseViewMatrix();
	void GetBaseViewMatrix(XMMATRIX&);

	void translationlocalX(float);
	void translationlocalY(float);
	void translationlocalZ(float);
	void translationlocal(XMFLOAT3);

	void  translationGlobalX(float);
	void  translationGlobalY(float);
	void  translationGlobalZ(float);

private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix, m_baseViewMatrix;
};

#endif