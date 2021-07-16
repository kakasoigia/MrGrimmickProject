#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define WINDOW_BBOX_WIDTH 16
#define WINDOW_BBOX_HEIGHT 15


#define WINDOW_STATE_OPEN 100

#define WINDOW_STATE_CLOSE 200

#define  WINDOW_ANI_OPEN 1
#define WINDOW_ANI_CLOSE 0


class Window :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	

	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Window();
	virtual void SetState(int state);
	void CallBoom();
	
};

