#include "../pch.h"
#include "BaseRenderer.h"
#include "ObjectManager.h"
#include "../ModelObject.h"
#include "../Renderer/DeferredRenderer.h"
#include "../Renderer/DeferredShader.h"

// Zero : Type object ( static moveable)
// First : File path so model can be loaded in
// Second : Is this object is attached to anything
// Third : If attached Give object to attach to
// Fourth : Offset it location from the attached location
BaseRenderer::BaseRenderer(int ObjectType,const char *FilePath, const char * DDSLocation, bool AttachTo, BaseObject *ObjectToAttackTo, DirectX::XMFLOAT3 OffsetAttachLocation, const char * NameRendererObject)
{
	m_Model.LoadObject(FilePath, NameRendererObject, DDSLocation);

	//Model.LoadObject();
	m_ObjectID = ObjectType;
	
	ObjectManager::GetInstance()->AddObject(this);
	
	m_WorldMatrix = XMMatrixIdentity();
	m_Position = XMFLOAT3(0, 0, 0);
}


BaseRenderer::~BaseRenderer()
{
	m_Model.Shutdown();
}

void BaseRenderer::Update(float DeltaTime)
{
	m_WorldMatrix = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}
void BaseRenderer::Renderer(XMMATRIX& ViewMatrix, XMMATRIX&projectionMatrix, DeferredShader * def)
{
	m_Model.RenderBuffers(DeferredRenderer::GetInstance()->GetDeviceContext());
	def->Render(DeferredRenderer::GetInstance()->GetDeviceContext(),m_Model.indexCount,m_WorldMatrix,ViewMatrix,projectionMatrix,m_Model.ShadeView,m_Model.StartPoint );
}