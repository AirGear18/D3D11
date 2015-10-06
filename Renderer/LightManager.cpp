#include "../pch.h"
#include "LightManager.h"
#include "BaseLight.h"
#include "DeferredBuffers.h"
#include "lightshaderclass.h"
LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

LightManager* LightManager::Getinstance()
{
	static LightManager s_Instance;	// stored in global memory once
	return &s_Instance;
}

void LightManager::Update(float Delta)
{
	list<BaseLight*>::iterator it;
	for (it = m_MoveAbleLights.begin(); it != m_MoveAbleLights.end(); it++)
	{
		(*it)->Update(Delta);
	}
}

void LightManager::Renderer(XMMATRIX&view, XMMATRIX&Projct)
{
	list<BaseLight*>::const_iterator it;
	for (size_t i = 0; i < NumContext; i++)
	{
		switch (i)
		{
		case 0:
		{
			m_LightShaderClass->RenderShader(PosUV, PosUV);
		}
		break;
		case 1:
		{
			m_LightShaderClass->RenderShader(Pos, Pos);
		}
		break;
		default:
			break;
		}

		for (it = SortListContext[i].begin(); it != SortListContext[i].end(); it++)
		{
			(*it)->Renderer(view, Projct, m_DeferredBuffersPtr,CameraPosition);
		}
	}
}

void LightManager::AddLightToManager(BaseLight*light, int Type)
{
	switch (Type)
	{
	case 0:
		m_MoveAbleLights.push_back(light);
		break;
	case 1:
		m_StaticLights.push_back(light);
		break;
	default:
		break;
	}

}

void LightManager::RemoveLight(BaseLight* light)
{
	m_MoveAbleLights.remove(light);
}

void LightManager::AddToContextListSort(int Type, BaseLight*object)
{
	switch (Type)
	{
	case Pos:
	{
		SortListContext[1].push_back(object);
	}
	break;
	case PosUV:
	{
		SortListContext[0].push_back(object);
	}
	break;
	default:
		break;
	}
}

void LightManager::ShutDown()
{
	list<BaseLight*>::iterator it;
	for (it = m_MoveAbleLights.begin(); it != m_MoveAbleLights.end(); it++)
	{
		delete (*it);
	}

	for (it = m_StaticLights.begin(); it != m_StaticLights.end(); it++)
	{
		delete (*it);
	}
}