////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "cameraclass.h"
#include "cameraclass.h"
#include "../Matrix4.h"
CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;
	return;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(m_positionX, m_positionY, m_positionZ);
}


XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::SetupViewMat()
{
	XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX;
	yaw = m_rotationY;
	roll = m_rotationZ;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	XMStoreFloat3(&lookAt, XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix));


	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	XMStoreFloat3(&up, XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix));

	// Translate the rotated camera position to the location of the viewer.
	lookAt = XMFLOAT3(position.x + lookAt.x, position.y + lookAt.y, position.z + lookAt.z);

	//	Move in direction your facing
	/*XMFLOAT3 forwardVector;
	forwardVector.x = lookAt.x - position.x;
	forwardVector.y = lookAt.y - position.y;
	forwardVector.z = lookAt.z - position.z;
	XMVector3Normalize(XMLoadFloat3(&forwardVector));

	lookAt.x += forwardVector.x;
	lookAt.y += forwardVector.y;
	lookAt.z += forwardVector.z;

	position.x += forwardVector.x;
	position.y += forwardVector.y;
	position.z += forwardVector.z;*/

	// Finally create the view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));

	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void CameraClass::RenderBaseViewMatrix()
{
	XMFLOAT3 up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw = m_rotationY * 0.0174532925f;
	roll = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	XMStoreFloat3(&lookAt, XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix));

	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	XMStoreFloat3(&up, XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix));

	// Translate the rotated camera position to the location of the viewer.
	lookAt = XMFLOAT3(position.x + lookAt.x, position.y + lookAt.y, position.z + lookAt.z);

	// Finally create the base view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&m_baseViewMatrix, &position, &lookAt, &up);
	m_baseViewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));

	return;
}


void CameraClass::GetBaseViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_baseViewMatrix;
	return;
}

void CameraClass::translationlocalX(float)
{

}
void CameraClass::translationlocalY(float)
{

}
void CameraClass::translationlocalZ(float)
{

}
void CameraClass::Render()
{
	SetupViewMat();
	//XMMATRIX temp = XMMatrixInverse(nullptr, m_viewMatrix);
	//temp = temp + XMMatrixTranslation(m_positionX, m_positionY, m_positionZ);
	//temp =  XMMatrixRotationRollPitchYaw(m_rotationX, m_rotationY, m_rotationX) * temp;
	//m_viewMatrix = XMMatrixInverse(nullptr, temp);
	//static float temp = 0;
	//temp -=.001f;
	//m_viewMatrix *= XMMatrixTranslation(0, 0, temp);
	
 	//Matrix4 temp;
	//temp.Identity();
	//Vector4 loc;
	//loc.x = m_positionX;
	//loc.y = m_positionY;
	//loc.z = m_positionZ;
	//loc.w = 0;
	//
	//temp.GlobalTranslate(loc);
	//temp.RotateX(m_rotationX);
	

}
void CameraClass::translationlocal(XMFLOAT3 tras)
{
	XMMATRIX temp = XMMatrixInverse(nullptr, m_viewMatrix);
	temp = temp * XMMatrixTranslation(tras.x, tras.y, tras.z);
	m_viewMatrix = XMMatrixInverse(nullptr, temp);
}

void CameraClass::translationGlobalX(float)
{

}
void CameraClass::translationGlobalY(float)
{

}
void CameraClass::translationGlobalZ(float)
{

}