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
#include "Incline.h"

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
	if(!isOnTopBlackEnemy && !isIncline)
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
		
		isIncline = false;
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
	
	/*	x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;*/

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
			if (dynamic_cast<BlackEnemy*>(e->obj)) // if e->obj is Goomba 
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
			if (dynamic_cast<Rocket*>(e->obj))
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
			if (dynamic_cast<SuspensionBridge*>(e->obj))
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
			if (dynamic_cast<CThunder*>(e->obj))
			{
				CThunder *thunder = dynamic_cast<CThunder *>(e->obj);
				SetState(GIMMICK_STATE_DIE);
			}
			if (dynamic_cast<Incline*>(e->obj)) {

				isIncline = true;

				float tran_y = -99999;

				Incline* incline = dynamic_cast<Incline*>(e->obj);

				if (incline->size == 1)
					incline_size = 1;
				else
					incline_size = 2;

				if (( /*GetState() != GIMMICK_STATE_JUMP &&*/ CGame::GetInstance()->IsKeyDown(DIK_RIGHT))
					|| GetState() == GIMMICK_STATE_WALKING_RIGHT) {
					direct_go = 1;
					if (incline->direct == 1) {
						direct_incline = 1;
					}
					else {
						direct_incline = -1;
					}
				}
				else if ((CGame::GetInstance()->IsKeyDown(DIK_LEFT))
					|| GetState() == GIMMICK_STATE_WALKING_LEFT) {
					direct_go = -1;
					if (incline->direct == 1) {
						direct_incline = 1;
					}
					else {
						direct_incline = -1;
					}
				}
				else {
					if (incline->direct == 1) {
						if (incline->size == 1) {
							incline_vx = -GIMMICK_INCLINE_DOWN_SPEED_X_1;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
						}
						else {
							incline_vx = -GIMMICK_INCLINE_DOWN_SPEED_X_2;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
						}
					}
					else {
						if (incline->size == 1) {
							incline_vx = GIMMICK_INCLINE_DOWN_SPEED_X_1;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
						}
						else {
							incline_vx = GIMMICK_INCLINE_DOWN_SPEED_X_2;
							incline_vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
						}
					}

				}

				//if (incline->direct == 1) {
				//	tran_y = isOnTriangle(incline->x, incline->y - 1, incline->x + incline->size, incline->y, incline->x + incline->size, incline->y - 1, x, y);
				//	if (tran_y <= 0)
				//		y += 0.2f;			// discount y to fall through slide
				//}
				//else {
				//	tran_y = isOnTriangle(incline->x, incline->y, incline->x, incline->y - 1, incline->x + incline->size, incline->y - 1, x, y);
				//	if (tran_y <= 0)
				//		y += 0.2f;			// discount y to fall through slide
				//}
			}
			else {
				isIncline = false;
			}
			if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
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
		if (!isIncline && !isSlide) {

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}
		else {
			x += dx;
			if (isIncline || isSlide) {
				y += min_ty * dy + ny * 0.4f;
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
	else if (state == GIMMICK_STATE_INCLINE_UP)
	{
		if (vx > 0)
			ani = GIMMICK_ANI_WALKING_RIGHT;
		else
			ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else if (state == GIMMICK_STATE_INCLINE_DOWN)
	{
		if (vx > 0)
			ani = GIMMICK_ANI_WALKING_RIGHT;
		else
			ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else if (state == GIMMICK_STATE_IDLE)
	{
		if (nx > 0)
		{
			ani = GIMMICK_ANI_IDLE_RIGHT;
		}
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	}
	else if (state == GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT)
	{
		ani = GIMMICK_ANI_WALKING_RIGHT;
	}
	else if (state == GIMMICK_STATE_AUTO_GO_SLIDE_LEFT)
	{
		ani = GIMMICK_ANI_WALKING_LEFT;
	}
	else //if (state == GIMMICK_STATE_AUTO_GO)
	{
		if (key_down == 1)
			ani = GIMMICK_ANI_IDLE_RIGHT;
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	}
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();

}

void CGimmick::KeyState(BYTE* state)
{
	CGame* game = CGame::GetInstance();

	// disable control key when Mario die 
	if (GetState() == GIMMICK_STATE_DIE) return;

	if (game->IsKeyDown(DIK_RIGHT)) {

		key_down = 1;

		if (!isIncline && !isSlide) {
			SetState(GIMMICK_STATE_WALKING_RIGHT);
		}
		else if (isIncline) {

			if (direct_incline == GIMMICK_TREND_INCLINE_RIGHT)

				SetState(GIMMICK_STATE_INCLINE_UP);

			else
				SetState(GIMMICK_STATE_INCLINE_DOWN);
		}
		else if (isSlide)
		{
			SetState(GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT)) {

		key_down = -1;

		if (!isIncline && !isSlide) {

			SetState(GIMMICK_STATE_WALKING_LEFT);
		}

		else if (isIncline) {

			if (direct_incline == GIMMICK_TREND_INCLINE_LEFT)

				SetState(GIMMICK_STATE_INCLINE_UP);

			else
				SetState(GIMMICK_STATE_INCLINE_DOWN);
		} if (isSlide)
		{
			SetState(GIMMICK_STATE_AUTO_GO_SLIDE_LEFT);
		}
	}
	else if (isIncline || isSlide) {
		//gimmick->key_down = 0;
		SetState(GIMMICK_STATE_AUTO_GO);
	}
	else if (vy == 0 /*&& gimmick->vx != 0*/) {

		//gimmick->key_down = 0;

		SetState(GIMMICK_STATE_IDLE);
	}



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
	case GIMMICK_STATE_INCLINE_UP:
	{
		if (direct_go == 1)
		{
			if (incline_size == 1) {
				vx = GIMMICK_INCLINE_UP_SPEED_X_1;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_1;
			}
			else {
				vx = GIMMICK_INCLINE_UP_SPEED_X_2;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_2;
			}
		}
		else //if (direct_go == -1)
		{
			if (incline_size == 1) {
				vx = -GIMMICK_INCLINE_UP_SPEED_X_1;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_1;
			}
			else {

				vx = -GIMMICK_INCLINE_UP_SPEED_X_2;
				vy = GIMMICK_INCLINE_UP_SPEED_Y_2;
			}
		}
	}
	break;

	case GIMMICK_STATE_INCLINE_DOWN:
	{
		if (direct_go == 1)
		{
			if (incline_size == 1) {

				vx = GIMMICK_INCLINE_DOWN_SPEED_X_1;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
			}
			else {
				vx = GIMMICK_INCLINE_DOWN_SPEED_X_2;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
			}
		}
		else if (direct_go == -1)
		{
			if (incline_size == 1) {

				vx = -GIMMICK_INCLINE_DOWN_SPEED_X_1;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_1;
			}
			else {

				vx = -GIMMICK_INCLINE_DOWN_SPEED_X_2;
				vy = -GIMMICK_INCLINE_DOWN_SPEED_Y_2;
			}
		}
	}
	break;
	case GIMMICK_STATE_AUTO_GO:
		if (isIncline) {
			vx = incline_vx;
			vy = incline_vy;
		}
		else if (isSlide)
		{
			if (slideType == 1)
			{
				vx = GIMMICK_WALKING_SPEED;
			}
			else
			{
				vx = -GIMMICK_WALKING_SPEED;
			}
		}
		break;
	case GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT:
		if (slideType == 1)
		{
			vx = GIMMICK_WALKING_SPEED_SLIDE_TRUE;
		}
		else
		{
			vx = GIMMICK_WALKING_SPEED_SLIDE_FALSE;
		}
		nx = 1;
		break;
	case GIMMICK_STATE_AUTO_GO_SLIDE_LEFT:
		if (slideType == -1)
		{
			vx = -GIMMICK_WALKING_SPEED_SLIDE_TRUE;
		}
		else
		{
			vx = -GIMMICK_WALKING_SPEED_SLIDE_FALSE;
		}
		nx = -1;
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
				
				star->SetPosition(this->x, this->y + 5);
				star->nx = this->nx;
				star->SetState(STAR_STATE_FLYING);
				return;
			}
		}
		
	}
}
