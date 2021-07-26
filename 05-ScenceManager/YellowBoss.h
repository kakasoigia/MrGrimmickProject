#pragma once
#include "GameObject.h"
#include "BlackEnemy.h"

#define YELLOWBOSS_WALKING_TIME 1000
#define YELLOWBOSS_FALLING_BACK_TIME 200
#define YELLOWBOSS_SCARED_TIME 300
#define YELLOWBOSS_WALKING_SPEED 0.1f;
#define YELLOWBOSS_JUMP_SPEED 0.2f;
#define YELLOWBOSS_SCARED_SPEED 0.25f;
#define YELLOWBOSS_GRAVITY 0.0008f

#define YELLOWBOSS_BE_ATTACKED_SPEED 0.08f;

#define YELLOWBOSS_BBOX_WIDTH 29
#define YELLOWBOSS_BBOX_HEIGHT 31
#define YELLOWBOSS_BBOX_HEIGHT_DIE 39

#define YELLOWBOSS_STATE_IDLE 50
#define YELLOWBOSS_STATE_WALKING 100

#define YELLOWBOSS_STATE_ATTACKING 200

#define YELLOWBOSS_STATE_BEING_ATTACKED 300
#define YELLOWBOSS_STATE_DIE 400

#define YELLOWBOSS_ANI_WALKING_LEFT 1
#define YELLOWBOSS_ANI_WALKING_RIGHT 0
#define YELLOWBOSS_ANI_ATTACKING_RIGHT 2
#define YELLOWBOSS_ANI_ATTACKING_LEFT 3
#define YELLOWBOSS_ANI_JUMPING_RIGHT 4
#define YELLOWBOSS_ANI_JUMPING_LEFT 5
#define YELLOWBOSS_ANI_BEING_ATTACKED_RIGHT 6 
#define YELLOWBOSS_ANI_BEING_ATTACKED_LEFT 7 
#define YELLOWBOSS_ANI_DIE 8 
#define YELLOWBOSS_ANI_SWORD 9 

class YellowBoss :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	bool isJump = false; 
	void CalcPotentialCollisions(
		vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);

public:
	int live = 3;
	YellowBoss();
	
	DWORD attacking_start = 0;
	//fallback
	bool isFallBack = false;
	DWORD fallingback_start = 0;
	//cutting
	bool isCutting = false;
	DWORD cutting_start = 0;
	// scare when see star
	bool isBeingScared = false;
	DWORD scared_start = 0;
	virtual void SetState(int state);
	
	
	// cutting 2
	bool isSliding = false;
	void StartAttacking() { attacking_start = GetTickCount(); }
	void StartFallingBack() { fallingback_start = GetTickCount(); }
	void StartCutting() { cutting_start = GetTickCount(); }
	void StartScared() { scared_start = GetTickCount(); }
	void CreateItem();
};