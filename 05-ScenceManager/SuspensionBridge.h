#pragma once
#include "GameObject.h"
#include "Gimmick.h"
#define BRIDGE_MOVING_SPEED 0.01f;

#define BRIDGE_BBOX_WIDTH 16
#define BRIDGE_BBOX_HEIGHT 15


#define BRIDGE_STATE_MOVING 100

#define BRIDGE_STATE_IDLING 200

#define BRIDGE_ANI_MOVING_LEFT 0
#define BRIDGE_ANI_MOVING_RIGHT 1

#define BRIDGE_ANI_CLOSING	3
#define BRIDGE_ANI_OPENING 4
class SuspensionBridge :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	bool isOpening;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	SuspensionBridge();
	virtual void SetState(int state);
};

