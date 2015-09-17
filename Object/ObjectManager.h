#pragma once
#include <map>
#include <vector>
#include <list>
class DeferredShader;
class BaseObject;

const int SORTNUMLIST = 3;

class ObjectManager
{
	std::list<BaseObject*>  StaticObjects;
	std::list<BaseObject*>  MoveableObjects;

	list<BaseObject*> SortListContext[SORTNUMLIST];

public:
	ObjectManager();
	~ObjectManager();
	static ObjectManager *GetInstance();

	//functions
	void AddObject(BaseObject*);
	void AddToContextListSort(int Type, BaseObject*object);
	void RemoveObject(BaseObject*);
	void ObjectUpdate(float DeltaTime);
	void ObjectRenderer(DirectX::XMMATRIX& view, DirectX::XMMATRIX& projection,DeferredShader * Def);

	void ShutDown();

	//gets
	std::list<BaseObject*>  GetStaticObject(){ return StaticObjects; }
	std::list<BaseObject*>  GetMoveableObjects(){ return MoveableObjects; }

};

