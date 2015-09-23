#include "../pch.h"
#include "PointLight.h"


PointLight::PointLight(XMFLOAT4 Color, XMFLOAT3 Pos, XMFLOAT3 Rot)
{
	m_Color = Color;
	m_Position = Pos;
	m_Rotation = Rot;
}


PointLight::~PointLight()
{

}

void PointLight::Update(float DeltaTime)
{

}
void PointLight::Renderer(XMMATRIX&, XMMATRIX&, DeferredShader*)
{

}
