#pragma once
#include "GameObject.h"
#include "BlackEnemy.h"

#define BOOMBOSS_WALKING_TIME 1000
#define BOOMBOSS_WALKING_SPEED 0.02f;
#define BOOMBOSS_GRAVITY 0.0008f
#define BOOMBOSS_BE_ATTACKED_SPEED 0.08f;
#define BOOMBOSS_BBOX_WIDTH 32
#define BOOMBOSS_BBOX_HEIGHT 44
#define BOOMBOSS_BBOX_HEIGHT_DIE 39
#define BOOMBOSS_POSITION_FIRE_X 107

#define BOOMBOSS_STATE_WALKING 100

#define BOOMBOSS_STATE_ATTACKING 200

#define BOOMBOSS_STATE_BEING_ATTACKED 300
#define BOOMBOSS_STATE_DIE 400

#define BOOMBOSS_ANI_WALKING 0
#define BOOMBOSS_ANI_ATTACKING 1
#define BOOMBOSS_ANI_BEING_ATTACKED 2


class BoomBoss :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

public:
	int live = 3;
	BoomBoss();
	vector<BlackEnemy*> ListBomb;
	DWORD attacking_start = 0;
	float tempbacky;
	bool back = false;
	virtual void SetState(int state);
	void Fire();
	void StartAttacking() { attacking_start = GetTickCount(); }
	void CreateItem();
};