#include <algorithm>
#include <assert.h>
#include "Star.h"
#include "Utils.h"
#include "Brick.h"
#include "BlackEnemy.h"
#include "BoomBoss.h"
#include "Worm.h"
#include "PlayScence.h"
#include "Slide.h"
#include "Incline.h"
#include "Thunder.h"
#include "Item.h"
#include "ElectricBoom.h"
Star::Star()
{
	SetState(STAR_STATE_DISAPPEAR);
	LimitY = STAR_FLYING_SPEED_Y;	LimitX = STAR_FLYING_SPEED_X;
	nx = 1;
}
void Star::FilterCollision(
	vector<LPCOLLISIONEVENT>& coEvents,
	vector<LPCOLLISIONEVENT>& coEventsResult,
	float& min_tx, float& min_ty,
	float& nx, float& ny, float& rdx, float& rdy)
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
			ny = 0;
			nx = 0;
		}
		if (dynamic_cast<Item*>(c->obj))
		{
			ny = 0;
			nx = 0;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void Star::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CGimmick*>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<Item*>(coObjects->at(i)))
		{
			continue;
		}
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Star::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != STAR_STATE_FLYING)
	{
		left = top = right = bottom = 0;

	}
	else
	{
		left = x;
		top = y;
		right = x + STAR_BBOX_WIDTH;
		bottom = y - STAR_BBOX_HEIGHT;
	}


}

void Star::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);

	if (state == STAR_STATE_DISAPPEAR) return;


	if (state == STAR_STATE_SMOKE)
	{
		if ((GetTickCount() - smoke_start > STAR_SMOKE_TIME))
		{
			smoke_start = 0;
			SetState(STAR_STATE_DISAPPEAR);
			return;
		}
	}

	//if (state == STAR_STATE_READY_TO_SHOT || state == STAR_STATE_LOADING)
	//{
	//	DebugOut(L"[INFO] qua ải xét state \n");
	//	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//	this->x = gimmick->x - 2;
	//	this->y = gimmick->y + 25;
	//	DebugOut(L"[INFO] vị trí x %d\n",(int) gimmick->x);
	//	DebugOut(L"[INFO] vị trí y %d\n",(int) gimmick->y);
	//	DebugOut(L"[INFO] Satet : %d \n", state);
	//}

	if (state == STAR_STATE_FLYING)
		vy -= STAR_GRAVITY * dt;
	// Simple fall down
	/*if (state != STAR_STATE_IDLING)
		vy -= STAR_GRAVITY * dt;*/

	if ((GetTickCount() - time_increase_converging > STAR_INC_CONVERG_TIME) && state == STAR_STATE_LOADING)
	{

		if (converging_level > 50)
		{

			/*converging_level = 0;*/
			SetState(STAR_STATE_READY_TO_SHOT);
			time_increase_converging = 0;
		}
		else
		{
			converging_level++;

			time_increase_converging = GetTickCount();
		}

	}
	if (state == STAR_STATE_FLYING)
	{
		if (LimitY <= 0 || LimitX <= 0)
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
		if (state == STAR_STATE_FLYING)
		{
			x += dx;
			y += dy;
		}



	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		/*x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;*/

		/*if (nx!=0) vx = 0;*/
	/*	if (ny != 0) vy = 0;*/

		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<Slide*>(e->obj) || dynamic_cast<Incline*>(e->obj))
			{

				// phương
				if (e->ny > 0)
				{
					LimitY -= STAR_DECREASE_SPEED_Y;
					vy = LimitY;
				}
				else if (e->ny < 0)
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
				else if (e->nx < 0)
				{
					LimitX -= STAR_DECREASE_SPEED_X;
					vx = -LimitY;
				}


				// giữ sao trên đầu

				// va chạm với mấy bạn Quái (quái điện phóng điện thủ )
				// bay trong thời gian quy định OUT 
				// bay ra màn hình ==> reset 
				// vo ham Hoa Khoi r ma k quay lai dem gio 
			}
			// chạm quái 
			 if (dynamic_cast<BlackEnemy*>(e->obj))
			{
				BlackEnemy* black = dynamic_cast<BlackEnemy*>(e->obj);
				black->SetState(BLACKENEMY_STATE_DIE);
				this->SetState(STAR_STATE_SMOKE);
			}
			 else if (dynamic_cast<BoomBoss*>(e->obj))
			 {
				 BoomBoss* boomBoss = dynamic_cast<BoomBoss*>(e->obj);
				 if (boomBoss->live != 1)
				 {
					 boomBoss->SetState(BOOMBOSS_STATE_BEING_ATTACKED);
					 //boomBoss->x -= 15;
					 //boomBoss->y += 5;
					 boomBoss->attacking_start = 0;
					 boomBoss->live--;
				 }
				 else
				 {
					 boomBoss->SetState(BOOMBOSS_STATE_DIE);
				 }
				 this->SetState(STAR_STATE_SMOKE);
			 }
			 if (dynamic_cast<Worm*>(e->obj))
			{
				Worm* worm = dynamic_cast<Worm*>(e->obj);
				worm->SetState(WORM_STATE_DIE);
				this->SetState(STAR_STATE_SMOKE);
			}
			 if (dynamic_cast<ElectricBoom*>(e->obj))
			{
				ElectricBoom* electricBoom = dynamic_cast<ElectricBoom*>(e->obj);
				electricBoom->SetState(ELECTRICBOOM_STATE_DIE);
				this->SetState(STAR_STATE_SMOKE);
			}
			 if (dynamic_cast<CThunder*>(e->obj))
			{
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
	int alpha = 255;
	if (state == STAR_STATE_DISAPPEAR) return;
	/*int ani = BULLET_ANI_FALLING_RIGHT;*/
	int ani = STAR_ANI_BIG_STAR;
	if (state == STAR_STATE_FLYING || state == STAR_STATE_READY_TO_SHOT)
	{
		// ??
		ani = STAR_ANI_BIG_STAR;
		animation_set->at(ani)->Render(x, y);
	}
	else if (state == STAR_STATE_SMOKE)
	{
		ani = STAR_ANI_BOOM;
		animation_set->at(ani)->Render(x, y);
	}
	else if (state == STAR_STATE_LOADING)
	{
		/*CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		gimmick->GetPosition(this->x, this->y);

		this->x -=  2;
		this->y +=  25;*/
		animation_set->at(STAR_ANI_SMALL_STAR)->Render(this->x + STAR_SMALL_MID_X0 - converging_level * STAR_SMALL_MID_X0 / 50 + 5, this->y + STAR_SMALL_MID_Y0 - converging_level * STAR_SMALL_MID_Y0 / 50 - 7, alpha);
		animation_set->at(STAR_ANI_SMALL_STAR)->Render(this->x + STAR_SMALL_LEFT1_X0 - converging_level * STAR_SMALL_LEFT1_X0 / 50 + 5, this->y + STAR_SMALL_LEFT1_Y0 - converging_level * STAR_SMALL_LEFT1_Y0 / 50 - 7, alpha);
		animation_set->at(STAR_ANI_SMALL_STAR)->Render(this->x + STAR_SMALL_LEFT2_X0 - converging_level * STAR_SMALL_LEFT2_X0 / 50 + 5, this->y + STAR_SMALL_LEFT2_Y0 - converging_level * STAR_SMALL_LEFT2_Y0 / 50 - 7, alpha);
		animation_set->at(STAR_ANI_SMALL_STAR)->Render(this->x + STAR_SMALL_RIGHT1_X0 - converging_level * STAR_SMALL_RIGHT1_X0 / 50 + 5, this->y + STAR_SMALL_RIGHT1_Y0 - converging_level * STAR_SMALL_RIGHT1_Y0 / 50 - 7, alpha);
		animation_set->at(STAR_ANI_SMALL_STAR)->Render(this->x + STAR_SMALL_RIGHT2_X0 - converging_level * STAR_SMALL_RIGHT2_X0 / 50 + 5, this->y + STAR_SMALL_RIGHT2_Y0 - converging_level * STAR_SMALL_RIGHT2_Y0 / 50 - 7, alpha);
		/*DebugOut(L"[INFO] vị trí  x %d\n", (int)gimmick->x);
		DebugOut(L"[INFO] vị trí  y %d\n", (int)gimmick->y);
		DebugOut(L"[INFO] vị trí render x %d\n", (int)x);
		DebugOut(L"[INFO] vị trí render y %d\n", (int)y);*/
		return;
	}


	RenderBoundingBox();
}

void Star::SetState(int state)
{
	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CGameObject::SetState(state);
	switch (state)
	{
	case STAR_STATE_DISAPPEAR:
		vx = vy = 0;

		break;
	case STAR_STATE_FLYING:
		LimitY = STAR_FLYING_SPEED_Y;
		LimitX = STAR_FLYING_SPEED_X;
		if (gimmick->GetJumping() == 1)
		{
			vy = -LimitY;
		}
		else
		{
			vy = LimitY;
		}

		/*+ gimmick ->vy*/;
		if (nx > 0)
		{
			x = gimmick->x + 5;
			vx = LimitX;
			/*vx = LimitX + gimmick->vx/3;*/
		}
		else
		{
			/*vx = -LimitX + gimmick->vx / 3;*/
			x = gimmick->x - 5;
			vx = -LimitX;
		}
		this->y = gimmick->y + 20;

		break;
	case STAR_STATE_SMOKE:
		StartSmoke();

		break;
	case STAR_STATE_READY_TO_SHOT:
		break;
	case STAR_STATE_LOADING:
		break;
	}


}
void Star::GetReady()
{
	if (state == STAR_STATE_DISAPPEAR)
	{
		StartTimeReady();
		SetState(STAR_STATE_LOADING);
	}
}
void Star::Shot()
{

	if (state == STAR_STATE_READY_TO_SHOT)
	{

		CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		nx = gimmick->nx;
		converging_level = 0;
		SetState(STAR_STATE_FLYING);
	}
	else
	{
		SetState(STAR_STATE_DISAPPEAR);
		converging_level = 0;
	}
}

