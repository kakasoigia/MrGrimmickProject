
#include "BoomBoss.h"
BoomBoss::BoomBoss()
{
	SetState(BOOMBOSS_STATE_WALKING);
	nx = 1;
}

void BoomBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMBOSS_BBOX_WIDTH;

	if (state == BOOMBOSS_STATE_DIE)
		bottom = y + BOOMBOSS_BBOX_HEIGHT_DIE;
	else
		bottom = y + BOOMBOSS_BBOX_HEIGHT;
}

void BoomBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy += BOOMBOSS_GRAVITY * dt;

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

void BoomBoss::Render()
{
	int ani = BOOMBOSS_ANI_WALK_RIGHT;
	if (state == BOOMBOSS_STATE_WALKING)
	{
		if (nx > 0)
			ani = BOOMBOSS_ANI_WALK_RIGHT;
		else
			ani = BOOMBOSS_ANI_WALK_LEFT;
	}
	else if (state == BOOMBOSS_STATE_FLYING)
	{
		if (nx > 0)
			ani = BOOMBOSS_ANI_FLY_RIGHT;
		else
			ani = BOOMBOSS_ANI_FLY_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void BoomBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMBOSS_STATE_DIE:
		y += BOOMBOSS_BBOX_HEIGHT - BOOMBOSS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case BOOMBOSS_STATE_WALKING:
		if (nx < 0)
		{
			vx = -BOOMBOSS_WALKING_SPEED;
		}
		else
		{
			vx = BOOMBOSS_WALKING_SPEED;
		}
	}
}
