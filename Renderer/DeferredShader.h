#pragma once

#include <d3d11.h>
#include<DirectXMath.h>
using namespace DirectX;

const int NumLayouts = 3;

class DeferredShader
{
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	DeferredShader();
	//DeferredShader(const DeferredShaderClass&);
	~DeferredShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*,int Startlocation);

	void SetInputLayout(ID3D11DeviceContext* deviceContext,int);
private:
	bool InitializeShader(ID3D11Device*, HWND);
	void ShutdownShader();

	bool SetShaderParameters(ID3D11DeviceContext*, XMMATRIX&, XMMATRIX&, XMMATRIX&, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int, int Startlocation);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout[NumLayouts];
	ID3D11SamplerState* m_sampleStateWrap;
	ID3D11Buffer* m_matrixBuffer;
};

