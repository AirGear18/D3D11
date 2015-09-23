#include "../pch.h"
#include "VertexBufferManager.h"


VertexBufferManager* VertexBufferManager::GetInstance()
{
	static VertexBufferManager s_Instance;	// stored in global memory once
	return &s_Instance;
}


VertexBufferManager::~VertexBufferManager()
{
	ShutDown();
}

ID3D11Buffer* VertexBufferManager::CheckVertexBuffers(const char * ObjectName)
{
	string temp = ObjectName;
	map<std::string, ID3D11Buffer*>::iterator it;
	for (it = m_VertBuffers.begin(); it != m_VertBuffers.end(); it++)
	{
		if (temp == it->first)
			return it->second;
	}
	return NULL;
}

void VertexBufferManager::AddVertexBuffers(const char *ObjectName, ID3D11Buffer* Buffer, UINT indexCount, UINT IndexStartLoc)
{
	m_VertBuffers[std::string(ObjectName)] = Buffer;
	m_IndexCount[std::string(ObjectName)] = indexCount;
	m_IndexStartLocation[std::string(ObjectName)] = IndexStartLoc;
}

void VertexBufferManager::ShutDown()
{
	map<std::string, ID3D11Buffer*>::iterator it;
	for (it = m_VertBuffers.begin(); it != m_VertBuffers.end();)
	{
		//clean up memeory
		if (it->second)
			it->second->Release();
		//clean up
		m_VertBuffers.erase(it->first);
		it = m_VertBuffers.begin();
	}

	//map<const char *, UINT>::iterator Index;
	//for (Index = m_IndexCount.begin(); Index != m_IndexCount.end(); Index++)
	//{
	//	//clean up memeory
	//	Index->second->Release();
	//	//clean up
	//	m_VertBuffers.erase(Index->first);
	//}
	//
	//for (Index = m_IndexStartLocation.begin(); Index != m_IndexStartLocation.end(); Index++)
	//{
	//	//clean up memeory
	//	Index->second->Release();
	//	//clean up
	//	m_VertBuffers.erase(Index->first);
	//}

}
UINT VertexBufferManager::GetIndexCount(const char *ObjectName)
{
	return  m_IndexCount.find(std::string(ObjectName))->second;

}

UINT VertexBufferManager::GetIndexStartPos(const char *ObjectName)
{
	return  m_IndexStartLocation.find(std::string(ObjectName))->second;
}