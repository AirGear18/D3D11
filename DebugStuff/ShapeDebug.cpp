#include "../pch.h"
#include "ShapeDebug.h"
#include "../Renderer/DeferredRenderer.h"
#include <vector>

ShapeDebug*ShapeDebug::GetInstance()
{
	static ShapeDebug m_Instance;
	return &m_Instance;
}

void ShapeDebug::Initialize()
{
	std::vector< DebugStruct > verts;

	for (float phi = 0; phi < 2 * PI; phi += PI / 8) // Azimuth [0, 2PI]
	{
		for (float theta = 0.; theta < PI; theta += PI / 8) // Elevation [0, PI]
		{
			DebugStruct point;
			point.x = .5f * cos(phi) * sin(theta);
			point.y = .5f * sin(phi) * sin(theta);
			point.z = .5f           * cos(theta);
			verts.push_back(point);
		}
	}
}

void ShapeDebug::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexInputType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(DeferredRenderer::GetInstance()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);


	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	return;
}