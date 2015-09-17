#include "../pch.h"
#include "ObjectManager.h"
#include "BaseObject.h"
#include "../Renderer/DeferredShader.h"
#include "../Renderer/DeferredRenderer.h"

//SignleTon
ObjectManager* ObjectManager::GetInstance()
{
	static ObjectManager s_Instance;	// stored in global memory once
	return &s_Instance;
}

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
	//ShutDown();
}

void ObjectManager::ShutDown()
{
	list<BaseObject*>::iterator it;
	for (it = MoveableObjects.begin(); it != MoveableObjects.end(); it++)
	{
		delete (*it);
	}

	for (it = StaticObjects.begin(); it != StaticObjects.end(); it++)
	{
		delete (*it);
	}
}

void ObjectManager::AddObject(BaseObject*Object)
{
	switch (Object->GetObjectType())
	{
		//Static
	case 0:
		StaticObjects.push_back(Object);
		break;
		//Moveable
	case 1:
		MoveableObjects.push_back(Object);
		break;

	default:
		break;
	}
}

void ObjectManager::RemoveObject(BaseObject*Object)
{
	switch (Object->GetObjectType())
	{
		//Static
	case 0:
	{
		StaticObjects.remove(Object);
		delete Object;
	}
	break;
	//Moveable
	case 1:
	{
		MoveableObjects.remove(Object);
		delete Object;
	}
	default:
		break;
	}
}

void ObjectManager::ObjectUpdate(float DeltaTime)
{
	list<BaseObject*>::iterator it;
	for (it = MoveableObjects.begin(); it != MoveableObjects.end(); it++)
	{
		(*it)->Update(DeltaTime);
	}
}
void ObjectManager::ObjectRenderer(XMMATRIX& view, XMMATRIX& projection, DeferredShader* Def)
{
	list<BaseObject*>::const_iterator it;
	for (size_t i = 0; i < SORTNUMLIST; i++)
	{
		Def->SetInputLayout(DeferredRenderer::GetInstance()->GetDeviceContext(), i);
		for (it = SortListContext[i].begin(); it != SortListContext[i].end(); it++)
		{
			(*it)->Renderer(view, projection, Def);
		}
	}
}
void ObjectManager::AddToContextListSort(int Type, BaseObject*object)
{
	switch (Type)
	{
	case Posnormuv:
		SortListContext[Type].push_back(object);
		break;
	case poscolor:
		SortListContext[Type].push_back(object);
		break;
	default:
		break;
	}
}