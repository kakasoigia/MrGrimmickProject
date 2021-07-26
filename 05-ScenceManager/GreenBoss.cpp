#include "GreenBoss.h"
#include "Utils.h"
#include "PlayScence.h"

GreenBoss::GreenBoss()
{
	SetState(GREEN_BOSS_STATE_IDLE);
	nx = 1;
}

void GreenBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (state == GREEN_BOSS_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x;
		top = y;
		right = x + GREEN_BOSS_BBOX_WIDTH;
		bottom = y - GREEN_BOSS_BBOX_HEIGHT;
	}
}

void GreenBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	if (state != GREEN_BOSS_STATE_DIE)
	{
		vy -= GREEN_BOSS_GRAVITY * dt;
	}

	if (live == 0)
	{
		SetState(GREEN_BOSS_STATE_DIE);
	}

	if (state == GREEN_BOSS_STATE_IDLE)
	{
		CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (abs(gimmick->x - this->x) < 100 && (gimmick->y - this->y) < 100) {
			this->vx = GREEN_BOSS_WALKING_SPEED;
			this->SetState(GREEN_BOSS_STATE_WALKING);
		}
	}
	/*if (state == GREEN_BOSS_STATE_WALKING)
	{
		if (GetTickCount() - time_state_jump > GREEN_BOSS_TIME_JUMP)
		{
			SetState(GREEN_BOSS_STATE_JUMP);
			time_state_jump = GetTickCount();
			SetState(GREEN_BOSS_STATE_WALKING);
		}
	}*/

	if (time_back == -1) {
		time_back = GetTickCount();
	}
	else {
		if (GetTickCount() - time_back > GREEN_BOSS_TIME_BACK) {
			vx = -vx;
			nx = -nx;
			time_back = GetTickCount();
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
	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		/*back = true;*/
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

		// Collision logic with other objects
	   //
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0) {
				if (GetTickCount() - time_state_jump > GREEN_BOSS_TIME_JUMP)
				{
					SetState(GREEN_BOSS_STATE_JUMP);
					time_state_jump = GetTickCount();
					SetState(GREEN_BOSS_STATE_WALKING);
				}
			}
		}
		/*tempbacky = y;*/
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void GreenBoss::Render()
{
	int ani = GREEN_BOSS_ANI_WALKING_RIGHT;

	if (state == GREEN_BOSS_STATE_DIE)
	{
		ani = GREEN_BOSS_ANI_DIE;
	}
	else if (state == GREEN_BOSS_STATE_WALKING)
	{
		if (vx > 0)
		{
			ani = GREEN_BOSS_ANI_WALKING_RIGHT;
		}
		else
		{
			ani = GREEN_BOSS_ANI_WALKING_LEFT;
		}
	}
	else if (state == GREEN_BOSS_STATE_JUMP)
	{
		if (vx > 0)
		{
			ani = GREEN_BOSS_ANI_JUMP_RIGHT;
		}
		else
		{
			ani = GREEN_BOSS_ANI_JUMP_LEFT;
		}
	}


	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void GreenBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GREEN_BOSS_STATE_DIE:
		vy = -0.02f;
		vx = 0;
		break;
	case GREEN_BOSS_STATE_JUMP:
		vy = GREEN_BOSS_JUMP_SPEED;
		break;
	}
}
