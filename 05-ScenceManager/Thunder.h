#pragma once
#include "GameObject.h"

#define THUNDER_BBOX_WIDTH  2
#define THUNDER_BBOX_HEIGHT 2

class CThunder : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};