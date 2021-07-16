#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Gimmick.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"
#include "BlackEnemy.h"
#include "Rocket.h"
#include "SuspensionBridge.h"
#include "Star.h"
CGimmick::CGimmick(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if(!isOnTopBlackEnemy)
	vy -= GIMMICK_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (jump == true && doubleJump_start != 0)
	{
		if (GetTickCount() - doubleJump_start > 100)
		{
			vy = GIMMICK_DOUBLE_JUMP_SPEED;
			doubleJump_start = 0;
		}
	}

	if (maxjumping == 1)
	{
		if (GetTickCount() - time_maxjumping >= 200)
		{     
			maxjumping = 0;
			time_maxjumping = 0;
		}
	}
	else
	{
		maxjumping = 0;
		time_maxjumping = 0;
	}
	// turn off collision when die 
	if (state!= GIMMICK_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	
	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		
		SetOnTopBlackEnemy(false);
		{
			x += dx;
			y += dy;
		}
		
		
		
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0; 
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		// block every object first!
	
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;

		if (ny != 0 && nx == 0)
		{
			if (GetJumping() == 1)
			{
				jump = 0;
				doubleJump_start = 0;

			}

			
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if(!dynamic_cast<BlackEnemy*>(e->obj))  this->SetOnTopBlackEnemy(false);

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -GIMMICK_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
								SetState(GIMMICK_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<BlackEnemy*>(e->obj)) // if e->obj is Goomba 
			{
				BlackEnemy* blackenemy = dynamic_cast<BlackEnemy*>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (blackenemy->GetState() != BLACKENEMY_STATE_DIE)
					{
						SetOnTopBlackEnemy(true);
						x = blackenemy->x ;
						y = blackenemy->y - GIMMICK_BIG_BBOX_HEIGHT - 2;
						vy = 0;

					}
				}
				else
				{
					SetOnTopBlackEnemy(false);
				}
				/*else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (blackenemy->GetState() != GOOMBA_STATE_DIE)
						{
							SetState(GIMMICK_STATE_DIE);
						}
					}
				}*/
			} // if Goomba
			else if (dynamic_cast<Rocket*>(e->obj))
			{
				if (untouchable == 0)
				{
					/*if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						SetState(GIMMICK_STATE_DIE);
					}*/
				}
				else
				{
					StartUntouchable();
				}
				
				
			}
			else if (dynamic_cast<SuspensionBridge*>(e->obj))
			{

				SuspensionBridge* bridge = dynamic_cast<SuspensionBridge*>(e->obj);
				if (bridge->GetState() != BRIDGE_STATE_MOVING && !bridge->GetIsOpening())
				{
					bridge->SetState(BRIDGE_STATE_MOVING);
					isOnBridge = true;
					//DebugOut(L"[INFO] Vô đây hoài: \n");
				}
				/*this->x += bridge->dt * BRIDGE_MOVING_SPEED;*/

			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CThunder*>(e->obj))
			{
				CThunder *thunder = dynamic_cast<CThunder *>(e->obj);
				SetState(GIMMICK_STATE_DIE);
			}

		}
	}
	// di chuyển theo cầu
	if (isOnBridge && !jump) this->x += this->dt * BRIDGE_MOVING_SPEED;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGimmick::Render()
{
	int ani = -1;
	if (jump == 1)
	{
		if (nx > 0)
			ani = GIMMICK_ANI_JUMPING_RIGHT;
		else
			ani = GIMMICK_ANI_JUMPING_LEFT;
	}
	else if (state == GIMMICK_STATE_WALKING_RIGHT)
	{
		ani = GIMMICK_ANI_WALKING_RIGHT;

	}
	else if (state == GIMMICK_STATE_WALKING_LEFT)
	{
		ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else //if (state == GIMMICK_STATE_IDLE)
	{
		if (nx > 0)
		{
			ani = GIMMICK_ANI_IDLE_RIGHT;
		}
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();

}

void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		vx = -GIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = GIMMICK_JUMP_SPEED_Y;
		break; 
	case GIMMICK_STATE_IDLE:
		vx = 0;
		break;
	case GIMMICK_STATE_DIE:
		vy = GIMMICK_DIE_DEFLECT_SPEED;
		break;
	case GIMMICK_STATE_JUMP_HIGH_SPEED:
		vy = GIMMICK_JUMP_HIGHT_SPEED_Y;
		break;
	}
}

void CGimmick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	right = x + GIMMICK_BIG_BBOX_WIDTH;
	bottom = y - GIMMICK_BIG_BBOX_HEIGHT;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CGimmick::Reset()
{
	SetState(GIMMICK_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}
void CGimmick::Fire()
{
	// call star 
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects();
	for (UINT i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Star*>(objects[i]))
		{
			Star* star = dynamic_cast<Star*>(objects[i]);
			if (star->GetIsUsed() == false)
			{
				// get fired
				
				star->SetPosition(this->x, this->y + 30);
				star->nx = this->nx;
				star->SetState(STAR_STATE_FLYING);
				return;
			}
		}
		
	}
}
