#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#include "PlayScence.h"

#define CANNON_GRAVITY 0.0008f
#define CANNON_BBOX_WIDTH 12
#define CANNON_BBOX_HEIGHT 15


#define CANNON_STATE_GREEN 100

#define CANNON_STATE_RED 200

#define CANNON_STATE_INCLINE_DOWN 300
#define CANNON_STATE_INCLINE_UP 400
#define CANNON_STATE_DIE 500
#define CANNON_STATE_DESTROY 600

#define CANNON_INCLINE_UP_SPEED_X_1 0.05f
#define CANNON_INCLINE_UP_SPEED_X_2 0.075f
#define CANNON_INCLINE_DOWN_SPEED_X_1 0.125f
#define CANNON_INCLINE_DOWN_SPEED_X_2 0.1f

#define CANNON_INCLINE_UP_SPEED_Y_1 0.065
#define CANNON_INCLINE_UP_SPEED_Y_2 0.04f
#define CANNON_INCLINE_DOWN_SPEED_Y_1 0.125f
#define CANNON_INCLINE_DOWN_SPEED_Y_2 0.05f

#define CANNON_ANI_GREEN_RIGHT 0

#define CANNON_ANI_RED_RIGHT 1

#define CANNON_ANI_DIE 2


class Cannon :
	public CGameObject
{
	float getTimeFire = -1;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD booming_start = 0;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:

	bool isIncline = false; // check for sliding
	int incline_size; // size of slide
	int direct_incline;
	int direct_go = 1;
	bool isFly = false;
	int isFlying = 0;
	void StartBooming() { booming_start = GetTickCount(); }

	Cannon();
	Cannon(bool isRight);
	virtual void SetState(int state);
};

