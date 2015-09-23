#pragma once
#include <d3d11.h>
#include <map>


class VertexBufferManager
{
	std::map<std::string, ID3D11Buffer *> m_VertBuffers;
	std::map<std::string, UINT> m_IndexCount;
	std::map<std::string, UINT> m_IndexStartLocation;
public:

	static VertexBufferManager * GetInstance();
	VertexBufferManager() = default;
	~VertexBufferManager();

	void ShutDown();

	ID3D11Buffer* CheckVertexBuffers(const char *);
	void AddVertexBuffers(const char *, ID3D11Buffer*,UINT,UINT);
	UINT GetIndexCount(const char *);
	UINT GetIndexStartPos(const char *);
};

