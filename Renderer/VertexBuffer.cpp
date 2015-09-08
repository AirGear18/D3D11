#include "../pch.h"
#include "VertexBuffer.h"
#include "../Renderer/DeferredRenderer.h"




VertexBuffer::VertexBuffer()
{
}


VertexBuffer::~VertexBuffer()
{
}

VertexBuffer* VertexBuffer::GetInstance()
{
	static VertexBuffer instancePtr;	// stored in global memory once
	return &instancePtr;
}

UINT VertexBuffer::AddVerts(const VertexInputType *verts, UINT _numVerts)
{
	size_t ret;
	// Test if this buffer has already been finalized
	if (!vertexBufferPtr)
	{
		ret = 0;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(VertexInputType) * _numVerts;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = verts;
		DeferredRenderer::GetInstance()->GetDevice()->CreateBuffer(&vbd, &vinitData, &vertexBufferPtr);
	}
	else
	{
		D3D11_BUFFER_DESC ibd;

		vertexBufferPtr->GetDesc(&ibd);

		D3D11_SUBRESOURCE_DATA iinitData;
		UINT oldBuffeSize = ibd.ByteWidth / sizeof(VertexInputType);
		UINT newBuffeSize = oldBuffeSize + _numVerts;

		iinitData.pSysMem = new VertexInputType[newBuffeSize];

		memcpy((char *)(iinitData.pSysMem) + ibd.ByteWidth,
			verts, sizeof(VertexInputType) * _numVerts);

		ibd.ByteWidth += sizeof(VertexInputType) * _numVerts;
		ID3D11Buffer *newVertexBufferPtr;
		
		DeferredRenderer::GetInstance()->GetDevice()->CreateBuffer(&ibd, &iinitData, &newVertexBufferPtr);


		DeferredRenderer::GetInstance()->GetDeviceContext()->CopySubresourceRegion(newVertexBufferPtr, 0, 0, 0, 0,
			vertexBufferPtr, 0, 0);

		vertexBufferPtr->Release();
		vertexBufferPtr = 0;
		vertexBufferPtr = newVertexBufferPtr;

		delete[] iinitData.pSysMem;

		ret = oldBuffeSize;
	}
	return (UINT)ret;
}

ID3D11Buffer *const VertexBuffer::GetVertexBuffer()
{
	return vertexBufferPtr;
}