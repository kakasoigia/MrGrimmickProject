#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define STAR_GRAVITY 0.001f
#define	STAR_FLYING_SPEED_Y 0.2f
#define	STAR_FLYING_SPEED_X 0.15f
#define STAR_BBOX_WIDTH 24
#define STAR_BBOX_HEIGHT 24
#define STAR_DECREASE_SPEED_X 0.01f
#define STAR_DECREASE_SPEED_Y 0.01f

#define STAR_STATE_IDLING 100

#define STAR_STATE_FLYING 200
#define STAR_STATE_SMOKE	300

#define	STAR_SMOKE_TIME 550
#define STAR_ANI_SMALL_STAR	0
#define STAR_ANI_BIG_STAR 1
#define STAR_ANI_BOOM 2



class Star:
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	bool isUsed = false;
	float LimitY ;
	float LimitX ;
	float v_start = 0.1f;
	DWORD smoke_start = 0;
	
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Star();
	virtual void SetState(int state);
	void SetIsUsed(bool isused) { isUsed = isused; };
	bool GetIsUsed() { return isUsed; };
	void StartSmoke() { smoke_start = GetTickCount(); }

};

