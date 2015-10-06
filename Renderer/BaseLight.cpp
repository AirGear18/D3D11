#include "../pch.h"
#include "BaseLight.h"


BaseLight::BaseLight()
{
}


BaseLight::~BaseLight()
{
	if (m_matrixBuffer)
		m_matrixBuffer->Release();
}
