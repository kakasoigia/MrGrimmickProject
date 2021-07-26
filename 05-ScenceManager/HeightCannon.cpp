#include "HeightCannon.h"
#include "Utils.h"

HeightCannon::HeightCannon()
{
	SetState(HEIGHT_CANNON_STATE_IDLE);
	nx = 1;
}

void HeightCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + HEIGHT_CANNON_BBOX_WIDTH;
	bottom = y - HEIGHT_CANNON_BBOX_HEIGHT;
}

void HeightCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (!gimmick->isOnTopCannon)
		vy -= HEIGHT_CANNON_GRAVITY * dt;

	if (gimmick->isPush) {
		SetState(HEIGHT_CANNON_STATE_MOVE);
	}
	else {
		vx = 0;
		SetState(HEIGHT_CANNON_STATE_IDLE);
	}


	/*	if (state == HEIGHT_CANNON_STATE_IDLE)
		{
			CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (abs(gimmick->x - this->x) < 140 && (gimmick->y - this->y) < 300) this->SetState(HeightCannon_STATE_RED);
		}

		if (getTimeFire == -1)
		{
			getTimeFire = GetTickCount();
		}
		else
		{
			if (GetTickCount() - getTimeFire > 2000 && state == HeightCannon_STATE_RED)
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
			else if (GetTickCount() - getTimeFire > 500 && state == HeightCannon_STATE_RED) SetState(HeightCannon_STATE_GREEN);
	}*/

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

void HeightCannon::Render()
{
	/*int ani = HeightCannon_ANI_GREEN_LEFT;
	if (state == HeightCannon_STATE_GREEN)
	{
		if (nx > 0)
			ani = HeightCannon_ANI_GREEN_RIGHT;
		else
			ani = HeightCannon_ANI_GREEN_LEFT;
	}
	else if (state == HeightCannon_STATE_RED)
	{
		if (nx > 0)
			ani = HeightCannon_ANI_RED_RIGHT;
		else
			ani = HeightCannon_ANI_RED_LEFT;
	}*/
	int ani = HEIGHT_CANNON_ANI_IDLE;
	if (state == HEIGHT_CANNON_STATE_IDLE)
	{
		ani = HEIGHT_CANNON_ANI_IDLE;
	}
	else if (state == HEIGHT_CANNON_STATE_MOVE)
	{
		ani = HEIGHT_CANNON_ANI_MOVE;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void HeightCannon::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case HEIGHT_CANNON_STATE_MOVE:
		//StartBooming();
		break;
		/*case HeightCannon_STATE_DESTROY:
			StartBooming();
			break;*/
	case HEIGHT_CANNON_STATE_IDLE:
		//StartBooming();
		break;
	}
}
