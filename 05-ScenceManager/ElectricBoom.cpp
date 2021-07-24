
#include "ElectricBoom.h"
#include "PlayScence.h"
#include <algorithm>
ElectricBoom::ElectricBoom()
{
	SetState(ELECTRICBOOM_STATE_DISAPPEAR);
	nx = 1;
}

void ElectricBoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x;
		top = y - 10;
		right = x + ELECTRICBOOM_BBOX_WIDTH-3;
		bottom = y - ELECTRICBOOM_BBOX_HEIGHT;
}
void ElectricBoom::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<ElectricBoom*>(coObjects->at(i)))
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
void ElectricBoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (isFollowGimmick)
	{

		
		if (this->y - gimmick->y < 0)
		{
			if (x - gimmick->x > 0.5f)
			{
				nx = -1;
				vx = -ELECTRICBOOM_WALKING_SPEED;

			}
			else if (x - gimmick->x < 0.5f)
			{
				nx = 1;
				vx = ELECTRICBOOM_WALKING_SPEED;
			}
		}
	}

	
	// Simple fall down
	vy -= ELECTRICBOOM_GRAVITY * dt;

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

		if (back)
		{
			if (tempbacky - y >= 0.4f)
			{
				y += 1;

				if (vx < 0)
					x += 1;
				else
					x -= 1;
				vx = -vx;
				nx = -nx;
				isFollowGimmick = false;
			}
		}


	}
	else
	{

		// land ...fly
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		back = true;


		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		/*if (nx!=0) vx = 0;*/
		if (ny != 0) vy = 0;

		tempbacky = y;

		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			//if (e->nx != 0 && ny == 0)
			//{
			//	this->vx = -this->vx;
			//	this->nx = -this->nx;

			//}
		}
	}

	

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void ElectricBoom::Render()
{
	int ani = ELECTRICBOOM_ANI_WALK_RIGHT;
	if (state == ELECTRICBOOM_STATE_WALKING)
	{
		if (nx > 0)
			ani = ELECTRICBOOM_ANI_WALK_RIGHT;
		else
			ani = ELECTRICBOOM_ANI_WALK_LEFT;
	}
	else if (state == ELECTRICBOOM_STATE_FLYING)
	{
		if (nx > 0)
			ani = ELECTRICBOOM_ANI_FLY_RIGHT;
		else
			ani = ELECTRICBOOM_ANI_FLY_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void ElectricBoom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ELECTRICBOOM_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case ELECTRICBOOM_STATE_WALKING:
		if (nx > 0)
		{
			vx = ELECTRICBOOM_WALKING_SPEED;
			vy = ELECTRICBOOM_GRAVITY;
		}
		else
		{
			vx = -ELECTRICBOOM_WALKING_SPEED;
			vy = ELECTRICBOOM_GRAVITY;
		}
	}
}
//void ElectricBoom::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
//{
//
//	min_tx = 1.0f;
//	min_ty = 1.0f;
//	int min_ix = -1;
//	int min_iy = -1;
//
//	nx = 0.0f;
//	ny = 0.0f;
//
//	coEventsResult.clear();
//
//	for (UINT i = 0; i < coEvents.size(); i++)
//	{
//		LPCOLLISIONEVENT c = coEvents[i];
//
//		if (c->t < min_tx && c->nx != 0) {
//			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
//		}
//
//		if (c->t < min_ty && c->ny != 0) {
//			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
//		}
//
//		if (dynamic_cast<CGimmick*>(c->obj))
//		{
//			ny = -0.01f;;
//
//		}
//	}
//
//	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
//	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
//}
