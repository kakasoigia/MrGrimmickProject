
#pragma once
#include "GameObject.h"


class CAniBrick : public CGameObject
{
public:
	CAniBrick(int type);

	int type;

	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};


