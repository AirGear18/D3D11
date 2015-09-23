#pragma once
#include "../Object/BaseObject.h"
class PointLight : public BaseObject
{
	XMFLOAT4 m_Color;

public:
	PointLight(XMFLOAT4 Color, XMFLOAT3 Pos, XMFLOAT3 Rot);
	virtual ~PointLight();

	virtual void Update(float DeltaTime);
	virtual void Renderer(XMMATRIX&, XMMATRIX&, DeferredShader*);

	//set
	void SetColor(XMFLOAT4 Color) { m_Color = Color; }
	//get
	XMFLOAT4 GetColor(){ return m_Color; }
};

