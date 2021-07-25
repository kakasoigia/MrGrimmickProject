#include "BlackEnemy.h"
#include "Incline.h"
#include "Game.h"
#include "Slide.h"
#include "Brick.h"
#include "Utils.h"
#include "PlayScence.h"
#include "Star.h"
#include <algorithm>
BlackEnemy::BlackEnemy()
{
	SetState(BLACKENEMY_STATE_WALKING);
	//StartJump();
	nx = -1;
}

void BlackEnemy::CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<BlackEnemy*>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CPipes*>(coObjects->at(i)))
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
			//ny = -0.01f;
		}
		if (dynamic_cast<Star*>(c->obj))
		{
			ny = 0;
			nx = 0;
		}
		/*if (dynamic_cast<CBrick*>(c->obj))
		{
		if(isFly)
		ny = 0;
		}*/
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void BlackEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == BLACKENEMY_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x;
		top = y;
		right = x + BLACKENEMY_BBOX_WIDTH;

		if (state == BLACKENEMY_STATE_DIE)
			bottom = y - BLACKENEMY_BBOX_HEIGHT_DIE;
		else
			bottom = y - BLACKENEMY_BBOX_HEIGHT;
	}
}

void BlackEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	if (!isIncline && state != BLACKENEMY_STATE_FLYING && !isFly)
		vy -= BLACKENEMY_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();

	if (state != BLACKENEMY_STATE_DIE && ids != 3)
	{
		if (vx < 0)
		{
			if (x - gimmick->x < -30)
				vx = -vx;
		}
		else
		{
			if (x - gimmick->x > 30)
				vx = -vx;
		}
		if (isFly)
		{
			if (isFlying != 0)
			{
				if (GetTickCount() - isFlying > 1000)
				{
					vy = 0;
					isFlying = 0;
				}
			}
			else
			{
				if (y - gimmick->y > 8)
				{
					vy = -0.05f;
				}
				else
				{
					if (y - gimmick->y < -8)
					{
						vy = 0.05f;
					}
					else
					{
						if ((int)y == (int)gimmick->y)
							vy = 0;
					}
				}
			}
		}

		if (time_state_walk == 0)
		{
			StartJump();
		}
		else
		{
			if (GetTickCount() - time_state_walk > BLACKENEMY_TIME_WALKING && state == BLACKENEMY_STATE_WALKING && !isFly)
			{
				SetState(BLACKENEMY_STATE_JUMP);
				time_state_walk = 0;
			}
		}

		if (time_state_fly == 0)
		{
			StartFly();
		}
		else {
			if (GetTickCount() - time_state_fly > BLACKENEMY_TIME_FLYING && state == BLACKENEMY_STATE_WALKING)
			{
				isFly = true;
				SetState(BLACKENEMY_STATE_FLYING);
				isFlying = GetTickCount();
			}
		}
		if (isSlide)
		{
			if (vx > 0 && !isFly)
			{
				SetState(BLACKENEMY_STATE_AUTO_GO_SLIDE_RIGHT);
			}
			else if (isSlide && vx < 0 && !isFly)
			{
				SetState(BLACKENEMY_STATE_AUTO_GO_SLIDE_LEFT);
			}
		}
		else
		{
			if (vx > 0)
			{
				vx = BLACKENEMY_WALKING_SPEED;
			}
			else if (vx < 0)
				vx = -BLACKENEMY_WALKING_SPEED;
		}
	}
	else
	{
		isIncline = false;
		isFly = false;
	}
	//
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
		isIncline = false;
	}
	else
	{
		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Incline*>(e->obj)) {

				isIncline = true;

				float tran_y = -99999;

				Incline* incline = dynamic_cast<Incline*>(e->obj);

				if (incline->size == 1)
					incline_size = 1;
				else
					incline_size = 2;
				if (!isFly)
					if (vx > 0)
					{
						direct_go = 1;
						if (incline->direct == 1) {
							SetState(BLACKENEMY_STATE_INCLINE_UP);
						}
						else {
							SetState(BLACKENEMY_STATE_INCLINE_DOWN);
						}
					}
					else
					{
						direct_go = -1;
						if (incline->direct == 1) {
							SetState(BLACKENEMY_STATE_INCLINE_DOWN);
						}
						else {
							SetState(BLACKENEMY_STATE_INCLINE_UP);
						}
					}
			}
			else {
				isIncline = false;
				if (!isFly)
					SetState(BLACKENEMY_STATE_WALKING);
			}
			if (dynamic_cast<Slide*>(e->obj))
			{
				isSlide = true;
				Slide* slide = dynamic_cast<Slide*>(e->obj);
				if (slide->GetType() == SLIDE_TYPE_LEFT)
				{
					slideType = -1;
				}
				else if (slide->GetType() == SLIDE_TYPE_RIGHT)
				{
					slideType = 1;
				}
			}
			else
			{
				isSlide = false;
			}

		}
		if (!isIncline) {

			x += min_tx * dx + nx * 0.4f;
			//x += dx;
			y += min_ty * dy + ny * 0.4f;

			/*if (nx != 0) vx = 0;*/
			if (ny != 0) vy = 0;
		}
		else {
			x += dx;
			if (isIncline) {
				y += min_ty * dy + ny * 0.4f;
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
	{
		if (vx > 0)
			ani = BLACKENEMY_ANI_WALK_RIGHT;
		else
			ani = BLACKENEMY_ANI_WALK_LEFT;
	}
	else if (state == BLACKENEMY_STATE_JUMP)
	{
		if (vx > 0)
			ani = BLACKENEMY_ANI_JUMP_RIGHT;
		else
			ani = BLACKENEMY_ANI_JUMP_LEFT;
	}
	else if (state == BLACKENEMY_STATE_FLYING)
	{
		if (vx > 0)
			ani = BLACKENEMY_ANI_FLY_RIGHT;
		else
			ani = BLACKENEMY_ANI_FLY_LEFT;
	}
	else if (state == BLACKENEMY_STATE_AUTO_GO_SLIDE_RIGHT)
	{
		ani = BLACKENEMY_ANI_WALK_RIGHT;
	}
	else if (state == BLACKENEMY_STATE_AUTO_GO_SLIDE_LEFT)
	{
		ani = BLACKENEMY_ANI_WALK_LEFT;
	}
	else if (state == BLACKENEMY_STATE_DIE)
	{
		ani = BLACKENEMY_ANI_DIE;
	}
	else
	{
		if (vx > 0)
			ani = BLACKENEMY_ANI_WALK_RIGHT;
		else
			ani = BLACKENEMY_ANI_WALK_LEFT;
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
		vx = 0;
		vy = BLACKENEMY_DEFLECT_DIE;
		break;
	case BLACKENEMY_STATE_WALKING:
		if (vx < 0)
		{
			vx = -BLACKENEMY_WALKING_SPEED;
		}
		else
		{
			vx = BLACKENEMY_WALKING_SPEED;
		}
		break;
	case BLACKENEMY_STATE_INCLINE_UP:
	{
		if (direct_go == 1)
		{
			if (incline_size == 1) {
				vx = BLACKENEMY_INCLINE_UP_SPEED_X_1;
				vy = BLACKENEMY_INCLINE_UP_SPEED_Y_1;
			}
			else {
				vx = BLACKENEMY_INCLINE_UP_SPEED_X_2;
				vy = BLACKENEMY_INCLINE_UP_SPEED_Y_2;
			}
		}
		else //if (direct_go == -1)
		{
			if (incline_size == 1) {
				vx = -BLACKENEMY_INCLINE_UP_SPEED_X_1;
				vy = BLACKENEMY_INCLINE_UP_SPEED_Y_1;
			}
			else {
				vx = -BLACKENEMY_INCLINE_UP_SPEED_X_2;
				vy = BLACKENEMY_INCLINE_UP_SPEED_Y_2;
			}
		}
	case BLACKENEMY_STATE_FLYING:
		if (vx > 0)
		{
			vy = 0.05f;
			vx = BLACKENEMY_FLY_SPEED_X;
		}
		else
		{
			vy = 0.05f;
			vx = -BLACKENEMY_FLY_SPEED_X;
		}
		break;
	case BLACKENEMY_STATE_JUMP:
		vy = BLACKENEMY_JUMP_SPEED_Y;
		break;
	case BLACKENEMY_STATE_AUTO_GO_SLIDE_LEFT:
		if (slideType == -1)
		{
			vx = -BLACKENEMY_WALKING_SPEED_SLIDE_TRUE;
		}
		else
		{
			vx = -BLACKENEMY_WALKING_SPEED_SLIDE_FALSE;
		}
		nx = -1;
		break;
	case BLACKENEMY_STATE_AUTO_GO_SLIDE_RIGHT:
		if (slideType == 1)
		{
			vx = BLACKENEMY_WALKING_SPEED_SLIDE_TRUE;
		}
		else
		{
			vx = BLACKENEMY_WALKING_SPEED_SLIDE_FALSE;
		}
		nx = 1;
		break;
	case BLACKENEMY_STATE_INCLINE_DOWN:
	{
		if (direct_go == 1)
		{
			if (incline_size == 1) {

				vx = BLACKENEMY_INCLINE_DOWN_SPEED_X_1;
				vy = -BLACKENEMY_INCLINE_DOWN_SPEED_Y_1;
			}
			else {
				vx = BLACKENEMY_INCLINE_DOWN_SPEED_X_2;
				vy = -BLACKENEMY_INCLINE_DOWN_SPEED_Y_2;
			}
		}
		else if (direct_go == -1)
		{
			if (incline_size == 1) {

				vx = -BLACKENEMY_INCLINE_DOWN_SPEED_X_1;
				vy = -BLACKENEMY_INCLINE_DOWN_SPEED_Y_1;
			}
			else {

				vx = -BLACKENEMY_INCLINE_DOWN_SPEED_X_2;
				vy = -BLACKENEMY_INCLINE_DOWN_SPEED_Y_2;
			}
		}
		break;
	}
	}
	}
}