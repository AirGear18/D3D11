#pragma once

#include <vector>

#include <d3d11.h>

#include <DirectXMath.h>
#include "Vector4.h"
#include<map>

using namespace DirectX;

using namespace std;

struct SIMPLE_VERTEX
{
	Vector4  position;
	//Vector4  color;
	Vector4  uv;
	Vector4  normal;
	//Vector4 tangent;
};
struct LOADING_VERTEX
{
	float x, y, z, w;
};
struct LOADING_UV
{
	float u, v;
};
struct LOADING_NORMAL
{
	float nX, nY, nZ;
};

class ModelObject
{
	bool SaveObjectBinary(const char* fileName);
	void LoadobjectBinary(const char* filePath);
	bool InitializeBuffers(ID3D11Device* device);
	void LoadTexture(string DDSLocation);

public:
	//CTOR & DTOR
	ModelObject();
	~ModelObject();
	bool LoadObject(const char* filePath, const char *FileName, const char *DDSLocation); //load the model information from obj file
	//vertex information
	SIMPLE_VERTEX* vertexList;
	unsigned int vertexCount;
	
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	
	//index information
	UINT* indexList;
	UINT indexCount;
	UINT StartPoint;

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	ID3D11ShaderResourceView * ShadeView;
	ID3D11Resource * pBB;
	void Shutdown();

	map<string, pair<unsigned int, SIMPLE_VERTEX*>> indexBuffer;
};

