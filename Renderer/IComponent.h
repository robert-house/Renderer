//--------------------------------------------------------------------------------------
// File: IComponent.h
//
// This class is an interface for the Component game mods
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------

#ifndef ICOMPONENT_H
#define ICOMPONENT_H

#include <string>

class IComponent
{
public:
	IComponent() {}
	virtual ~IComponent() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void ShutDown() = 0;

	//bool Equals(IComponent* component) { if (TYPE == IComponent::TYPE) return true; else return false; }

protected:
	//StringID m_Name;

};

#endif ICOMPONENT_H