#pragma once

struct DebugStruct
{
	float x, y, z, w;
	float r, g, b, a;
};


class ShapeDebug
{
	ShapeDebug() = default;
	~ShapeDebug() = default;

	ID3D11Buffer *m_vertexBuffer;
public:

	static ShapeDebug*GetInstance();
	//functions
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	void Initialize();
};

