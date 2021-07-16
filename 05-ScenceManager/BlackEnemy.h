#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define BLACKENEMY_WALKING_SPEED 0.01f;
#define BLACKENEMY_GRAVITY 0.0004f
#define BLACKENEMY_BBOX_WIDTH 16
#define BLACKENEMY_BBOX_HEIGHT 15
#define BLACKENEMY_BBOX_HEIGHT_DIE 9

#define BLACKENEMY_STATE_WALKING 100

#define BLACKENEMY_STATE_FLYING 200

#define BLACKENEMY_STATE_DIE 500

#define BLACKENEMY_ANI_WALK_RIGHT 0
#define BLACKENEMY_ANI_WALK_LEFT 1
#define BLACKENEMY_ANI_FLY_RIGHT 2
#define BLACKENEMY_ANI_FLY_LEFT 3

class BlackEnemy :
    public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
	bool isUsed = false;
public:
	BlackEnemy();
	virtual void SetState(int state);
	void SetIsUsed(bool isused) { isUsed = isused; };
	bool GetIsUsed() { return isUsed; };
};

