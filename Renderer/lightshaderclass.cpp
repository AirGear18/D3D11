////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "../pch.h"
#include "lightshaderclass.h"
#include "../DirectionalLightVS.csh"
#include "../DirectionalLightPS.csh"
#include "../PointLight_PS.csh"
#include "../PointLight_VS.csh"
#include "DeferredRenderer.h"

LightShaderClass::LightShaderClass()
{
	m_vertexShader[0] = 0;
	m_vertexShader[1] = 0;
	m_pixelShader[0] = 0;
	m_pixelShader[1] = 0;
	m_layout[0] = 0;
	m_layout[1] = 0;
	m_sampleState = 0;
	m_matrixBuffer = 0;
	m_lightBuffer = 0;
}


LightShaderClass::LightShaderClass(const LightShaderClass& other)
{
}


LightShaderClass::~LightShaderClass()
{
}


bool LightShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}


void LightShaderClass::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* colorTexture, ID3D11ShaderResourceView* normalTexture,
	XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor)
{
	bool result;


	// Set the shader parameters that it will use for rendering.
	result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, colorTexture, normalTexture, lightDirection, ambientColor, diffuseColor);
	if (!result)
	{
		return false;
	}

	// Now render the prepared buffers with the shader.
	RenderShader(deviceContext, indexCount);

	return true;
}


bool LightShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd)
{
	HRESULT result;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;



	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(DirectionalLightVS, sizeof(DirectionalLightVS), NULL, &m_vertexShader[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(DirectionalLightPS, sizeof(DirectionalLightPS), NULL, &m_pixelShader[0]);
	if (FAILED(result))
	{
		return false;
	}

	// Create the vertex shader from the buffer.
	result = device->CreateVertexShader(PointLight_VS, sizeof(PointLight_VS), NULL, &m_vertexShader[1]);
	if (FAILED(result))
	{
		return false;
	}

	// Create the pixel shader from the buffer.
	result = device->CreatePixelShader(PointLight_PS, sizeof(PointLight_PS), NULL, &m_pixelShader[1]);
	if (FAILED(result))
	{
		return false;
	}



	// Create the vertex input layout description.
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	// Get a count of the elements in the layout.
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Create the vertex input layout.
	result = device->CreateInputLayout(polygonLayout, numElements, DirectionalLightVS, sizeof(DirectionalLightVS),
		&m_layout[0]);

	result = device->CreateInputLayout(polygonLayout, 1, PointLight_VS, sizeof(PointLight_VS),
		&m_layout[1]);

	if (FAILED(result))
	{
		return false;
	}

	// Release the vertex shader buffer and pixel shader buffer since they are no longer needed.

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the description of the light dynamic constant buffer that is in the pixel shader.
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(DirectionLightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
	result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void LightShaderClass::ShutdownShader()
{
	// Release the light constant buffer.
	if (m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = 0;
	}

	// Release the matrix constant buffer.
	if (m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = 0;
	}

	// Release the sampler state.
	if (m_sampleState)
	{
		m_sampleState->Release();
		m_sampleState = 0;
	}

	// Release the layout.
	for (size_t i = 0; i < 2; i++)
	{
		if (m_layout[i])
		{
			m_layout[i]->Release();
			m_layout[i] = 0;
		}
		// Release the pixel shader.
		if (m_pixelShader[i])
		{
			m_pixelShader[i]->Release();
			m_pixelShader[i] = 0;
		}

		// Release the vertex shader.
		if (m_vertexShader[i])
		{
			m_vertexShader[i]->Release();
			m_vertexShader[i] = 0;
		}
	}




	return;
}

//
// Reminder got remove this after i get lighting Manager up and running
//
bool LightShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix,
	XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* colorTexture,
	ID3D11ShaderResourceView* normalTexture, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	unsigned int bufferNumber;
	MatrixBufferType* dataPtr;
	DirectionLightBufferType* dataPtr2;


	// Transpose the matrices to prepare them for the shader.
	//D3DXMatrixTranspose(&worldMatrix, &worldMatrix);
	//worldMatrix = XMMatrixTranspose(worldMatrix);
	//D3DXMatrixTranspose(&viewMatrix, &viewMatrix);
	//viewMatrix = XMMatrixTranspose(viewMatrix);
	//D3DXMatrixTranspose(&projectionMatrix, &projectionMatrix);
	//projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Now set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	// Set shader texture resources in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &colorTexture);
	deviceContext->PSSetShaderResources(1, 1, &normalTexture);

	// Lock the light constant buffer so it can be written to.
	result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr2 = (DirectionLightBufferType*)mappedResource.pData;

	// Copy the lighting variables into the constant buffer.
	dataPtr2->lightDirection = XMFLOAT4(lightDirection.x, lightDirection.y, lightDirection.z, 0);
	dataPtr2->m_ambientColor = ambientColor;
	dataPtr2->m_diffuseColor = diffuseColor;
	// Unlock the constant buffer.
	deviceContext->Unmap(m_lightBuffer, 0);

	// Set the position of the light constant buffer in the pixel shader.
	bufferNumber = 0;

	// Finally set the light constant buffer in the pixel shader with the updated values.
	deviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);

	return true;
}


void LightShaderClass::RenderShader(int InputLayOut, int ShaderType)
{
	switch (InputLayOut)
	{
	case PosUV:
	{
		// Set the vertex input layout.
		DeferredRenderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_layout[0]);
	}
	break;
	case Pos:
	{
		// Set the vertex input layout.
		DeferredRenderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_layout[1]);
		//DeferredRenderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	}
	break;
	default:
		break;
	}

	switch (ShaderType)
	{
	case Pos:
	{
		DeferredRenderer::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader[1], NULL, 0);
		DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader[1], NULL, 0);
	}
	break;
	case PosUV:
	{
		DeferredRenderer::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader[0], NULL, 0);
		DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader[0], NULL, 0);
	}
	break;
	default:
		break;
	}

	// Set the sampler state in the pixel shader.
	DeferredRenderer::GetInstance()->GetDeviceContext()->PSSetSamplers(0, 1, &m_sampleState);



}

void LightShaderClass::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{

	// Set the vertex input layout.
	deviceContext->IASetInputLayout(m_layout[0]);

	// Set the vertex and pixel shaders that will be used to render.
	deviceContext->VSSetShader(m_vertexShader[0], NULL, 0);
	deviceContext->PSSetShader(m_pixelShader[0], NULL, 0);

	// Set the sampler state in the pixel shader.
	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	// Render the geometry.
	deviceContext->DrawIndexed(indexCount, 0, 0);
}