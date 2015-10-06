#pragma once
class DeferredBuffers;
class BaseLight
{
protected:
	enum ObjectType
	{
		STATIC,
		MOVEABLE,
	};
	int m_ObjectID;
	int m_ContextID;
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Rotation;
	XMMATRIX m_WorldMatric = XMMatrixIdentity();
public:
	BaseLight();
	virtual ~BaseLight();
	//Get
	inline XMFLOAT3 GetPosition() { return m_Position; }
	inline XMFLOAT3 GetRotation() { return m_Rotation; }
	//Sets
	inline void SetPosition(XMFLOAT3 Tposition) { m_Position = Tposition; }
	inline void SetRotation(XMFLOAT3 TRotation) { m_Rotation = TRotation; }
	int GetObjectType(){ return m_ObjectID; }

	virtual void Update(float DeltaTime)=0;
	virtual void Renderer(XMMATRIX&view, XMMATRIX& projection, DeferredBuffers*, XMFLOAT3 Cam) = 0;

	ID3D11Buffer* m_matrixBuffer;
};

