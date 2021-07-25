#pragma once
#include "GameObject.h"
#include "Gimmick.h"

#define BULLET_GRAVITY 0.001f
#define	BULLET_WALKING_SPEED 0.1f

#define BULLET_BBOX_WIDTH 15
#define BULLET_BBOX_HEIGHT 15

#define BULLET_ANI_FALLING 0
#define BULLET_ANI_DESTROY 1


#define BULLET_STATE_FALLING 200
#define BULLET_STATE_DISAPPEAR 300
#define BULLET_STATE_DESTROY 400

#define BULLET_BOOMING_TIME 500

//#define BULLET_ANI_IDLING	0
//#define BULLET_ANI_FALLING_LEFT 1
//#define BULLET_ANI_FALLING_RIGHT 2



class Bullet :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	DWORD booming_start = 0;
	bool isInclineHeightToLow = false;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Bullet();
	void StartBooming() { booming_start = GetTickCount(); }
	virtual void SetState(int state);
};

