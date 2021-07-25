
#include "ElectricBoom.h"
#include "PlayScence.h"
#include <algorithm>
#include "Portal.h"
ElectricBoom::ElectricBoom()
{
	SetState(ELECTRICBOOM_STATE_DISAPPEAR);
	nx = 1;
}

void ElectricBoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == ELECTRICBOOM_STATE_DIE  || state == ELECTRICBOOM_STATE_DISAPPEAR)
	{
		left = top = right = bottom = 0;
		return;
	}

	left = x;
	top = y - 10;
	right = x + ELECTRICBOOM_BBOX_WIDTH - 3;
	bottom = y - ELECTRICBOOM_BBOX_HEIGHT;
}
void ElectricBoom::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<ElectricBoom*>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CPortal*>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CGimmick*>(coObjects->at(i)))
		{
			continue;
		}
		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void ElectricBoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (isFollowGimmick)
	{
		if (this->y + 10 > gimmick->y)
		{
			if (x - gimmick->x > 0.5f)
			{
				nx = -1;
				vx = -ELECTRICBOOM_WALKING_SPEED;
			}
			else if (x - gimmick->x < 0.5f)
			{
				nx = 1;
				vx = ELECTRICBOOM_WALKING_SPEED;
			}
		}
	}
	if (this->x < 633 /*&&	back = true;*/)
	{
		this->nx = 1;
		this->vx = -vx;
		x = 637;
	}
	// Simple fall down
	vy -= ELECTRICBOOM_GRAVITY * dt;

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
		isFollowGimmick = false;

		/*if ((back == true) && nx == -1 && state != ELECTRICBOOM_STATE_DIE)
		{
			if (tempbacky - y >= 0.4f)
			{
				y += 1;

				if (vx < 0)
					x += 1;
				else
					x -= 1;
				vx = -vx;
				nx = -nx;
				isFollowGimmick = false;
			}
		}*/


	}
	else
	{
		isFollowGimmick = true;
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
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void ElectricBoom::Render()
{
	if (state == ELECTRICBOOM_STATE_DISAPPEAR) return;
	int ani = ELECTRICBOOM_ANI_WALK_RIGHT;
	if (state == ELECTRICBOOM_STATE_WALKING)
	{
		if (nx > 0)
			ani = ELECTRICBOOM_ANI_WALK_RIGHT;
		else
			ani = ELECTRICBOOM_ANI_WALK_LEFT;
	}
	/*else if (state == ELECTRICBOOM_STATE_FLYING)
	{
		if (nx > 0)
			ani = ELECTRICBOOM_ANI_FLY_RIGHT;
		else
			ani = ELECTRICBOOM_ANI_FLY_LEFT;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void ElectricBoom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ELECTRICBOOM_STATE_DIE:
	
		vy = 0.2f;
		break;
	case ELECTRICBOOM_STATE_WALKING:
		if (nx > 0)
		{
			vx = ELECTRICBOOM_WALKING_SPEED;
			vy = ELECTRICBOOM_GRAVITY;
		}
		else
		{
			vx = -ELECTRICBOOM_WALKING_SPEED;
			vy = ELECTRICBOOM_GRAVITY;
		}
		break;
	}
}
