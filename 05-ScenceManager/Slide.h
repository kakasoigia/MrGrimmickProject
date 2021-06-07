#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define SLIDE_WALKING_SPEED 0.01f;

#define SLIDE_BBOX_WIDTH 16
#define SLIDE_BBOX_HEIGHT 15


#define SLIDE_STATE_SLIDING_LEFT 100
#define SLIDE_STATE_SLIDING_RIGHT 200

#define SLIDE_ANI_SLIDING_LEFT 0
#define SLIDE_ANI_SLIDING_RIGHT 1

class Slide :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Slide();
	virtual void SetState(int state);
};

