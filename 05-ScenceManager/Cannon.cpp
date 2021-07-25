
#include "Cannon.h"
#include "Bullet.h"
#include "Utils.h"

Cannon::Cannon()
{
	SetState(CANNON_STATE_GREEN);
	nx = 1;
}

Cannon::Cannon(bool isRight)
{
	SetState(CANNON_STATE_GREEN);
	nx = 1;
}

void Cannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CANNON_BBOX_WIDTH;
	bottom = y - CANNON_BBOX_HEIGHT;
}

void Cannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= CANNON_GRAVITY * dt;


	if (state == CANNON_STATE_GREEN)
	{
		CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (abs(gimmick->x - this->x) < 140 && (gimmick->y - this->y) < 300) this->SetState(CANNON_STATE_RED);
	}

	if (getTimeFire == -1)
	{
		getTimeFire = GetTickCount();
	}
	else
	{
		if (GetTickCount() - getTimeFire > 2000 && state == CANNON_STATE_RED)
		{
			vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects();

			for (UINT i = 0; i < objects.size(); i++)
			{
				if (dynamic_cast<Bullet*>(objects.at(i)))
				{
					Bullet* bullet = dynamic_cast<Bullet*>(objects.at(i));

					if (bullet->GetState() == BULLET_STATE_DISAPPEAR)
					{
						bullet->SetState(BULLET_STATE_FALLING);
						bullet->SetPosition(x + 16, y);
						break;
					}
				}
			}
			getTimeFire = GetTickCount();
		}
		else if(GetTickCount() - getTimeFire > 500 && state == CANNON_STATE_RED) SetState(CANNON_STATE_GREEN);
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
	}

	// clean up collision events

	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Cannon::Render()
{
	/*int ani = CANNON_ANI_GREEN_LEFT;
	if (state == CANNON_STATE_GREEN)
	{
		if (nx > 0)
			ani = CANNON_ANI_GREEN_RIGHT;
		else
			ani = CANNON_ANI_GREEN_LEFT;
	}
	else if (state == CANNON_STATE_RED)
	{
		if (nx > 0)
			ani = CANNON_ANI_RED_RIGHT;
		else
			ani = CANNON_ANI_RED_LEFT;
	}*/
	int ani = CANNON_ANI_GREEN_RIGHT;
	if (state == CANNON_STATE_GREEN)
	{
			ani = CANNON_ANI_GREEN_RIGHT;
	}
	else if (state == CANNON_STATE_RED)
	{
			ani = CANNON_ANI_RED_RIGHT;
	
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Cannon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case CANNON_STATE_GREEN:
	
		break;
	case CANNON_STATE_RED:
		

		break;
	}
}
