
#include "BlackEnemy.h"
#include "BLACKENEMY.h"
BlackEnemy::BlackEnemy()
{
	SetState(BLACKENEMY_STATE_WALKING);
	nx = 1;
}
void BlackEnemy::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		
		if (dynamic_cast<CGimmick*>(c->obj))
		{
			ny = -0.01f;;

		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void BlackEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLACKENEMY_BBOX_WIDTH;

	if (state == BLACKENEMY_STATE_DIE)
		bottom = y - BLACKENEMY_BBOX_HEIGHT_DIE;
	else
		bottom = y - BLACKENEMY_BBOX_HEIGHT;
}

void BlackEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= BLACKENEMY_GRAVITY * dt;

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

		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny!=0) vy = 0;

		

		//
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

void BlackEnemy::Render()
{
	int ani = BLACKENEMY_ANI_WALK_RIGHT;
	if (state == BLACKENEMY_STATE_WALKING) 
	{ if (nx>0)
		ani = BLACKENEMY_ANI_WALK_RIGHT;
		else
		ani = BLACKENEMY_ANI_WALK_LEFT;
	}
	else if (state == BLACKENEMY_STATE_FLYING)
	{
		if (nx > 0)
			ani = BLACKENEMY_ANI_FLY_RIGHT;
		else
			ani = BLACKENEMY_ANI_FLY_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void BlackEnemy::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BLACKENEMY_STATE_DIE:
		y += BLACKENEMY_BBOX_HEIGHT - BLACKENEMY_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case BLACKENEMY_STATE_WALKING:
		if (nx < 0)
		{
			vx = -BLACKENEMY_WALKING_SPEED;
		}
		else 
		{
			vx = BLACKENEMY_WALKING_SPEED;
		}
	}
}
