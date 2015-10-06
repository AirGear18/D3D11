#include "../pch.h"
#include "PointLight.h"
#include "LightManager.h"
#include "DeferredRenderer.h"
#include "DeferredBuffers.h"
#include "VertexBufferManager.h"

PointLight::PointLight(XMFLOAT4 Color, XMFLOAT3 postion, XMFLOAT3 Rot, int Type, int Context)
{
	m_Color = Color;
	m_Position = postion;
	m_Rotation = Rot;
	m_ObjectID = Type;
	m_ContextID = Context;

	LightManager::Getinstance()->AddLightToManager(this, Type);
	LightManager::Getinstance()->AddToContextListSort(Context, this);

	m_PointLightModel.LoadObject("Assets/Models/Sphere.obj", "Sphere", NULL, Pos);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	DeferredRenderer::GetInstance()->GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBuffer);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	DeferredRenderer::GetInstance()->GetDevice()->CreateBuffer(&lightBufferDesc, NULL, &m_PointLightBuffer);

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	D3D11_BUFFER_DESC InvViewProjBufferBufferDesc;
	InvViewProjBufferBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	InvViewProjBufferBufferDesc.ByteWidth = sizeof(InvViewProjBufferType);
	InvViewProjBufferBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	InvViewProjBufferBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	InvViewProjBufferBufferDesc.MiscFlags = 0;
	InvViewProjBufferBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	DeferredRenderer::GetInstance()->GetDevice()->CreateBuffer(&InvViewProjBufferBufferDesc, NULL, &InvViewProjBuffer);
	Range = 15;
	//m_WorldMatric = XMMatrixIdentity();
	m_WorldMatric *= XMMatrixScaling(Range, Range, Range);
	//m_WorldMatric *= XMMatrixRotationY(90);
	m_WorldMatric *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

}


PointLight::~PointLight()
{
	if (m_PointLightBuffer)
		m_PointLightBuffer->Release();
	if (InvViewProjBuffer)
		InvViewProjBuffer->Release();
	//if (m_vertexBuffer)
	//	m_vertexBuffer->Release();
}

void PointLight::Update(float DeltaTime)
{
	//static float test = 0;
	//test += DeltaTime;
	//m_WorldMatric = XMMatrixIdentity();
	//m_WorldMatric *= XMMatrixScaling(5, 5, 5);
	//m_WorldMatric *= XMMatrixRotationY(30);
	//m_WorldMatric *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}

void PointLight::Renderer(XMMATRIX&view, XMMATRIX& projection, DeferredBuffers*def, XMFLOAT3 Cam)
{
	UpdateConstBuffer(view, projection, def, Cam);
	m_PointLightModel.RenderBuffers(DeferredRenderer::GetInstance()->GetDeviceContext(), Pos);

	XMVECTOR vector1 = XMLoadFloat3(&Cam);
	XMVECTOR vector2 = XMLoadFloat3(&m_Position);
	XMVECTOR vectorSub = XMVectorSubtract(vector1, vector2);
	XMVECTOR Thelength = XMVector3Length(vectorSub);

	float Thedistance = 0.0f;
	XMStoreFloat(&Thedistance, Thelength);
	if (Thedistance < Range)
	{
		//DeferredRenderer::GetInstance()->TurnOffCulling();
		//
		DeferredRenderer::GetInstance()->TurnZBufferOff();
		DeferredRenderer::GetInstance()->TurnOnCCWcullMode();

		DeferredRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_PointLightModel.indexCount, m_PointLightModel.StartPoint, 0);

		DeferredRenderer::GetInstance()->TurnOnCWcullMode();
	}
	else
		DeferredRenderer::GetInstance()->GetDeviceContext()->DrawIndexed(m_PointLightModel.indexCount, m_PointLightModel.StartPoint, 0);

}

void PointLight::UpdateConstBuffer(XMMATRIX&view, XMMATRIX& projection, DeferredBuffers*def, XMFLOAT3 Cam)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	LightBuffer* dataPtr2;
	InvViewProjBufferType * dataPtr3;

	// Lock the constant buffer so it can be written to.
	result = DeferredRenderer::GetInstance()->GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = m_WorldMatric;
	dataPtr->view = view;
	dataPtr->projection = projection;

	// Unlock the constant buffer.
	DeferredRenderer::GetInstance()->GetDeviceContext()->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	DeferredRenderer::GetInstance()->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	ID3D11ShaderResourceView * color = def->GetShaderResourceView(0);
	ID3D11ShaderResourceView * normal = def->GetShaderResourceView(1);
	ID3D11ShaderResourceView * depth = def->GetShaderResourceView(2);
	//Set shader texture resources in the pixel shader.
	DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(0, 1, &color);
	DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(1, 1, &normal);
	DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetShaderResources(2, 1, &depth);

	// Lock the light constant buffer so it can be written to.
	result = DeferredRenderer::GetInstance()->GetDeviceContext()->Map(m_PointLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (LightBuffer*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	//dataPtr2->ambientColor = XMFLOAT3(m_Color.x, m_Color.y, m_Color.z);
	dataPtr2->diffuseColor = XMFLOAT3(m_Color.x, m_Color.y, m_Color.z);
	dataPtr2->CameraPosition = Cam;
	dataPtr2->position = XMFLOAT4(m_Position.x, m_Position.y, m_Position.z, 1.0f);
	dataPtr2->range = Range;
	dataPtr2->Attenuation = XMFLOAT4(1, .1f, .1f, 1);

	// Unlock the constant buffer.
	DeferredRenderer::GetInstance()->GetDeviceContext()->Unmap(m_PointLightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_PointLightBuffer);



	result = DeferredRenderer::GetInstance()->GetDeviceContext()->Map(InvViewProjBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

	// Get a pointer to the data in the constant buffer.
	dataPtr3 = (InvViewProjBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr3->gInvViewProj = XMMatrixInverse(NULL, XMMatrixMultiply(view, projection));

	int test = sizeof(XMMatrixInverse(NULL, view));
	// Unlock the constant buffer.
	DeferredRenderer::GetInstance()->GetDeviceContext()->Unmap(InvViewProjBuffer, 0);

	DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetConstantBuffers(1, 1, &InvViewProjBuffer);
}

