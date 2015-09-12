#pragma once
#include <map>

class DeferredShader;
class BaseObject;
class ObjectManager
{
	std::map<BaseObject*, BaseObject*> StaticObjects;
	std::map<BaseObject*, BaseObject*> MoveableObjects;

public:
	ObjectManager();
	~ObjectManager();
	static ObjectManager *GetInstance();

	//functions
	void AddObject(BaseObject*);
	void RemoveObject(BaseObject*);
	void ObjectUpdate(float DeltaTime);
	void ObjectRenderer(DirectX::XMMATRIX& view, DirectX::XMMATRIX& projection,DeferredShader * Def);

	void ShutDown();

	//gets
	std::map<BaseObject*, BaseObject*> GetStaticObject(){ return StaticObjects; }
	std::map<BaseObject*, BaseObject*> GetMoveableObjects(){ return MoveableObjects; }

};

