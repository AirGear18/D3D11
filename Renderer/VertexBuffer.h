#pragma once
#include <d3d11.h>
#include "../Definds.h"
//using namespace DirectX;


class VertexBuffer
{
	ID3D11Buffer *vertexBufferPtr;
public:

	static VertexBuffer* GetInstance();
	VertexBuffer();
	~VertexBuffer();
	/// Adds new vertices to the vertex buffer.
	/// \param verts - The vertices to be added.
	/// \param numVerts - The number of vertices to be added.
	/// \return Returns the location in the vertex buffer where the added 
	/// vertices begin.  This is needed for rendering.
	UINT AddVerts(const VertexInputType *verts, UINT _numVerts);
	
	ID3D11Buffer *const GetVertexBuffer();
};

