#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define BLACKENEMY_WALKING_SPEED 0.05f;
#define BLACKENEMY_JUMP_SPEED_Y 0.25f
#define BLACKENEMY_FLY_SPEED_X 0.05f
#define BLACKENEMY_WALKING_SPEED_SLIDE_TRUE 0.1f
#define BLACKENEMY_WALKING_SPEED_SLIDE_FALSE 0.01f
#define BLACKENEMY_FLY_MAX 100
#define BLACKENEMY_GRAVITY 0.0008f
#define BLACKENEMY_DEFLECT_DIE 0.02f

#define BLACKENEMY_BBOX_WIDTH 16
#define BLACKENEMY_BBOX_HEIGHT 15
#define BLACKENEMY_BBOX_HEIGHT_DIE 9

#define BLACKENEMY_INCLINE_UP_SPEED_X_1 0.05f
#define BLACKENEMY_INCLINE_UP_SPEED_X_2 0.075f
#define BLACKENEMY_INCLINE_DOWN_SPEED_X_1 0.125f
#define BLACKENEMY_INCLINE_DOWN_SPEED_X_2 0.1f

#define BLACKENEMY_INCLINE_UP_SPEED_Y_1 0.065
#define BLACKENEMY_INCLINE_UP_SPEED_Y_2 0.04f
#define BLACKENEMY_INCLINE_DOWN_SPEED_Y_1 0.125f
#define BLACKENEMY_INCLINE_DOWN_SPEED_Y_2 0.05f

#define BLACKENEMY_STATE_WALKING 100
#define BLACKENEMY_STATE_FLYING 200

#define BLACKENEMY_STATE_INCLINE_DOWN 300
#define BLACKENEMY_STATE_INCLINE_UP 400

#define BLACKENEMY_STATE_JUMP 500
#define BLACKENEMY_STATE_FLYING 600
#define BLACKENEMY_STATE_DIE 700
#define BLACKENEMY_STATE_AUTO_GO_SLIDE_RIGHT 800
#define BLACKENEMY_STATE_AUTO_GO_SLIDE_LEFT 900

#define BLACKENEMY_ANI_WALK_RIGHT 0
#define BLACKENEMY_ANI_WALK_LEFT 1
#define BLACKENEMY_ANI_JUMP_RIGHT 2
#define BLACKENEMY_ANI_JUMP_LEFT 3
#define BLACKENEMY_ANI_FLY_RIGHT 4
#define BLACKENEMY_ANI_FLY_LEFT 5
#define BLACKENEMY_ANI_DIE 6

#define BLACKENEMY_TIME_WALKING 2000
#define BLACKENEMY_TIME_FLYING 10000

class BlackEnemy :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	DWORD time_state_walk = 0;
	//DWORD time_state_jump = 0;
	DWORD time_state_fly = 0;
public:
	bool isIncline = false; // check for sliding
	int incline_size; // size of slide
	int direct_incline;
	int direct_go = 1;
	bool isFly = false;
	int isFlying = 0;

	float incline_vx, incline_vy;

	bool isSlide = false;
	int slideType = 0;
	BlackEnemy();
	void StartJump() { time_state_walk = GetTickCount(); };
	void StartFly() { time_state_fly = GetTickCount(); };
	virtual void SetState(int state);
};