
#include "SuspensionBridge.h"
SuspensionBridge::SuspensionBridge()
{
	SetState(BRIDGE_STATE_IDLING);
	nx = 1;
	isOpening = false;
}

void SuspensionBridge::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BRIDGE_BBOX_WIDTH;
	bottom = y - BRIDGE_BBOX_HEIGHT;
}

void SuspensionBridge::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

	}
	CalcPotentialCollisions(coObjects, coEvents);
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{

		x += dx;
		y += dy;

	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->nx != 0 && ny == 0)
			{
				this->vx = -this->vx;
				this->nx = -this->nx;

			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void SuspensionBridge::Render()
{
	int ani = BRIDGE_ANI_CLOSING;
	if (state == BRIDGE_STATE_MOVING)
	{
		
			ani = BRIDGE_ANI_MOVING_RIGHT;
		
	}
	if (state == BRIDGE_STATE_IDLING)
	{
		if (isOpening)
			ani = BRIDGE_ANI_OPENING;
		else
			ani = BRIDGE_ANI_CLOSING;
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
		
		break;
	case BRIDGE_STATE_MOVING:
		if (nx < 0)
		{
			vx = -BRIDGE_MOVING_SPEED;
		}
		else
		{
			vx = BRIDGE_MOVING_SPEED;
		}
	}
}
