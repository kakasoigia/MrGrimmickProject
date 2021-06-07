
#include "FloatBox.h"
FloatBox::FloatBox()
{
	SetState(FLOATBOX_STATE_FLYING_UP);
	nx = 1;
}

void FloatBox::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLOATBOX_BBOX_WIDTH;
	bottom = y + FLOATBOX_BBOX_HEIGHT;
}

void FloatBox::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	/*vy += CANNON_GRAVITY * dt;*/

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

void FloatBox::Render()
{
	//int ani = FLOATBOX_ANI_FLYING_UP;
	//if (state == FLOATBOX_STATE_FLYING_UP)
	//{
	//	
	//		ani = FLOATBOX_ANI_FLYING_UP;
	//}
	//else if (state == FLOATBOX_STATE_FLYING_DOWN)
	//{

	//	ani = FLOATBOX_ANI_FLYING_DOWN;
	//}

	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void FloatBox::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FLOATBOX_STATE_FLYING_DOWN:

		break;
	case FLOATBOX_STATE_FLYING_UP:
		break;
	}
}
