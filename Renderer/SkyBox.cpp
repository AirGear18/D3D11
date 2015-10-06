#include "../pch.h"
#include "SkyBox.h"
#include "DeferredRenderer.h"
#include "DeferredShader.h"
#include "VertexBufferManager.h"


SkyBox::SkyBox()
{
	//m_SkyBoxModel.SetNormalFlip(false);
	//m_SkyBoxModel.LoadObject("Assets/Models/Box.obj", "SkyBox", NULL, Posnormuv);

	if (VertexBufferManager::GetInstance()->CheckVertexBuffers("Box") == NULL)
	{
		D3D11_BUFFER_DESC Buff;
		D3D11_SUBRESOURCE_DATA  Data;
		VertexInputType OurVertices1[] =
		{


			{ XMFLOAT4(-1.0f, 1.0f, -1.0f, 0), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(1.0f, 1.0f, -1.0f, 0), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(1.0f, 1.0f, 1.0f, 0), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 0), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },

			{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 0), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(1.0f, -1.0f, -1.0f, 0), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(1.0f, -1.0f, 1.0f, 0), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
			{ XMFLOAT4(-1.0f, -1.0f, 1.0f, 0), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },

			{ XMFLOAT4(-1.0f, -50.0f, 50.0f, 0), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-50.0f, -50.0f, -50.0f, 0), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-50.0f, 50.0f, -50.0f, 0), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(-50.0f, 50.0f, 50.0f, 0), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(50.0f, -50.0f, 50.0f, 0), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(50.0f, -50.0f, -50.0f, 0), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(50.0f, 50.0f, -50.0f, 0), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ XMFLOAT4(50.0f, 50.0f, 50.0f, 0), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) },

			{ XMFLOAT4(-50.0f, -50.0f, -50.0f, 0), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			{ XMFLOAT4(50.0f, -50.0f, -50.0f, 0), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			{ XMFLOAT4(50.0f, 50.0f, -50.0f, 0), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },
			{ XMFLOAT4(-50.0f, 50.0f, -50.0f, 0), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },

			{ XMFLOAT4(-50.0f, -50.0f, 50.0f, 0), XMFLOAT2(1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT4(50.0f, -50.0f, 50.0f, 0), XMFLOAT2(0.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT4(50.0f, 50.0f, 50.0f, 0), XMFLOAT2(0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ XMFLOAT4(-50.0f, 50.0f, 50.0f, 0), XMFLOAT2(1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) },


			/*
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) }, //3
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },//0
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },//1
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f) },//2


			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) }, //4
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) }, // 6
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) }, //5
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },// 7

			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) }, // 11
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 1-.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) }, // 9
			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) }, //10
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0.0f, 0.0f) }, // 8


			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, //12
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, //14
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, //13
			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f) }, //15

			{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) },// 19
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) }, //17
			{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) }, // 18
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, -1.0f) }, //16

			{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, //22
			{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, //21
			{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, //20
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f) }, //23
			*/
		};
		//OurVertices[0].pos *= XMMatrixIdentity();
		ZeroMemory(&Buff, sizeof(Buff));
		Buff.ByteWidth = sizeof(VertexInputType) * 24;
		Buff.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Buff.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		Buff.StructureByteStride = sizeof(VertexInputType);
		Buff.Usage = D3D11_USAGE_DYNAMIC;
		// TODO: PART 2 STEP 3c
		Data.pSysMem = OurVertices1;
		Data.SysMemPitch = 0;
		Data.SysMemSlicePitch = 0;

		// TODO: PART 2 STEP 3d
		//ID3D11Buffer * temp;
		DeferredRenderer::GetInstance()->GetDevice()->CreateBuffer(&Buff, &Data, &m_vertex);

		UINT indices[] =
		{
			0, 1, 3,
			3, 1, 2,

			5, 4, 6,
			6, 4, 7,

			8, 9, 11,
			11, 9, 10,

			13, 12, 14,
			14, 12, 15,

			16, 17, 19,
			19, 17, 18,

			21, 20, 22,
			22, 20, 23
		};
		m_indiceStartLocation = DeferredRenderer::GetInstance()->AddIndices(indices, 36);
		//Buff.Usage = D3D11_USAGE_DEFAULT;
		//Buff.ByteWidth = sizeof(WORD) * 36;
		//Buff.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//Buff.CPUAccessFlags = 0;
		//Data.pSysMem = indices;
		//Device->CreateBuffer(&Buff, &Data, &IndexCubeBuffer);
		m_WorldMatrix = XMMatrixIdentity();
		m_WorldMatrix = XMMatrixScaling(1, 1, 1);

		VertexBufferManager::GetInstance()->AddVertexBuffers("Box", m_vertex, 36, m_indiceStartLocation);
	}
	else
	{
		m_vertex = VertexBufferManager::GetInstance()->CheckVertexBuffers("Box");
		m_indiceStartLocation = VertexBufferManager::GetInstance()->GetIndexStartPos("Box");
		m_WorldMatrix = XMMatrixIdentity();

		m_WorldMatrix = XMMatrixScaling(10, 10, 10);
	}
	//m_WorldMatrix = XMMatrixIdentity();
	//m_WorldMatrix = XMMatrixScaling(10, 10, 10);

}


SkyBox::~SkyBox()
{
	//if (m_vertex)
	//	m_vertex->Release();
}

void SkyBox::Renderer(XMMATRIX& ViewMatrix, XMMATRIX&projectionMatrix, DeferredShader * def)
{
	//m_SkyBoxModel.RenderBuffers(DeferredRenderer::GetInstance()->GetDeviceContext(), Posnormuv);

	unsigned int stride;
	unsigned int offset;
	
	// Set vertex buffer stride and offset.
	stride = sizeof(VertexInputType);
	offset = 0;
	
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	DeferredRenderer::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex, &stride, &offset);
	
	// Set the index buffer to active in the input assembler so it can be rendered.
	DeferredRenderer::GetInstance()->GetDeviceContext()->IASetIndexBuffer(DeferredRenderer::GetInstance()->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//Set Input
	def->SetInputLayout(DeferredRenderer::GetInstance()->GetDeviceContext(), 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	DeferredRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	auto temp = XMMatrixInverse(nullptr, ViewMatrix);


	auto tempp = XMMatrixTranslationFromVector(temp.r[3]);

	auto newWorld = m_WorldMatrix * tempp;

	//DeferredRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_SkyBoxModel.indexCount, m_SkyBoxModel.StartPoint, 0);

	//def->RenderSkyBox(DeferredRenderer::GetInstance()->GetDeviceContext(), m_SkyBoxModel.indexCount, newWorld, ViewMatrix, projectionMatrix, NULL, m_SkyBoxModel.StartPoint);

	def->RenderSkyBox(DeferredRenderer::GetInstance()->GetDeviceContext(), 36, newWorld, ViewMatrix, projectionMatrix, NULL, m_indiceStartLocation);

}