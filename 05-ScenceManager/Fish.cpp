
#include "Fish.h"
#include "Utils.h"
Fish::Fish()
{
	//SetState(FISH_STATE_SWIMMING_LEFT);
	nx = 1;
	vx = FISH_SWIMMING_SPEED;
	vy = 0;
	StartSwimming();
}


void Fish::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = top = bottom = right = 0;
}

void Fish::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	/*vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	 coEvents.clear();*/
	

	if (GetTickCount() - swimming_start > FISH_SWIMMING_TIME)
	{
		nx = -nx;
		vx = -vx;
		swimming_start = GetTickCount();
	}
	x += dx;

	//for (UINT i = 0; i < coObjects->size(); i++)
   //{
   // LPGAMEOBJECT obj = coObjects->at(i);

	//}
   //CalcPotentialCollisions(coObjects, coEvents);
   ////// No collision occured, proceed normally
   //if (coEvents.size() == 0)
   //{

	// x += dx;
   // y += dy;

	//}
   //else
   //{

	//// // land ...fly
   // float min_tx, min_ty, nx = 0, ny;
   // float rdx = 0;
   // float rdy = 0;
   // FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	// x += min_tx * dx + nx * 0.4f;
   // y += min_ty * dy + ny * 0.4f;

	//// /*if (nx!=0) vx = 0;*/
   // if (ny != 0) vy = 0;

	//// // Collision logic with other objects
   //// //
   // for (UINT i = 0; i < coEventsResult.size(); i++)
   // {
   // LPCOLLISIONEVENT e = coEventsResult[i];

	// if (e->nx != 0 && ny == 0)
   // {
   // this->vx = -this->vx;
   // this->nx = -this->nx;

	// }
   // }
   //}

	////// clean up collision events
   //for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Fish::Render()
{
	/*if (state == FISH_STATE_SWIMMING_LEFT)
	{
	{
	switch (Type)
	{
	case FISH_TYPE_YELLOW:
	ani = FISH_YELLOW_ANI_SWIMMING_LEFT;
	break;
	case FISH_TYPE_BLACK:
	ani = FISH_BLACK_ANI_SWIMMING_LEFT;
	break;
	case FISH_TYPE_RED:
	ani = FISH_RED_ANI_SWIMMING_LEFT;
	break;
	}
	}
	}*/
	int ani = FISH_ANI_LEFT;
	if (nx < 0)
	{
		ani = FISH_ANI_LEFT;
	}
	else
	{
		ani = FISH_ANI_RIGHT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Fish::SetState(int state)
{
	CGameObject::SetState(state);
	/*switch (state)
	{
	case FISH_STATE_SWIMMING_RIGHT:

	 break;
	case FISH_STATE_SWIMMING_LEFT:
	break;
	}*/
}