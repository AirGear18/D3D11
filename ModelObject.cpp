#include "pch.h"
#include "ModelObject.h"

#include <fstream>
#include "Renderer\DeferredRenderer.h"
#include "Renderer\VertexBufferManager.h"
#include "Definds.h"
#include "DDSTextureLoader.h"
//#include <string.h>
#include <sstream>
using namespace std;

ModelObject::ModelObject()
{
	vertexList = 0;
	indexList = 0;
}

ModelObject::~ModelObject()
{
	Shutdown();
}
void ModelObject::Shutdown()
{
	if (vertexList)
		delete[] vertexList;
	if (indexList)
		delete[] indexList;
	if (ShadeView)
		ShadeView->Release();
	if (pBB)
		pBB->Release();
}


bool ModelObject::LoadObject(const char* filePath, const char *FileName, const char *DDSLocation)
{
	bool BinaryLoaded = false;
	string BinaryFilePath("Assets/Binary");
	ofstream myfile;

	int indextemp = 0;
	while (FileName[indextemp] != NULL)
		indextemp++;
	char * FileNameA = new char[indextemp + 4];
	memcpy(FileNameA, &FileName[0], sizeof(const char)*indextemp);

	FileNameA[indextemp] = '.';
	FileNameA[indextemp + 1] = 'P';
	FileNameA[indextemp + 2] = 'I';
	FileNameA[indextemp + 3] = 'E';
	FileNameA[indextemp + 4] = NULL;

	BinaryFilePath += "/";
	BinaryFilePath += FileNameA;

	ifstream infile(BinaryFilePath.c_str());

	if (VertexBufferManager::GetInstance()->CheckVertexBuffers(filePath) != NULL)
	{
		m_vertexBuffer = VertexBufferManager::GetInstance()->CheckVertexBuffers(filePath);
		indexCount = VertexBufferManager::GetInstance()->GetIndexCount(filePath);
		StartPoint = VertexBufferManager::GetInstance()->GetIndexStartPos(filePath);

		LoadTexture(string(DDSLocation));
	}
	else if (!infile.good())
	{
		if (!filePath)
			return false;

		//code source :
		//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

		//create a read-only file using the filepath name
		FILE* inputFile = nullptr;
		//secure version
		errno_t err = fopen_s(&inputFile, filePath, "r");

		if (!inputFile || err)
			return false;

		//set to the beginning
		fseek(inputFile, 0L, SEEK_SET);

		//vectors for all data
		std::vector<unsigned int> vertexIndex, uvIndex, normalIndex, indexArr;
		std::vector<Vector4> vertexVec, uvVec, normalVec;

		std::vector<SIMPLE_VERTEX> arr;

		//read until the end (DUH!)
		while (true)
		{
			//info
			std::string header;
			char buffer[128];
			//read beginning
			int result = fscanf_s(inputFile, "%s", buffer, 128);
			//done?
			if (result == EOF)
				break;

			//capture for simple checking
			header = buffer;

			//parse for data type
			if (header == "v")
			{
				//local vertex info
				Vector4 vertex;
				//zero out valus
				ZeroMemory(&vertex, sizeof(Vector4));
				//scan values
				fscanf_s(inputFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				//push into vector
				vertexVec.push_back(vertex);
			}
			else if (header == "vt")
			{
				//local texture info
				Vector4 uv;
				//zero out all values
				ZeroMemory(&uv, sizeof(Vector4));
				//scan values into variable
				fscanf_s(inputFile, "%f %f\n", &uv.u, &uv.v);
				//push variable into vector
				uvVec.push_back(uv);
			}
			else if (header == "vn")
			{
				//local normal struct
				Vector4 norm;
				//zero out all values
				ZeroMemory(&norm, sizeof(Vector4));
				//scan values
				fscanf_s(inputFile, "%f %f %f\n", &norm.x, &norm.y, &norm.z);
				//push values
				normalVec.push_back(norm);
			}
			else if (header == "f")
			{
				//arrays for values
				unsigned int vInfo[3], uvInfo[3], nInfo[3];
				//zero out all values
				ZeroMemory(vInfo, sizeof(vInfo));
				ZeroMemory(uvInfo, sizeof(uvInfo));
				ZeroMemory(nInfo, sizeof(nInfo));
				//scan values
				int val = fscanf_s(inputFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vInfo[0], &uvInfo[0], &nInfo[0], &vInfo[1], &uvInfo[1], &nInfo[1], &vInfo[2], &uvInfo[2], &nInfo[2]);

				//store values
				vertexIndex.push_back(vInfo[0]);
				vertexIndex.push_back(vInfo[1]);
				vertexIndex.push_back(vInfo[2]);

				uvIndex.push_back(uvInfo[0]);
				uvIndex.push_back(uvInfo[1]);
				uvIndex.push_back(uvInfo[2]);

				normalIndex.push_back(nInfo[0]);
				normalIndex.push_back(nInfo[1]);
				normalIndex.push_back(nInfo[2]);
			}
		}




		//resize
		arr.clear();

		//create the vertices
		for (unsigned int i = 0; i < vertexIndex.size(); ++i)
		{
			SIMPLE_VERTEX simpleVertex;
			ZeroMemory(&simpleVertex, sizeof(SIMPLE_VERTEX));

			//identifier
			ostringstream oss;

			int Index = vertexIndex[i];
			simpleVertex.position.x = (Index != 0) ? vertexVec[Index - 1].x : 0.f;
			simpleVertex.position.y = (Index != 0) ? vertexVec[Index - 1].y : 0.f;
			simpleVertex.position.z = (Index != 0) ? vertexVec[Index - 1].z : 0.f;
			simpleVertex.position.w = 1;
			oss << (Index - 1);

			//Texture Coords
			Index = uvIndex[i];
			simpleVertex.uv.u = (Index != 0) ? uvVec[Index - 1].u : 0.f;
			simpleVertex.uv.v = (Index != 0) ? 1 - uvVec[Index - 1].v : 0.f;
			oss << (Index - 1);

			//normals
			Index = normalIndex[i];
			simpleVertex.normal.x = (Index != 0) ? normalVec[Index - 1].x : 0;
			simpleVertex.normal.y = (Index != 0) ? normalVec[Index - 1].y : 0;
			simpleVertex.normal.z = (Index != 0) ? normalVec[Index - 1].z : 0;
			simpleVertex.normal.w = 0;
			oss << (Index - 1);

			//Is this vertex unique?
			if (indexBuffer[oss.str()].second == 0)
			{
				//create a unique vertex instance
				pair<unsigned int, SIMPLE_VERTEX*> uniqueVert;
				//index of the vert
				uniqueVert.first = arr.size();
				//initialize the pointer
				uniqueVert.second = &simpleVertex;
				//add to the buffer
				indexBuffer[oss.str()] = uniqueVert;
				//add to the index
				indexArr.push_back(arr.size());
				//add to the vector
				arr.push_back(simpleVertex);
			}
			else
				indexArr.push_back(indexBuffer[oss.str()].first);


			//arr.push_back(simpleVertex);

		}


		//clear memory
		vertexIndex.clear();
		uvIndex.clear();
		normalIndex.clear();
		//indexArr.clear();
		vertexVec.clear();
		uvVec.clear();
		normalVec.clear();

		//release?
		//SAFE_ARRAY_DELETE(vertexList);
		if (vertexList)
		{
			delete[] vertexList;
			vertexList = 0;
		}
		//create the vertex list
		vertexCount = (unsigned int)arr.size();
		vertexList = new SIMPLE_VERTEX[vertexCount];
		//for (unsigned int i = 0; i < vertexCount; i++)
		//vertexList[i] = arr[i];
		memcpy(vertexList, &arr[0], sizeof(SIMPLE_VERTEX)*vertexCount);

		//create the index buffer
		indexCount = indexArr.size();
		indexList = new unsigned int[indexCount];
		memcpy(indexList, &indexArr[0], sizeof(UINT)* indexArr.size());

		/*for (UINT i = 0; i < indexCount; i++)
			indexList[i] = i;*/
		//clear mem
		arr.clear();


		StartPoint = DeferredRenderer::GetInstance()->AddIndices(indexList, indexCount);

		SaveObjectBinary(BinaryFilePath.c_str());
		InitializeBuffers(DeferredRenderer::GetInstance()->GetDevice());
		LoadTexture(string(DDSLocation));

		VertexBufferManager::GetInstance()->AddVertexBuffers(filePath, m_vertexBuffer, indexCount, StartPoint);
	}
	else
	{
		LoadobjectBinary(BinaryFilePath.c_str());

		StartPoint = DeferredRenderer::GetInstance()->AddIndices(indexList, indexCount);
		InitializeBuffers(DeferredRenderer::GetInstance()->GetDevice());
		LoadTexture(string(DDSLocation));
		VertexBufferManager::GetInstance()->AddVertexBuffers(filePath, m_vertexBuffer, indexCount, StartPoint);
	}

	// Clean up memory leak
	//delete [] FileNameA;

	return true;
}

void ModelObject::LoadobjectBinary(const char* filePath)
{
	ifstream infile(filePath, ios_base::in | ios_base::binary);
	if (infile.is_open())
	{

		infile.read((char *)&vertexCount, sizeof(unsigned int));
		vertexList = new SIMPLE_VERTEX[vertexCount];
		infile.read((char *)vertexList, sizeof(SIMPLE_VERTEX)*vertexCount);


		infile.read((char *)&indexCount, sizeof(indexCount));
		indexList = new UINT[indexCount];
		infile.read((char *)indexList, sizeof(UINT)*indexCount);

		infile.close();
	}

}

bool ModelObject::SaveObjectBinary(const char* fileName)
{
	ofstream outfile(fileName, ios::binary);
	//how many SIMPLE_VERTEX to read in
	outfile.write((char *)&vertexCount, sizeof(vertexCount));
	outfile.write((char *)vertexList, sizeof(SIMPLE_VERTEX)*vertexCount);
	//how many indexs to read in
	outfile.write((char *)&indexCount, sizeof(indexCount));
	outfile.write((char *)indexList, sizeof(UINT)*indexCount);
	outfile.close();

	return true;
}

bool ModelObject::InitializeBuffers(ID3D11Device* device)
{
	VertexInputType* vertices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	unsigned int i;


	// Create the vertex array.
	vertices = new VertexInputType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < vertexCount; i++)
	{
		vertices[i].position = XMFLOAT4(vertexList[i].position.x, vertexList[i].position.y, vertexList[i].position.z, vertexList[i].position.w);
		vertices[i].tex = XMFLOAT2(vertexList[i].uv.u, vertexList[i].uv.v);
		vertices[i].normal = XMFLOAT3(vertexList[i].normal.x, vertexList[i].normal.y, vertexList[i].normal.z);
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexInputType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;


	return true;
}

void ModelObject::RenderBuffers(ID3D11DeviceContext* deviceContext)
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
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}
void ModelObject::LoadTexture(string DDSLocation)
{
	wstring widstr = wstring(DDSLocation.begin(), DDSLocation.end());
	//const wchar_t* szName = DDSLocation.c_str();
	CreateDDSTextureFromFile(DeferredRenderer::GetInstance()->GetDevice(), widstr.c_str(), &pBB, &ShadeView);
}