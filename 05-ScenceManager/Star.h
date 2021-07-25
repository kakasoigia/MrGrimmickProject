#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define STAR_GRAVITY 0.0005f
#define	STAR_FLYING_SPEED_Y 0.125f
#define	STAR_FLYING_SPEED_X 0.25f
#define STAR_BBOX_WIDTH 24
#define STAR_BBOX_HEIGHT 24
#define STAR_DECREASE_SPEED_X 0.005f
#define STAR_DECREASE_SPEED_Y 0.02f

#define STAR_STATE_DISAPPEAR 100
#define STAR_STATE_LOADING 400
#define STAR_STATE_READY_TO_SHOT 600
#define STAR_STATE_FLYING 200
#define STAR_STATE_SMOKE	300

#define	STAR_SMOKE_TIME 550
#define STAR_INC_CONVERG_TIME 4

#define STAR_ANI_SMALL_STAR	0
#define STAR_ANI_BIG_STAR 1
#define STAR_ANI_BOOM 2

#define STAR_SMALL_MID_X0			0
#define STAR_SMALL_MID_Y0			35

#define STAR_SMALL_LEFT1_X0			-30
#define STAR_SMALL_LEFT1_Y0			10

#define STAR_SMALL_LEFT2_X0			-20
#define STAR_SMALL_LEFT2_Y0			-25

#define STAR_SMALL_RIGHT1_X0			30
#define STAR_SMALL_RIGHT1_Y0			10

#define STAR_SMALL_RIGHT2_X0			20
#define STAR_SMALL_RIGHT2_Y0			-25

class Star :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	virtual void Render();

	float LimitY;
	float LimitX;
	float v_start = 0.1f;
	int converging_level = 0;
	DWORD smoke_start = 0;
	DWORD time_increase_converging = 0;

	void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Star();
	virtual void SetState(int state);
	//void SetIsUsed(bool isused) { isUsed = isused; };
	//bool GetIsUsed() { return isUsed; };
	void StartSmoke() { smoke_start = GetTickCount(); }
	void GetReady();
	void Shot();
	void StartTimeReady() { time_increase_converging = GetTickCount(); };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void CalcPotentialCollisions(
		vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);
};

