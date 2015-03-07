#ifndef ICOMDRAWABLE_H
#define ICOMDRAWABLE_H

#include "IComponent.h"

//--------------------------------------------------------------------------------------
// File: IRenderableComponent.h
//
// This extends the functionallity of the IComponent. These components know how to
// draw themselves
//
// Author: Robert House 2014
//--------------------------------------------------------------------------------------

class ICOMDrawable : public IComponent
{
public:
	ICOMDrawable() : IComponent() {}
	virtual ~ICOMDrawable() {}

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void ShutDown() = 0;
	virtual void Draw() = 0;
};

#endif ICOMDRAWABLE_H