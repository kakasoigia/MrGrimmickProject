
#include "Star.h"
#include "Utils.h"
#include "Brick.h"
Star::Star()
{
	SetState(STAR_STATE_IDLING);
	LimitY= STAR_FLYING_SPEED_Y;	LimitX = STAR_FLYING_SPEED_X;
	nx = 1;
}

void Star::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + STAR_BBOX_WIDTH;
	bottom = y - STAR_BBOX_HEIGHT;
}

void Star::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	vy -= STAR_GRAVITY * dt;
	// Simple fall down
	/*if (state != STAR_STATE_IDLING)
		vy -= STAR_GRAVITY * dt;*/

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
		/*if (vy <= 0)
		{*/
			
		/*}
		else
		{

		}
		*/

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
	/*	if (ny != 0) vy = 0;*/

		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				
				// phương
				if (e->ny > 0)
				{
					LimitY -= 0.005f;
					vy = LimitY;
				}
				else if (e->nx > 0)
				{
					LimitY -= 0.005f;
					vy = -LimitY;
				}
				// phương dọc
				if (e->nx > 0)
				{
					LimitX -= 0.005f;
					vx = LimitY;
				}
				else if(e->nx < 0)
				{
					LimitX -= 0.005f;
					vx = -LimitY;
				}
				// phương chéo ???
				
				// chỉnh lại V cho phù hợp
				// va chạm với mấy thằng lz Quái
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Star::Render()
{
	
	
	if(isUsed == false) return;
	/*int ani = BULLET_ANI_FALLING_RIGHT;*/
	//if (state == BULLET_STATE_IDLING)
	//{
	//	ani = BULLET_ANI_IDLING;
	//}
	//else if (state == BULLET_STATE_FALLING)
	//{
	//	/*if (nx > 0)
	//		ani = BULLET_ANI_FALLING_RIGHT;
	//	else
	//		ani = BULLET_ANI_FALLING_LEFT;*/
	//	
	//}
	DebugOut(L"[ERR] VÔ ĐÂY \n");
	animation_set->at(1)->Render(x, y);

	RenderBoundingBox();
}

void Star::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case STAR_STATE_IDLING:
		isUsed = false;
		vx = 0;
		break;
	case STAR_STATE_FLYING:
		isUsed = true;
		vy = LimitY;
		if (nx > 0)
			vx = LimitX;
		else
		{
			vx = -LimitX;
		}
		break;
	}
}
