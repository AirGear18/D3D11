#include "../pch.h"
#include "ObjectManager.h"
#include "BaseObject.h"

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
	ShutDown();
}

void ObjectManager::ShutDown()
{
	map<BaseObject*, BaseObject*>::iterator it;
	for (it = MoveableObjects.begin(); it != MoveableObjects.end(); it++)
	{
		delete it->first;
	}

	for (it = StaticObjects.begin(); it != StaticObjects.end(); it++)
	{
		delete it->first;
	}
}

void ObjectManager::AddObject(BaseObject*Object)
{
	switch (Object->GetObjectType())
	{
		//Static
	case 0:
	{
		StaticObjects[Object] = Object;
	}
	break;
	//Moveable
	case 1:
	{
		MoveableObjects[Object] = Object;
	}
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
		StaticObjects.erase(Object);
	}
	break;
	//Moveable
	case 1:
	{
		MoveableObjects.erase(Object);
	}
	default:
		break;
	}
}

void ObjectManager::ObjectUpdate(float DeltaTime)
{
	map<BaseObject*, BaseObject*>::iterator it;
	for (it = MoveableObjects.begin(); it != MoveableObjects.end(); it++)
	{
		it->second->Update(DeltaTime);
	}
}
void ObjectManager::ObjectRenderer(XMMATRIX& view, XMMATRIX& projection,DeferredShader* Def)
{
	map<BaseObject*, BaseObject*>::iterator it;
	for (it = MoveableObjects.begin(); it != MoveableObjects.end(); it++)
	{
		it->second->Renderer(view, projection,Def);
	}
	
	for (it = StaticObjects.begin(); it != StaticObjects.end(); it++)
	{
		it->second->Renderer(view, projection,Def);
	}
}
