////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "cameraclass.h"
#include "cameraclass.h"
#include "../Matrix4.h"
#include "../Input/inputclass.h"
CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;
	PositionMatrix.Identity();
	CameraSpeed = 10;
	m_OldMousePosX = 800 / 2;
	m_OldMousePosY = 600 / 2;
	RotationMatrix.Identity();
	SetCursorPos(800, 600);

	CameraUp.x = 0;
	CameraUp.y = 1;
	CameraUp.z = 0;
	CameraUp.w = 0;

	MouseSpeed = 10;
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
	Vector4 temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	temp.w = 1;
	PositionMatrix.WAxis = temp;

	LocalMatrix.WAxis = temp;
	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;

	PositionMatrix.RotateX(XMConvertToRadians(x));
	PositionMatrix.RotateY(XMConvertToRadians(y));
	PositionMatrix.RotateZ(XMConvertToRadians(z));

	LocalMatrix.RotateX(XMConvertToRadians(x));
	LocalMatrix.RotateY(XMConvertToRadians(y));
	LocalMatrix.RotateZ(XMConvertToRadians(z));
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
	XMFLOAT3 front, Right;



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
	pitch = XMConvertToRadians(m_rotationX);
	yaw = XMConvertToRadians(m_rotationY);
	roll = XMConvertToRadians(m_rotationZ);


	//	Move in direction your facing
	//position.x +=5*( cosf(yaw)*cosf(pitch));
	//position.y +=5*( sinf(pitch));
	//position.z +=5*( sinf(yaw) *cosf(pitch));
	//XMStoreFloat3(&cameraFront, XMVector3Normalize(XMLoadFloat3(&front)));
	//Do_Movement();
	////cameraFront.x *= 5;
	//lookAt = XMFLOAT3(position.x + cameraFront.x, position.y + cameraFront.y, position.z + cameraFront.z);
	//
	//m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));

	// Create the rotation matrix from the yaw, pitch, and roll values.
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	XMStoreFloat3(&lookAt, XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix));

	//cameraFront = XMFLOAT3(lookAt.x - position.x, lookAt.y - position.y, lookAt.z - position.z);
	//XMStoreFloat3(&cameraFront,XMVector3Normalize(XMLoadFloat3(&cameraFront)));
	//XMStoreFloat3(&Right, XMVector3Cross(XMLoadFloat3(&cameraFront), XMLoadFloat3(&up)));
	//
	//XMStoreFloat3(&Right, XMVector3Normalize(XMLoadFloat3(&Right)));
	//
	//position.x += Right.x + 5;
	//position.y += Right.y + 5;
	//position.z += Right.z + 5;

	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);
	XMStoreFloat3(&up, XMVector3TransformCoord(XMLoadFloat3(&up), rotationMatrix));

	// Translate the rotated camera position to the location of the viewer.
	lookAt = XMFLOAT3(position.x + lookAt.x, position.y + lookAt.y, position.z + lookAt.z);

	// Finally create the view matrix from the three updated vectors.
	//D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);
	m_viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up));
	//Matrix4 temp;
	//temp= PositionMatrix * RotationMatrix;
	//m_viewMatrix = temp.ConvertToXMMatrix();
	//m_viewMatrix = PositionMatrix.ConvertToXMMatrix();
	//m_viewMatrix = XMMatrixInverse(NULL, m_viewMatrix);
	//Vector4 forwardtodoawn;
	//forwardtodoawn.x = LocalMatrix.ZAxis.x*moveOffset.x;
	//forwardtodoawn.y = LocalMatrix.ZAxis.y*moveOffset.y;
	//forwardtodoawn.z = LocalMatrix.ZAxis.z*moveOffset.z;
	//forwardtodoawn.w = LocalMatrix.ZAxis.w;
	//
	//LocalMatrix.WAxis += forwardtodoawn;
	//
	m_viewMatrix = LocalMatrix.ConvertToXMMatrix();

	return;

}

// Wait for update to do acual move
//
void CameraClass::Move(XMFLOAT3 Move)
{
	moveOffset.x = Move.x;
	moveOffset.y = Move.y;
	moveOffset.z = Move.z;
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



	//XMMatrixLookAtLH()

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

void CameraClass::MouseRotation()
{
	float OffsetX, OffsetY;
	POINT mousePos;
	GetCursorPos(&mousePos);
	SetCursorPos(400, 300);

	int x = 400;
	int y = 300;

	OffsetX = -float(x - mousePos.x)*DeltaTime*MouseSpeed;
	OffsetY = -float(mousePos.y - y)*DeltaTime*MouseSpeed;

	static bool FirstRun = true;
	if (!FirstRun)
	{
		m_rotationX += OffsetX;
		if (m_rotationX > 360)
		{
			m_rotationX = 0;
		}
		if (m_rotationX < 0)
		{
			m_rotationX = 360;
		}
		m_rotationY += OffsetY;
	}
	FirstRun = false;
	/*RotationMatrix = RotationMatrix.Identity();

	RotationMatrix.GlobalRotation((RotationMatrix.RotateY(XMConvertToRadians(m_rotationX))));
	RotationMatrix.GlobalRotation((RotationMatrix.RotateX(XMConvertToRadians(m_rotationY))));

	if (m_rotationX > 89.0f)
	{
	RotationMatrix.GlobalRotation((RotationMatrix.RotateX(XMConvertToRadians(89))));
	m_rotationX = 89;
	}
	if (m_rotationX < -89.0f)
	{
	RotationMatrix.GlobalRotation((RotationMatrix.RotateX(XMConvertToRadians(-89))));
	m_rotationX = -89;
	}*/

	if (m_rotationY >= 89.0f)
	{
		m_rotationY = 89;
		//LocalMatrix.GlobalRotation(Matrix4::RotateX(XMConvertToRadians(m_rotationY)));
	}
	if (m_rotationY <= -89.0f)
	{
		m_rotationY = -89;
		//LocalMatrix.GlobalRotation(Matrix4::RotateX(XMConvertToRadians(m_rotationY)));
	}
	//CameraFront.z = cos(XMConvertToRadians(m_rotationY)) * cos(XMConvertToRadians(m_rotationX));
	//CameraFront.y = sin(XMConvertToRadians(m_rotationX));
	//CameraFront.x = sin(XMConvertToRadians(m_rotationY)) *cos(XMConvertToRadians(m_rotationX));
	//CameraFront.Normalize();

	//}
	/*if (OffsetX != 0 || OffsetY != 0)
		SetCursorPos(800, 600);*/
	//SetCursorPos()
	//static float temp = 0;
	//temp += DeltaTime * 1;
	//if (OffsetX != 0)
	//{
	//	//PositionMatrix.LocalRotation(PositionMatrix.RotateY(XMConvertToRadians(OffsetX)));
	//
	//	PositionMatrix.GlobalRotation((PositionMatrix.RotateY(XMConvertToRadians(OffsetX))));
	//
	//	m_rotationY = temp;
	//	//PositionMatrix.RotateX(XMConvertToRadians(OffsetY));
	//}
	//if (OffsetY != 0)
	//{
	//
	//}
}
void CameraClass::Movement()
{

	if (moveOffset.z > 0)
	{
		m_positionX -= LocalMatrix.ZAxis.x  *DeltaTime * 10;// moveOffset.z;
		m_positionY -= LocalMatrix.ZAxis.y  *DeltaTime * 10;// moveOffset.z;
		m_positionZ += LocalMatrix.ZAxis.z  *DeltaTime * 10;// moveOffset.z;
	}
	if (moveOffset.z < 0)
	{
		m_positionX += LocalMatrix.ZAxis.x  *DeltaTime * 10;
		m_positionY += LocalMatrix.ZAxis.y  *DeltaTime * 10;
		m_positionZ -= LocalMatrix.ZAxis.z  *DeltaTime * 10;
	}
	if (moveOffset.x > 0)
	{
		m_positionX += LocalMatrix.XAxis.x *DeltaTime * 10;//moveOffset.x;
		m_positionY -= LocalMatrix.XAxis.y *DeltaTime * 10;//moveOffset.x;
		m_positionZ -= LocalMatrix.XAxis.z *DeltaTime * 10;//moveOffset.x;
	}
	if (moveOffset.x < 0)
	{
		m_positionX -= LocalMatrix.XAxis.x *DeltaTime * 10;
		m_positionY += LocalMatrix.XAxis.y *DeltaTime * 10;
		m_positionZ += LocalMatrix.XAxis.z *DeltaTime * 10;
	}
	//m_positionX += LocalMatrix.ZAxis.x * moveOffset.x;
	//m_positionY += LocalMatrix.ZAxis.y * moveOffset.y;
	//m_positionZ += LocalMatrix.ZAxis.z * moveOffset.z;
}

void CameraClass::Render()
{
	LocalMatrix = LocalMatrix.Identity();

	//Jardan test

	MouseRotation();
	LocalMatrix.GlobalRotation(Matrix4::RotateY(XMConvertToRadians(m_rotationX)));
	LocalMatrix.GlobalRotation(Matrix4::RotateX(XMConvertToRadians(-m_rotationY))); // Remove - if you want flip controls

	if (moveOffset.x != 0 || moveOffset.y != 0 || moveOffset.z != 0)
		Movement();

	LocalMatrix.LocalTranslate(m_positionX, m_positionY, m_positionZ);
	//end jardan test


	//start test
	//MouseRotation();
	//
	//Vector4 Position;
	//Position.x = m_positionX;
	//Position.y = m_positionY;
	//Position.z = m_positionZ;
	//Position.w = 0;
	//
	//if (moveOffset.z > 0 || moveOffset.z < 0)
	//{
	//	Position.x += (CameraFront.x * moveOffset.x);
	//	Position.y += (CameraFront.y * moveOffset.y);
	//	Position.z += (CameraFront.z * moveOffset.z);
	//}
	//if (moveOffset.x > 0 || moveOffset.x < 0)
	//{
	//	Position.x -= Vector4::CrossProduct(CameraFront, CameraUp).x* moveOffset.x;
	//	Position.y -= Vector4::CrossProduct(CameraFront, CameraUp).y* moveOffset.y;
	//	Position.z -= Vector4::CrossProduct(CameraFront, CameraUp).z* moveOffset.z;
	//}
	//
	//m_positionX = Position.x;
	//m_positionY = Position.y;
	//m_positionZ = Position.z;
	//
	//LocalMatrix.GlobalTranslate(m_positionX, m_positionY, m_positionZ);
	//LocalMatrix.GlobalRotation(Matrix4::RotateY(XMConvertToRadians(m_rotationX)));
	//LocalMatrix.GlobalRotation(Matrix4::RotateX(XMConvertToRadians(m_rotationY)));
	//LocalMatrix.GlobalRotation(Matrix4::RotateZ(XMConvertToRadians(0)));
	/// end test

	//LocalMatrix.GlobalTranslate(m_positionX, m_positionY, m_positionZ);
	//MouseRotation();
	////LocalMatrix = LocalMatrix.Inverse();

	//LocalMatrix.LocalRotation(Matrix4::RotateY(XMConvertToRadians(m_rotationX)));
	//LocalMatrix.LocalRotation(Matrix4::RotateX(XMConvertToRadians(m_rotationY)));
	////LocalMatrix.GlobalRotation(Matrix4::RotateZ(XMConvertToRadians(0)));


	//if (moveOffset.x != 0 || moveOffset.y != 0 || moveOffset.z != 0)
	//	Movement();
	////LocalMatrix.LocalTranslate(Position);
	////LocalMatrix = LocalMatrix.Inverse();

	////Movement();
	////LocalMatrix.GlobalTranslate(temp);

	////LocalMatrix.LocalTranslate(m_positionX, m_positionY, m_positionZ);

	////LocalMatrix.LocalTranslate(moveOffset.x, moveOffset.y, moveOffset.z);
	////LocalMatrix.LocalTranslate(m_positionX, m_positionY, m_positionZ);

	////
	////m_positionX = LocalMatrix.WAxis.x;
	////m_positionY = LocalMatrix.WAxis.y;
	////m_positionZ = LocalMatrix.WAxis.z;

	////Movement();
	////if (moveOffset.z != 0)
	////{
	////	LocalMatrix.WAxis += LocalMatrix.ZAxis * (DeltaTime*moveOffset.z);
	////	m_positionX = LocalMatrix.WAxis.x;
	////	m_positionY = LocalMatrix.WAxis.y;
	////	m_positionZ = LocalMatrix.WAxis.z;
	////}
	////LocalMatrix.LocalTranslate(moveOffset.x, moveOffset.y, moveOffset.z);

	SetupViewMat();


	moveOffset.x = 0;
	moveOffset.y = 0;
	moveOffset.z = 0;
	//Movement();

}

void CameraClass::ResetMousePos()
{
	SetCursorPos(800, 600);
}

void CameraClass::MoveForward()
{
	PositionMatrix.LocalRotation(Matrix4::RotateX(XMConvertToRadians(m_rotationX)));

	PositionMatrix = PositionMatrix* RotationMatrix;
	PositionMatrix.WAxis += PositionMatrix.ZAxis * (DeltaTime*CameraSpeed);
	m_positionX = PositionMatrix.Wx;
	m_positionY = PositionMatrix.Wy;
	m_positionZ = PositionMatrix.Wz;
}

void CameraClass::MoveBackwards()
{
	PositionMatrix.WAxis -= PositionMatrix.ZAxis *  (DeltaTime*CameraSpeed);
	m_positionX = PositionMatrix.Wx;
	m_positionY = PositionMatrix.Wy;
	m_positionZ = PositionMatrix.Wz;
}

void CameraClass::MoveLeft()
{
	PositionMatrix.WAxis -= PositionMatrix.XAxis *  (DeltaTime*CameraSpeed);
	m_positionX = PositionMatrix.Wx;
	m_positionY = PositionMatrix.Wy;
	m_positionZ = PositionMatrix.Wz;
}

void CameraClass::MoveRight()
{
	PositionMatrix.WAxis += PositionMatrix.XAxis *  (DeltaTime*CameraSpeed);
	m_positionX = PositionMatrix.Wx;
	m_positionY = PositionMatrix.Wy;
	m_positionZ = PositionMatrix.Wz;
}
