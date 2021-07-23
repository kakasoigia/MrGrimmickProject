#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define BRIDGE_MOVING_SPEED 0.01f;

#define BRIDGE_BBOX_WIDTH 36
#define BRIDGE_BBOX_HEIGHT 63
#define BRIDGE_TIME_MOVING	5000
#define BRIDGE_TIME_OPENING	200
#define BRIDGE_STATE_MOVING 100

#define BRIDGE_STATE_IDLING 200
#define BRIDGE_STATE_IDLING_OPEN 3000

#define  BRIDGE_ANI_CLOSING 0

#define BRIDGE_ANI_MOVING_RIGHT 1
#define BRIDGE_ANI_OPENING 2
#define BRIDGE_ANI_OPENING_IDLING 3
class SuspensionBridge :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	
	DWORD time_count_moving = 0;;
	DWORD time_count_opening = 0;;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	bool isOpening = false;
	SuspensionBridge();
	virtual void SetState(int state);
	void StartMoving() { time_count_moving = GetTickCount(); }
	bool GetIsOpening() { return isOpening; }
	void SetIsOpening(bool _isOpening) { isOpening = _isOpening; };
	void StartOpening() { time_count_opening = GetTickCount(); }
};

