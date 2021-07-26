#pragma once
#include "GameObject.h"
#include "BlackEnemy.h"

#define GREEN_BOSS_WALKING_SPEED 0.1f
#define GREEN_BOSS_JUMP_SPEED 0.25f
#define GREEN_BOSS_GRAVITY 0.0008f
#define GREEN_BOSS_BBOX_WIDTH 20
#define GREEN_BOSS_BBOX_HEIGHT 30

#define GREEN_BOSS_STATE_WALKING 100
#define GREEN_BOSS_STATE_JUMP 200
#define GREEN_BOSS_STATE_DIE 300
#define GREEN_BOSS_STATE_IDLE 400


#define GREEN_BOSS_ANI_WALKING_RIGHT 0
#define GREEN_BOSS_ANI_WALKING_LEFT 1
#define GREEN_BOSS_ANI_JUMP_RIGHT 2
#define GREEN_BOSS_ANI_JUMP_LEFT 3
#define GREEN_BOSS_ANI_DIE 4

#define GREEN_BOSS_TIME_JUMP 2000

#define GREEN_BOSS_TIME_BACK 4000


class GreenBoss :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD time_state_jump = 0;
	DWORD time_back = -1;

public:
	int live = 3;
	GreenBoss();
	bool isBack = false;
	virtual void SetState(int state);
	void StartJump() { time_state_jump = GetTickCount(); };
};