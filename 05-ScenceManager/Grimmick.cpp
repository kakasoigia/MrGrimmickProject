#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Grimmick.h"
#include "Game.h"

#include "Goomba.h"
#include "Portal.h"

CGrimmick::CGrimmick(float x, float y) : CGameObject()
{
	untouchable = 0;
	SetState(GRIMMICK_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CGrimmick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += GRIMMICK_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!= GRIMMICK_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > GRIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
	{
		x += dx; 
		y += dy;
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


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -GRIMMICK_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
								SetState(GRIMMICK_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGrimmick::Render()
{
	int ani = -1;
	
		if (vx == 0)
		{
			if (nx>0) ani = GRIMMICK_ANI_BIG_IDLE_RIGHT;
			else ani = GRIMMICK_ANI_BIG_IDLE_LEFT;
		}
		else if (vx > 0) 
			ani = GRIMMICK_ANI_BIG_WALKING_RIGHT;
		else ani = GRIMMICK_ANI_BIG_WALKING_LEFT;
	

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CGrimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GRIMMICK_STATE_WALKING_RIGHT:
		vx = GRIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GRIMMICK_STATE_WALKING_LEFT:
		vx = -GRIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	case GRIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -GRIMMICK_JUMP_SPEED_Y;
		break; 
	case GRIMMICK_STATE_IDLE:
		vx = 0;
		break;
	case GRIMMICK_STATE_DIE:
		vy = -GRIMMICK_DIE_DEFLECT_SPEED;
		break;
	}
}

void CGrimmick::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y; 
	right = x + GRIMMICK_BIG_BBOX_WIDTH;
	bottom = y + GRIMMICK_BIG_BBOX_HEIGHT;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CGrimmick::Reset()
{
	SetState(GRIMMICK_STATE_IDLE);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

