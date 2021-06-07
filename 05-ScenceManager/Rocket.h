#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define ROCKET_GRAVITY 0.0008f
#define ROCKET_BBOX_WIDTH 16
#define ROCKET_BBOX_HEIGHT 15


#define ROCKET_STATE_IDLING 100

#define ROCKET_STATE_FALLING 200

#define ROCKET_ANI_GREEN 0

#define ROCKET_ANI_RED 1


class Rocket :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Rocket();
	virtual void SetState(int state);
};

