#pragma once
#include <DirectXMath.h>
using namespace DirectX;

#include "../ModelObject.h"
class DeferredShader;


class Player
{
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;

	XMFLOAT3 m_Movement;

	ModelObject m_Model;

	XMMATRIX m_WorldMatrix;

public:
	Player();
	~Player();

	//Get
	inline XMFLOAT3 GetPosition() { return m_Position; }
	inline XMFLOAT3 GetRotation() { return m_Rotation; }
	//Sets
	inline void SetPosition(XMFLOAT3 Tposition) { m_Position = Tposition; }
	inline void SetRotation(XMFLOAT3 TRotation) { m_Rotation = TRotation; }

	void Update(float DeltaTime);
	void Input();
	void Renderer(XMMATRIX&, XMMATRIX&, DeferredShader * def);
};

