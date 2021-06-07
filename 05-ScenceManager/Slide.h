#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define SLIDE_WALKING_SPEED 0.01f;

#define SLIDE_BBOX_WIDTH 16
#define SLIDE_BBOX_HEIGHT 15

#define SLIDE_TYPE_LEFT 10
#define	SLIDE_TYPE_RIGHT 20

#define SLIDE_STATE_MOVING 500

#define SLIDE_ANI_SLIDING 0


class Slide :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	int  Type; 
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Slide();
	Slide(int type);
	int GetType() { return Type; };
	void SetType(int type) { Type = type; };
	
	virtual void SetState(int state);
};

