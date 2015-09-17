#pragma once
#include <DirectXMath.h>
using namespace DirectX;

class DeferredShader;
class BaseObject
{
protected:
	enum ObjectType
	{
		STATIC,
		MOVEABLE,
	};
	int m_ObjectID;
	int m_Context_Type=-1;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;

public:
	BaseObject();
	virtual ~BaseObject();

	//Get
	inline XMFLOAT3 GetPosition() { return m_Position; }
	inline XMFLOAT3 GetRotation() { return m_Rotation; }
	//Sets
	inline void SetPosition(XMFLOAT3 Tposition) { m_Position = Tposition; }
	inline void SetRotation(XMFLOAT3 TRotation) { m_Rotation = TRotation; }
	inline void SetContextType(int Type) { m_Context_Type = Type; }
	int GetObjectType(){ return m_ObjectID; }

	virtual void Update(float DeltaTime);
	virtual void Renderer(XMMATRIX&, XMMATRIX&,DeferredShader*);

};

