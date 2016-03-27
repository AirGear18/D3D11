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
#include "../Matrix4.h"

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

	void MoveForward();
	void MoveBackwards();
	void MoveLeft();
	void MoveRight();
	void ResetMousePos();
	void MouseRotation();
	void SetDeltaTime(float t) { DeltaTime = t; }

	void Movement();

	void Move(XMFLOAT3);
	void SetCamSpeed(float t) { CameraSpeed = t; }
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_rotationX, m_rotationY, m_rotationZ;
	XMMATRIX m_viewMatrix, m_baseViewMatrix;
	float DeltaTime, CameraSpeed, m_OldMousePosX, m_OldMousePosY;
	Vector4 moveOffset;
	bool m_DirtyRotation;
	bool m_DirtyPosition;

	Matrix4 PositionMatrix, RotationMatrix;
	Matrix4 WorldMatrix, LocalMatrix;

	Vector4 CameraUp, CameraFront, CameraRight, CameraDirection;

	float MouseSpeed;
};

#endif