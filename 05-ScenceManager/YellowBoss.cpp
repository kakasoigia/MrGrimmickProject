#include "YellowBoss.h"
#include "BlackEnemy.h"
#include "PlayScence.h"
#include "Utils.h"
#include <algorithm>
YellowBoss::YellowBoss()
{
	nx = -1;
	SetState(YELLOWBOSS_STATE_WALKING);



}
void YellowBoss::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CGimmick*>(coObjects->at(i)))
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
void YellowBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{

	if (state == YELLOWBOSS_STATE_DIE)
	{
		left = top = right = bottom = 0;
	}
	else
	{
		left = x;
		top = y - 3;
		right = x + YELLOWBOSS_BBOX_WIDTH;
		bottom = y - YELLOWBOSS_BBOX_HEIGHT;
	}
}

void YellowBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	vy -= YELLOWBOSS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// quay mặt về gimmick 

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	Star* star = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetStar();
	/*if (gimmick->nx == this->nx )*/
	{
		if (gimmick->x < this->x)
		{
			this->nx = -1;
		}
		else if (gimmick->x > this->x)
		{
			this->nx = 1;
		}

		//reset tốc độ sau khi quay mặt 
		if (state == YELLOWBOSS_STATE_WALKING && !isBeingScared) // khong bi hoang so di lui 
		{
			SetState(YELLOWBOSS_STATE_WALKING);
			//reset lai toc do di 
		}
		/*else if (state == YELLOWBOSS_STATE_BEING_ATTACKED)
		{
			if (nx > 0)
			{
				vx = YELLOWBOSS_BE_ATTACKED_SPEED;
			}
			else
			{
				vx = -YELLOWBOSS_BE_ATTACKED_SPEED;
			}
		}*/
	}




	//fallbacktime
	if (fallingback_start != 0)
	{
		if (GetTickCount() - fallingback_start > YELLOWBOSS_FALLING_BACK_TIME && state == YELLOWBOSS_STATE_BEING_ATTACKED)
		{
			fallingback_start = 0;
			SetState(YELLOWBOSS_STATE_WALKING);
		}
	}
	//scared

	// co sao la lui 
	if (this->state == YELLOWBOSS_STATE_WALKING)
	{
		if (star->state != STAR_STATE_DISAPPEAR)
		{
			if (star->state != STAR_STATE_FLYING)
			{
					if (scared_start != -1)
					{
						if (scared_start == 0)
						{
							StartScared();
							isBeingScared = true;
							if (gimmick->x < this->x)
							{
								vx = YELLOWBOSS_SCARED_SPEED;
							}
							else
							{
								vx = -YELLOWBOSS_SCARED_SPEED;
							}
						}
						else
						{
							if (GetTickCount() - scared_start > YELLOWBOSS_SCARED_TIME)
							{
								isBeingScared = false;
								SetState(YELLOWBOSS_STATE_WALKING);
								scared_start = -1;
							}
						}

					}


				
			}
			else
			{
				if (isBeingScared)
				{
					isBeingScared = false;
					SetState(YELLOWBOSS_STATE_WALKING);
				}
			}
			if (abs(star->x - this->x) < 40 && (star->state == STAR_STATE_FLYING || star->state == STAR_STATE_SMOKE))
			{
				isCutting = true;
			}
			else
			{
				isCutting = false;
			}

		}
		else //disappear star
		{
			if (isBeingScared)
			{
				isBeingScared = false;
				SetState(YELLOWBOSS_STATE_WALKING);
			}
			isCutting = false;
			scared_start = 0;
		}

	}

	//dung gan thi chem gimmick
	if (state == YELLOWBOSS_STATE_WALKING)
	{
		if (abs(gimmick->x - this->x) < 30)
		{
			isSliding = true;
		}
		else
		{
			isSliding = false;
		}
	}
	// di qua bien gioi thi comeback
	if (this->x < 1664)
	{
		isJump = true;
		vy += YELLOWBOSS_JUMP_SPEED;
		vx = 0.2;
	}
	else if (this->x > 2021)
	{
		isJump = true;
		vy += YELLOWBOSS_JUMP_SPEED;
		vx = -0.2;
	}
	if (gimmick->GetState() == GIMMICK_STATE_DIE)
	{
		SetState(YELLOWBOSS_STATE_WALKING);
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

		if (ny != 0) vy = 0;

		// Collision logic with other objects
	   //
		if (isJump == true)
		{
			isJump = false;
			SetState(YELLOWBOSS_STATE_WALKING);
		}
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
		/*tempbacky = y;*/
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void YellowBoss::Render()
{
	int ani = YELLOWBOSS_ANI_WALKING_LEFT;
	if (state == YELLOWBOSS_STATE_BEING_ATTACKED)
	{
		if (nx < 0)
		{
			ani = YELLOWBOSS_ANI_BEING_ATTACKED_LEFT;
		}
		else
		{
			ani = YELLOWBOSS_ANI_BEING_ATTACKED_RIGHT;
		}
	}
	else if (isJump)
	{
		if (nx < 0)
		{
			ani = YELLOWBOSS_ANI_JUMPING_LEFT;
		}
		else
		{
			ani = YELLOWBOSS_ANI_JUMPING_RIGHT;
		}
	}
	else if (isCutting || isSliding)
	{
		if (nx < 0)
		{
			ani = YELLOWBOSS_ANI_ATTACKING_LEFT;
		}
		else
		{
			ani = YELLOWBOSS_ANI_ATTACKING_RIGHT;
		}
	}
	else if (state == YELLOWBOSS_STATE_WALKING)
	{
		if (nx < 0)
		{
			ani = YELLOWBOSS_ANI_WALKING_LEFT;
		}
		else
		{
			ani = YELLOWBOSS_ANI_WALKING_RIGHT;
		}
	}

	else if (state == YELLOWBOSS_STATE_ATTACKING)
	{
		if (nx < 0)
		{
			ani = YELLOWBOSS_ANI_ATTACKING_LEFT;
		}
		else
		{
			ani = YELLOWBOSS_ANI_ATTACKING_RIGHT;
		}
	}
	if (state == YELLOWBOSS_STATE_DIE)
	{
		ani = YELLOWBOSS_ANI_DIE;
		animation_set->at(YELLOWBOSS_ANI_SWORD)->Render(x+20, y+30);
	}

	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void YellowBoss::SetState(int state)
{
	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	CGameObject::SetState(state);
	switch (state)
	{
	case YELLOWBOSS_STATE_DIE:
		vx = -vx;
		vy = 0.25f;
		CGame::GetInstance()->IncScore(500);
		CreateItem();
		break;
	case YELLOWBOSS_STATE_WALKING:
		if (nx > 0)
		{
			vx = YELLOWBOSS_WALKING_SPEED;
		}
		else
		{
			vx = -YELLOWBOSS_WALKING_SPEED;
		}

		break;
	case YELLOWBOSS_STATE_ATTACKING:
		attacking_start = GetTickCount();
		vx = 0;
		break;
	case YELLOWBOSS_STATE_BEING_ATTACKED:
		if (gimmick->x < this->x)
		{
			vx = YELLOWBOSS_BE_ATTACKED_SPEED;
		}
		else
		{
			vx = -YELLOWBOSS_BE_ATTACKED_SPEED;
		}

		StartFallingBack();
		break;
	}
}



void YellowBoss::CreateItem()
{
	Item* item = new Item(ITEM_TYPE_MEDICINE_PINK_BOMB);
	CAnimationSets* ani = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = ani->Get(62);
	item->SetAnimationSet(ani_set);
	item->SetPosition(88, 79);

	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->addObject(item);
}