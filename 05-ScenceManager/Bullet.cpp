#include "Utils.h"
#include "Bullet.h"
#include "Thunder.h"
#include "Incline.h"
#include "Star.h"
Bullet::Bullet()
{
	SetState(BULLET_STATE_DISAPPEAR);
	nx = 1;
}
void Bullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == BULLET_STATE_DISAPPEAR)
	{
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y;
	right = x + BULLET_BBOX_WIDTH;
	bottom = y - BULLET_BBOX_HEIGHT;
}

void Bullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	if (state == BULLET_STATE_FALLING)
		vy -= BULLET_GRAVITY * dt;


	// thời gian hiện animation nổ
	if (state == BULLET_STATE_DESTROY)
	{
		if ((GetTickCount() - booming_start > 500) )
		{
			SetState(BULLET_STATE_DISAPPEAR);
			booming_start = 0;
		}
	}
	

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

			if (dynamic_cast<CThunder*>(e->obj))
			{
				if(state != BULLET_STATE_DESTROY)
					SetState(BULLET_STATE_DESTROY);
				this->vx = 0;
			}
			if (dynamic_cast<Star*>(e->obj))
			{
				if (state != BULLET_STATE_DESTROY)
					SetState(BULLET_STATE_DESTROY);
				this->vx = 0;
				Star* star = dynamic_cast<Star*>(e->obj);
				star->SetState(STAR_STATE_SMOKE);
			}
			if (dynamic_cast<Incline*>(e->obj)) 
			{
				Incline* incline = dynamic_cast<Incline*>(e->obj);

				if (incline->direct > 0) {
					isInclineHeightToLow = true;
					if (isInclineHeightToLow) {
						this->nx = -1;
						vx = -BULLET_WALKING_SPEED;
					}
				}
			}
			else this->nx = 1;
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Bullet::Render()
{
	int ani = BULLET_ANI_FALLING;
	if (state == BULLET_STATE_FALLING)
	{
		ani = BULLET_ANI_FALLING;
	}
	else if (state == BULLET_STATE_DESTROY)
	{
		ani = BULLET_ANI_DESTROY;
	}
	else if (state == BULLET_STATE_DISAPPEAR)
	{
		return;
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Bullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BULLET_STATE_DESTROY:
		StartBooming();
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
