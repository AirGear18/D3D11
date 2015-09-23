#include "../pch.h"
#include "Player.h"
#include "../Camera/cameraclass.h"
#include "../Input/inputclass.h"
#include "../ModelObject.h"
#include "../Renderer/DeferredRenderer.h"
#include "../Renderer/DeferredShader.h"
Player::Player()
{
	m_Model.LoadObject("Assets/Models/Trex2.obj", "Player", "Assets/Models/TrexTemp.dds");
	m_WorldMatrix = XMMatrixIdentity();
	m_Movement = XMFLOAT3(0, 0, 0);
	m_Position = XMFLOAT3(0, 0, 0);
	m_Rotation = XMFLOAT3(0, 0, 0);
}


Player::~Player()
{
}

void Player::Update(float DeltaTime)
{
	m_Position.x += m_Movement.x*DeltaTime;
	m_Position.y += m_Movement.y*DeltaTime;
	m_Position.z += m_Movement.z*DeltaTime;
	//Rotate
	m_WorldMatrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	//translate
	m_WorldMatrix *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	//size

	//Tells the camera here to move with player


	//

}

void Player::Input()
{
	if (InputClass::GetInstance()->IsUpPressed())
	{
		m_Movement.z += 5;
		//m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y + (10)*elapsedTime, m_Camera->GetPosition().z);
	}
	if (InputClass::GetInstance()->IsDownPressed())
	{
		m_Movement.z -= 5;
		//m_Camera->SetPosition(m_Camera->GetPosition().x, m_Camera->GetPosition().y - (10)*elapsedTime, m_Camera->GetPosition().z);
	}
	if (InputClass::GetInstance()->IsLeftPressed())
	{
		m_Movement.x -= 5;
		//m_Camera->SetPosition(m_Camera->GetPosition().x - (10)*elapsedTime, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	}
	if (InputClass::GetInstance()->IsRightPressed())
	{
		m_Movement.x += 5;
		//m_Camera->SetPosition(m_Camera->GetPosition().x + (10)*elapsedTime, m_Camera->GetPosition().y, m_Camera->GetPosition().z);
	}
	//will change for gravity later
	if (InputClass::GetInstance()->IsSpaceBarPressed())
	{
		m_Movement.y += 5;
	}
}

void Player::Renderer(XMMATRIX& ViewMatrix, XMMATRIX& projectionMatrix, DeferredShader * def)
{
	m_Model.RenderBuffers(DeferredRenderer::GetInstance()->GetDeviceContext());
	def->Render(DeferredRenderer::GetInstance()->GetDeviceContext(), m_Model.indexCount, m_WorldMatrix, ViewMatrix, projectionMatrix, m_Model.ShadeView, m_Model.StartPoint);

}