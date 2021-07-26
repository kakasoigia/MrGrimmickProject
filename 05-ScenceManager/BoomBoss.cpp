#include "BoomBoss.h"
#include "BlackEnemy.h"
#include "PlayScence.h"
#include "Utils.h"
BoomBoss::BoomBoss()
{
	SetState(BOOMBOSS_STATE_WALKING);
	nx = 1;
	for (int i = 0; i < 6; i++) {

		BlackEnemy* bomb = new BlackEnemy();

		CAnimationSets* ani = CAnimationSets::GetInstance();
		LPANIMATION_SET ani_set = ani->Get(3);
		bomb->SetAnimationSet(ani_set);
		ListBomb.push_back(bomb);

		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->addObject(bomb);
	}
}

void BoomBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (state == BOOMBOSS_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x;
		top = y;
		right = x + BOOMBOSS_BBOX_WIDTH;
		if (state == BOOMBOSS_STATE_ATTACKING)
		{
			bottom = y - 46;
		}
		else
			bottom = y - BOOMBOSS_BBOX_HEIGHT;
	}
}

void BoomBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= BOOMBOSS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (attacking_start != 0)
	{
		if (GetTickCount() - attacking_start > 3000)
		{
			SetState(BOOMBOSS_STATE_BEING_ATTACKED);
			Fire();
			attacking_start = 0;
		}
	}
	else
	{
		if (state == BOOMBOSS_STATE_BEING_ATTACKED)
		{
			//y += 5;
			if (x < 52)
			{
				x = 52;
				SetState(BOOMBOSS_STATE_WALKING);
			}

			//SetState(BOOMBOSS_STATE_ATTACKING);
		}
		if (x > 82)
		{
			x = 82;
			y += 5;
			SetState(BOOMBOSS_STATE_ATTACKING);
			vx = 0;
		}
	}



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
	else if (state == BOOMBOSS_STATE_DIE)
	{
		ani = 3;
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
		vx = 0;
		vy = 0.05f;
		CreateItem();
		break;
	case BOOMBOSS_STATE_WALKING:
		vx = BOOMBOSS_WALKING_SPEED;
		break;
	case BOOMBOSS_STATE_ATTACKING:
		attacking_start = GetTickCount();
		vx = 0;
		break;
	case BOOMBOSS_STATE_BEING_ATTACKED:
		vx = -BOOMBOSS_BE_ATTACKED_SPEED;
		break;
	}
}

void BoomBoss::Fire()
{
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects();
	int count = 0;
	for (int i = 0; i < 6; i++)
	{
		ListBomb[i]->SetPosition(x + 25 + i * 17, y + i * 3);
		ListBomb[i]->vy = 0.03f * (i + 1);
		ListBomb[i]->SetState(BLACKENEMY_STATE_WALKING);
	}
}

void BoomBoss::CreateItem()
{
	Item* item = new Item(ITEM_TYPE_MEDICINE_PINK_BOMB);
	CAnimationSets* ani = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = ani->Get(62);
	item->SetAnimationSet(ani_set);
	item->SetPosition(88, 79);

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->addObject(item);
}