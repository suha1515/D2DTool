#include "stdafx.h"
#include "Component.h"


CComponent::CComponent()
{
	m_ComponentOn = true;
	m_GameObject = nullptr;
}
CComponent::~CComponent()
{
}
void CComponent::SetOn(bool on)
{
	m_ComponentOn = on;
}

bool CComponent::GetOn()
{
	return m_ComponentOn;
}
