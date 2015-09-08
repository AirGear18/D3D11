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


	//bool operator==(const SIMPLE_VERTEX& other)
	//{
	//	for (unsigned int i = 0; i < 4; ++i)
	//	{
	//		if (position[i] != other.position[i])
	//		{
	//			return false;
	//		}
	//		//if (color[i] != other.color[i])
	//		//	{return false;}			
	//		if (uv[i] != other.uv[i])
	//		{
	//			return false;
	//		}
	//		//if (normal[i] != other.normal[i])
	//		//	{return false;}			
	//		//if (tangent[i] != other.tangent[i])
	//		//	{return false; }
	//	}
	//	return true;
	//}
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
public:
	//CTOR & DTOR
	ModelObject();
	~ModelObject();
	bool LoadObject(const char* filePath, const char *FileName); //load the model information from obj file
	//Initialization
	//bool Initialize(ID3D11Device* theDevice);  //create all d3d11 objects
	bool SaveObjectBinary(const char* fileName);
	void LoadobjectBinary(const char* filePath);

	//vertex information
	SIMPLE_VERTEX* vertexList;
	unsigned int vertexCount;
	bool InitializeBuffers(ID3D11Device* device);
	void RenderBuffers(ID3D11DeviceContext* deviceContext);
	void LoadTexture();	

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

