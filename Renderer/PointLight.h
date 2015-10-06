#pragma once
#include "BaseLight.h"
#include "../ModelObject.h"
class PointLight : public BaseLight
{
	XMFLOAT4 m_Color;
	ID3D11Buffer* m_PointLightBuffer;
	ID3D11Buffer* InvViewProjBuffer;
	ID3D11Buffer*m_vertexBuffer;

	UINT m_IndexStartLocation;

	ModelObject m_PointLightModel;
	float Range;

public:
	PointLight(XMFLOAT4 Color, XMFLOAT3 Pos, XMFLOAT3 Rot,int Type,int Context);
	virtual ~PointLight();

	virtual void Update(float DeltaTime);
	virtual void Renderer(XMMATRIX&view, XMMATRIX& projection, DeferredBuffers*, XMFLOAT3 Cam);

	//set
	void SetColor(XMFLOAT4 Color) { m_Color = Color; }
	//get
	XMFLOAT4 GetColor(){ return m_Color; }
	void UpdateConstBuffer(XMMATRIX&view, XMMATRIX& projection, DeferredBuffers*def, XMFLOAT3 Cam);
	
};

struct LightBuffer
{
	/// The position the light shines from.
	XMFLOAT4 position;

	XMFLOAT4 Attenuation;
	/// The color of the diffuse component of the light.
	XMFLOAT3 diffuseColor;
	/// The maximum distance this light should effect, adjusts the resulting attenuation to match
	float range;
	/// The color of the ambient component of the light.
	//float3 ambientColor;
	/// The color of the specular component of the light.
	//float3 specularColor;
	///(x = constant attenuation, y = linear attenuation, z = quadratic attenuation)
	XMFLOAT3 CameraPosition;
	/// Controls the brightness of the specualr reflection.
	//float specularIntensity;

	/// Controls how big the specular reflection is. A smaller value creates a large reflection.
	//float specularPower;
	float padding;
};

struct MatrixBufferType
{
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};
struct InvViewProjBufferType
{
	/// The current cameras current inverse view-projection matrix
	XMMATRIX gInvViewProj;
};