#pragma once
#include "BaseObject.h"

#include "../ModelObject.h"

class BaseRenderer : public BaseObject
{
protected:
	DirectX::XMMATRIX m_WorldMatrix;
public:
	
	BaseRenderer(int ObjectType,const char *FilePath, const char * DDSLocation, bool AttachTo, BaseObject *ObjectToAttackTo, DirectX::XMFLOAT3 OffsetAttachLocation, const char *NameRendererObject);
	~BaseRenderer();
	virtual void Update(float DeltaTime) override;
	virtual void Renderer(XMMATRIX&, XMMATRIX&, DeferredShader *) override;

	ModelObject m_Model;
};

