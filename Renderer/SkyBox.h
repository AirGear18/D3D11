#pragma once
class DeferredShader;

class SkyBox
{
	XMMATRIX m_WorldMatrix;
public:
	SkyBox();
	~SkyBox();
	ID3D11Buffer* m_vertex;
	int m_indiceStartLocation;

	void Renderer(XMMATRIX& ViewMatrix, XMMATRIX&projectionMatrix, DeferredShader * def);
};

