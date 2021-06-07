#pragma once
#include "GameObject.h"
#include "Gimmick.h"


#define FLOATBOX_BBOX_WIDTH 16
#define FLOATBOX_BBOX_HEIGHT 15


#define FLOATBOX_STATE_FLYING_UP 100

#define FLOATBOX_STATE_FLYING_DOWN 200

#define FLOATBOX_ANI_FLYING_UP 0

#define FLOATBOX_ANI_FLYING_DOWN 1


class FloatBox :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	FloatBox();
	virtual void SetState(int state);
};

