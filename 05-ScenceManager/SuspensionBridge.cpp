
#include "SuspensionBridge.h"
#include "Utils.h"
#include "PlayScence.h"
SuspensionBridge::SuspensionBridge()
{
	SetState(BRIDGE_STATE_IDLING);
	nx = 1;
	isOpening = false;
}

void SuspensionBridge::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (isOpening)
	{
		left = top = right = bottom = 0;
		return;
	}
		
	// boudingbox của lồng nằm dưới lồng
	left = x;
	top = y- BRIDGE_BBOX_HEIGHT+4; // trừ hao cái thành cầu
	right = x + BRIDGE_BBOX_WIDTH;
	bottom = y - BRIDGE_BBOX_HEIGHT - BRIDGE_BBOX_HEIGHT;
}

void SuspensionBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	
			x += dx;
			y += dy;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	
	if (GetTickCount() - time_count_moving > BRIDGE_TIME_MOVING && state != BRIDGE_STATE_IDLING && !isOpening )
	{
		isOpening = true;
		time_count_moving = 0;
		vx = 0;
		StartOpening();
		// ngừng chuyển động GImmick
		CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		gimmick->SetOnBridge(false);
	}
	if (isOpening)
	{
		if (GetTickCount() - time_count_opening > BRIDGE_TIME_OPENING && state != BRIDGE_STATE_IDLING_OPEN)
		{
			
			time_count_opening = 0;
			SetState(BRIDGE_STATE_IDLING_OPEN);
		}
	}
	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	LPGAMEOBJECT obj = coObjects->at(i);

	//}
	//CalcPotentialCollisions(coObjects, coEvents);
	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{

	//	x += dx;
	//	y += dy;

	//}
	//else
	//{

	//	// land ...fly
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	/*if (nx!=0) vx = 0;*/
	//	if (ny != 0) vy = 0;

	//	// Collision logic with other objects
	//	//
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		/*if (e->nx != 0 && ny == 0)
	//		{
	//			this->vx = -this->vx;
	//			this->nx = -this->nx;

	//		}*/
	//	}
	//}

	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void SuspensionBridge::Render()
{
	int ani = BRIDGE_ANI_CLOSING;
	if (state == BRIDGE_STATE_MOVING)
	{
		if (isOpening)
			ani = BRIDGE_ANI_OPENING;
		else
			ani = BRIDGE_ANI_MOVING_RIGHT;
	}
	else if (state == BRIDGE_STATE_IDLING)
	{
			ani = BRIDGE_ANI_CLOSING;
	}
	else if (state == BRIDGE_STATE_IDLING_OPEN)
	{
		ani = BRIDGE_ANI_OPENING_IDLING;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void SuspensionBridge::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BRIDGE_STATE_IDLING:
		vx = 0;
		break;
	case BRIDGE_STATE_MOVING:
		StartMoving();
		vx = BRIDGE_MOVING_SPEED;
		break;
	}
}
