#pragma once
#include "GameObject.h"




class CRenderImage : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};