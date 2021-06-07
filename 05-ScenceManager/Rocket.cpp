
#include "Rocket.h"
Rocket::Rocket()
{
	SetState(ROCKET_STATE_IDLING);
	nx = 1;
}

void Rocket::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ROCKET_BBOX_WIDTH;
	bottom = y + ROCKET_BBOX_HEIGHT;
}

void Rocket::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	if (state == ROCKET_STATE_FALLING )
	vy += ROCKET_GRAVITY * dt;

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

void Rocket::Render()
{
	int ani = ROCKET_ANI_GREEN;
	if (state == ROCKET_STATE_IDLING)
	{
		ani = ROCKET_ANI_GREEN;
	}
	else if (state == ROCKET_STATE_FALLING)
	{
		ani = ROCKET_ANI_RED;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Rocket::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ROCKET_STATE_IDLING:

		break;
	case ROCKET_STATE_FALLING:
		break;
	}
}
