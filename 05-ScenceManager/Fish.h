#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define FISH_SWIMMING_SPEED 0.07f
#define FISH_BBOX_WIDTH 16
#define FISH_BBOX_HEIGHT 15

#define FISH_SWIMMING_TIME 2000

#define FISH_TYPE_RED 100
#define FISH_TYPE_BLACK 200
#define FISH_TYPE_YELLOW 300


#define FISH_STATE_SWIMMING_RIGHT 300
#define FISH_STATE_SWIMMING_LEFT 200

#define FISH_ANI_RIGHT 1
#define FISH_ANI_LEFT 0
class Fish :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int Type;
	DWORD swimming_start = 0;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Fish();

	void StartSwimming() { swimming_start = GetTickCount(); }
	virtual void SetState(int state);
};