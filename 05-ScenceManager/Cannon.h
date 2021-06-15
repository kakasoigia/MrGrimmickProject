#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define CANNON_GRAVITY 0.0008f
#define CANNON_BBOX_WIDTH 16
#define CANNON_BBOX_HEIGHT 15


#define CANNON_STATE_GREEN 100

#define CANNON_STATE_RED 200



#define CANNON_ANI_GREEN_RIGHT 0

#define CANNON_ANI_RED_RIGHT 1


class Cannon :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Cannon();
	virtual void SetState(int state);
};

