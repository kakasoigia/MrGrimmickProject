
#include "BoomBoss.h"
#include "BlackEnemy.h"
#include "PlayScence.h"
#include "Utils.h"
BoomBoss::BoomBoss()
{
	SetState(BOOMBOSS_STATE_WALKING);
	nx = 1;
}

void BoomBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOOMBOSS_BBOX_WIDTH;

	if (state == BOOMBOSS_STATE_DIE)
		bottom = y - BOOMBOSS_BBOX_HEIGHT_DIE;
	else
		bottom = y - BOOMBOSS_BBOX_HEIGHT;
}

void BoomBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= BOOMBOSS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	/*if (GetTickCount() - attacking_start > 2000 && state == CANNON_STATE_RED)
   {
   vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects(); for (UINT i = 0; i < objects.size(); i++)
   {
   if (dynamic_cast<Bullet*>(objects.at(i)))
   {
   Bullet* bullet = dynamic_cast<Bullet*>(objects.at(i)); if (bullet->GetState() == BULLET_STATE_DISAPPEAR)
   {
   bullet->SetState(BULLET_STATE_FALLING);
   bullet->SetPosition(x + 16, y);
   break;
   }
   }
   }
   attacking_start = GetTickCount();*/

	SetState(BOOMBOSS_STATE_ATTACKING);

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
		/*if (vx == BOOMBOSS_POSITION_FIRE_X)
		{
		SetState(BOOMBOSS_STATE_ATTACKING);
		}*/

		/*if (GetTickCount() - attacking_start > ROCKET_BOOMING_TIME && state == ROCKET_STATE_BOOM)
	   {
	   SetState(ROCKET_STATE_DISAPPEAR);
	   attacking_start = GetTickCount;

		}*/

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

		}
		/*tempbacky = y;*/
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void BoomBoss::Render()
{
	int ani = BOOMBOSS_ANI_WALKING;
	if (state == BOOMBOSS_STATE_WALKING)
	{
		ani = BOOMBOSS_ANI_WALKING;
	}
	else if (state == BOOMBOSS_STATE_BEING_ATTACKED)
	{
		ani = BOOMBOSS_ANI_BEING_ATTACKED;
	}
	else if (state == BOOMBOSS_STATE_ATTACKING)
	{
		ani = BOOMBOSS_ANI_ATTACKING;
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void BoomBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOOMBOSS_STATE_DIE:
		y += BOOMBOSS_BBOX_HEIGHT - BOOMBOSS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case BOOMBOSS_STATE_WALKING:
		vx = BOOMBOSS_WALKING_SPEED;
		break;
	case BOOMBOSS_STATE_ATTACKING:
		vx = 0;
		vy = 0;
		break;
		/*case BOOMBOSS_STATE_DIE:*/

	}
}

void BoomBoss::Fire()
{
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects();
	int count = 0;
	DebugOut(L"[ERROR] DCM TAN NGU 1!\n");
	for (UINT i = 0; i < objects.size(); i++)
	{
		DebugOut(L"[ERROR] DCM TAN NGU 2!\n");
		if (count == 6) return;
		if (dynamic_cast<BlackEnemy*>(objects.at(i)))
		{
			BlackEnemy* blackenemy = dynamic_cast<BlackEnemy*>(objects.at(i));

			{
				DebugOut(L"[ERROR] DCM KHOA NGU!\n");
				blackenemy->SetPosition(this->x, this->y + 5);
				blackenemy->nx = this->nx;
				switch (count)
				{
				case 1:
					blackenemy->vx = 0.1f;
					nx = 1;
					break;
				case 2:
					blackenemy->vx = 0.12f;
					nx = 1;
					break;
				case 3:
					blackenemy->vx = 0.14f;
					nx = 1;
					break;
				case 4:
					blackenemy->vx = 0.16f;
					nx = 1;
					break;
				case 5:
					blackenemy->vx = 0.15f;
					nx = 1;
					break;
				}
				blackenemy->SetState(BLACKENEMY_STATE_WALKING);
				count++;
				continue;
			}
		}
	}
}