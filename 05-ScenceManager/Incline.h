#pragma once

#include "GameObject.h"

#define INCLINE_SPEED_X
#define INCLINE_GRAVITY

#define INCLINE_TYPE_UP	1
#define INCLINE_TYPE_DOWN	2

#define INCLINE_SIZE_QUAD		1
#define INCLINE_SIZE_REC		2

class Incline : public CGameObject
{
public:

	float x1, x2;

	int size;

	int state;
	int direct;

	float slidePos;

	Incline(float x, float y, int direct, int size);
	~Incline();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

};

