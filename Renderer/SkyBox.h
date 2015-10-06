#pragma once
class DeferredShader;
#include "../ModelObject.h"

class SkyBox
{
	XMMATRIX m_WorldMatrix;
	ModelObject m_SkyBoxModel;
public:
	SkyBox();
	~SkyBox();
	ID3D11Buffer* m_vertex;
	int m_indiceStartLocation;

	void Renderer(XMMATRIX& ViewMatrix, XMMATRIX&projectionMatrix, DeferredShader * def);
};

