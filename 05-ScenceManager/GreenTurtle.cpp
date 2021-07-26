#include "GreenTurtle.h"
GreenTurtle::GreenTurtle()
{
	nx = 1;
	SetState(GREEN_TURTLE_STATE_WALKING);

}

void GreenTurtle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GREEN_TURTLE_BBOX_WIDTH;
	if (!isDie)
	{
		bottom = y - GREEN_TURTLE_BBOX_HEIGHT;
	}
	else
	{
		bottom = y - GREEN_TURTLE_BBOX_HEIGHT + 1;
	}

}

void GreenTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= GREEN_TURTLE_GRAVITY * dt;

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
		if (state != GREEN_TURTLE_STATE_DIE)
		{
			if (back)
			{
				if (tempbacky - y >= 0.5f)
				{
					y += 5;
					if (vx < 0)
						x += 12;
					else
						x -= 12;
					vx = -vx;
					nx = -nx;
				}
			}
		}
		else if (state == GREEN_TURTLE_STATE_DIE)
		{
			//this -> y = yPositionDie;
			this->vx = 0;
		}
	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		back = true;

		if (state != GREEN_TURTLE_STATE_DIE || !isDie)
		{
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;
		}


		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

		tempbacky = y;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Star*>(e->obj))
			{
				if (state != GREEN_TURTLE_STATE_DIE)
				{
					SetState(GREEN_TURTLE_STATE_DIE);
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void GreenTurtle::Render()
{
	int ani = GREEN_TURTLE_ANI_WALK_RIGHT;
	if (state == GREEN_TURTLE_STATE_WALKING)
	{
		if (vx > 0)
			ani = GREEN_TURTLE_ANI_WALK_RIGHT;
		else
			ani = GREEN_TURTLE_ANI_WALK_LEFT;
	}
	else if (state == GREEN_TURTLE_STATE_DIE)
	{
		/*if (!isDie) {
			yPositionDie = this->y;
		}*/
		ani = GREEN_TURTLE_ANI_DIE;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void GreenTurtle::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_TURTLE_STATE_DIE:
		break;
	case GREEN_TURTLE_STATE_WALKING:
		if (nx < 0)
		{
			vx = -GREEN_TURTLE_WALKING_SPEED;
		}
		else
		{
			vx = GREEN_TURTLE_WALKING_SPEED;
		}
	}
}
