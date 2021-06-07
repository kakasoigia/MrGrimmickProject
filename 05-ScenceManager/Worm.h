#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define WORM_WALKING_SPEED 0.01f;
#define WORM_GRAVITY 0.0008f
#define WORM_BBOX_WIDTH 16
#define WORM_BBOX_HEIGHT 15


#define WORM_STATE_WALKING 100

#define WORM_STATE_FLYING 200

#define WORM_STATE_DIE 500

#define WORM_ANI_WALK_RIGHT 1
#define WORM_ANI_WALK_LEFT 0


class Worm :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Worm();
	virtual void SetState(int state);
};

