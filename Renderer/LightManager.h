#pragma once
#include <list>
class BaseLight;
class DeferredBuffers;
class LightShaderClass;
const int NumContext = 2;

class LightManager
{
	std::list<BaseLight*> SortListContext[NumContext];
	std::list<BaseLight*> m_StaticLights;
	std::list<BaseLight*> m_MoveAbleLights;

	DeferredBuffers * m_DeferredBuffersPtr;
	LightShaderClass * m_LightShaderClass;

	XMFLOAT3 CameraPosition;
public:
	static LightManager* Getinstance();
	LightManager();
	~LightManager();

	void Update(float Delta);
	void Renderer(XMMATRIX&view,XMMATRIX&Projct);
	void AddLightToManager(BaseLight *,int Type);
	void RemoveLight(BaseLight *);

	void AddToContextListSort(int Type, BaseLight*object);

	void SetDeferredBuffers(DeferredBuffers*t) { m_DeferredBuffersPtr = t; }
	void SetLightShaderClass(LightShaderClass*t) { m_LightShaderClass = t; }
	void SetCameraPosition(XMFLOAT3 t){ CameraPosition = t; }
	void ShutDown();
};

