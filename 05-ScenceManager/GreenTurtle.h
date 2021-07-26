#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#include "Star.h"

#define GREEN_TURTLE_WALKING_SPEED 0.01f;
#define GREEN_TURTLE_GRAVITY 0.0008f
#define GREEN_TURTLE_BBOX_WIDTH 12
#define GREEN_TURTLE_BBOX_HEIGHT 17

#define GREEN_TURTLE_DEFLECT_SPEED 0.3f
#define GREEN_TURTLE_STATE_WALKING 100

#define GREEN_TURTLE_STATE_FLYING 200

#define GREEN_TURTLE_STATE_DIE 500

#define GREEN_TURTLE_ANI_WALK_RIGHT 0
#define GREEN_TURTLE_ANI_WALK_LEFT 1
#define GREEN_TURTLE_ANI_DIE 2


class GreenTurtle :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	float tempbacky;
	bool isDie = false;
	float yPositionDie = 0;
	bool back = false;
	GreenTurtle();
	virtual void SetState(int state);
};

