
#include "Star.h"
#include "Utils.h"
#include "Brick.h"
#include "BlackEnemy.h"
#include "BoomBoss.h"
#include "Worm.h"
Star::Star()
{
	SetState(STAR_STATE_IDLING);
	LimitY= STAR_FLYING_SPEED_Y;	LimitX = STAR_FLYING_SPEED_X;
	nx = 1;
}

void Star::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (isUsed == false)
	{
		left = top = right = bottom = 0;
		return;
	 }
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
	if (GetTickCount() - smoke_start > STAR_SMOKE_TIME && state == STAR_STATE_SMOKE)
	{ 
		isUsed = false; 
		smoke_start = 0;

	}
	if (LimitY <=0 || LimitX <= 0) 
	{
		if (state != STAR_STATE_SMOKE)
		{
			vx = vy = 0;
			SetState(STAR_STATE_SMOKE);
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
					LimitY -= STAR_DECREASE_SPEED_Y;
					vy = LimitY;
				}
				else if (e->ny > 0)
				{
					LimitY -= STAR_DECREASE_SPEED_Y;
					vy = -LimitY;
				}
				// phương dọc
				if (e->nx > 0)
				{
					LimitX -= STAR_DECREASE_SPEED_X;
					vx = LimitY;
				}
				else if(e->nx < 0)
				{
					LimitX -= STAR_DECREASE_SPEED_X;
					vx = -LimitY;
				}
				// phương chéo ???
				
				// chỉnh lại V cho phù hợp
				// bay loạn
				// giữ sao trên đầu
				// bắn 5 sao 1 lần
				// va chạm với mấy bạn Quái (quái điện phóng điện thủ )
				// bay trong thời gian quy định OUT 
				// tìm lại sprite ..Đĩ Hiếu
			}
			// chạm quái 
			else if (dynamic_cast<BlackEnemy*>(e->obj))
			{
				BlackEnemy* black = dynamic_cast<BlackEnemy*>(e->obj);
				black->SetState(BLACKENEMY_STATE_DIE);
				this->SetState(STAR_STATE_SMOKE);
			}
			else if (dynamic_cast<BoomBoss*>(e->obj))
			{
				BoomBoss* boomBoss = dynamic_cast<BoomBoss*>(e->obj);
				boomBoss->SetState(BOOMBOSS_STATE_DIE);
				this->SetState(STAR_STATE_SMOKE);
			}
			else if (dynamic_cast<Worm*>(e->obj))
			{
				Worm* worm = dynamic_cast<Worm*>(e->obj);
				worm->SetState(WORM_STATE_DIE);
				this->SetState(STAR_STATE_SMOKE);
			}
			// set time nổ khói 
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Star::Render()
{
	if(isUsed == false) return;
	/*int ani = BULLET_ANI_FALLING_RIGHT;*/
	int ani = STAR_ANI_BIG_STAR;
	if (state == STAR_STATE_FLYING)
	{
		ani = STAR_ANI_BIG_STAR;
	}
	else if (state == STAR_STATE_SMOKE)
	{
		ani = STAR_ANI_BOOM;

	}

	animation_set->at(ani)->Render(x, y);

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
		LimitY = STAR_FLYING_SPEED_Y;	LimitX = STAR_FLYING_SPEED_X;
		vy = LimitY;
		if (nx > 0)
			vx = LimitX;
		else
		{
			vx = -LimitX;
		}
		break;
	case STAR_STATE_SMOKE:
		StartSmoke();
		break;
	}
	

}
