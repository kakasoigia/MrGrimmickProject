
#include "Bullet.h"
Bullet::Bullet()
{
	SetState(BULLET_STATE_IDLING);
	nx = 1;
}

void Bullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y + BULLET_BBOX_HEIGHT;
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	if (state != BULLET_STATE_IDLING)
		vy += BULLET_GRAVITY * dt;

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

void Bullet::Render()
{
	int ani = BULLET_ANI_FALLING_RIGHT;
	if (state == BULLET_STATE_IDLING)
	{
		ani = BULLET_ANI_IDLING;
	}
	else if (state == BULLET_STATE_FALLING)
	{
		if (nx > 0)
			ani = BULLET_ANI_FALLING_RIGHT;
		else
			ani = BULLET_ANI_FALLING_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Bullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_STATE_IDLING:

		break;
	case BULLET_STATE_FALLING:
		if (nx < 0)
		{
			vx = -BULLET_WALKING_SPEED;
		}
		else
		{
			vx = BULLET_WALKING_SPEED;
		}
		break;
	}
}
