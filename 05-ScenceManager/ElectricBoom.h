#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define ELECTRICBOOM_WALKING_SPEED 0.01f;
#define ELECTRICBOOM_GRAVITY 0.0008f
#define ELECTRICBOOM_BBOX_WIDTH 16
#define ELECTRICBOOM_BBOX_HEIGHT 15


#define ELECTRICBOOM_STATE_WALKING 100

#define ELECTRICBOOM_STATE_FLYING 200

#define ELECTRICBOOM_STATE_DIE 500

#define ELECTRICBOOM_ANI_WALK_RIGHT 0
#define ELECTRICBOOM_ANI_WALK_LEFT 1
#define ELECTRICBOOM_ANI_FLY_RIGHT 2
#define ELECTRICBOOM_ANI_FLY_LEFT 3

class ElectricBoom :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	ElectricBoom();
	virtual void SetState(int state);
};

