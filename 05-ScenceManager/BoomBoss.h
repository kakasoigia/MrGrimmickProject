#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define BOOMBOSS_WALKING_SPEED 0.01f;
#define BOOMBOSS_GRAVITY 0.0008f
#define BOOMBOSS_BBOX_WIDTH 16
#define BOOMBOSS_BBOX_HEIGHT 15
#define BOOMBOSS_BBOX_HEIGHT_DIE 9

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
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	BoomBoss();
	virtual void SetState(int state);
};
