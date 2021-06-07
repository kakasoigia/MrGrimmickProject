
#include "Slide.h"
Slide::Slide()
{
	SetState(SLIDE_STATE_SLIDING_LEFT);
	nx = 1;
}

void Slide::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SLIDE_BBOX_WIDTH;
	bottom = y + SLIDE_BBOX_HEIGHT;
}

void Slide::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	
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

void Slide::Render()
{
	int ani = SLIDE_ANI_SLIDING_RIGHT;
	if (state == SLIDE_STATE_SLIDING_RIGHT)
	{
		ani = SLIDE_ANI_SLIDING_RIGHT;
		
	}
	else
	{
		ani = SLIDE_ANI_SLIDING_LEFT;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Slide::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	
	}
}
