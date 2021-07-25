#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Gimmick.h"
#include "PlayScence.h"

#include "Goomba.h"
#include "Portal.h"
#include "BlackEnemy.h"
#include "Rocket.h"
#include "SuspensionBridge.h"
#include "Star.h"
#include "Incline.h"
#include "MovingBrick.h"
#include "Cannon.h"
#include "ElectricBoom.h"
#include "GimmickDieEffect.h"
#include "Pipe.h"

void CGimmick::FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy)
{

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	bool check_rec = false;
	bool check_brickbroken = false;

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}


			/*if (dynamic_cast<ElectricBoom*>(c->obj))
			{
				if (c->ny < 0)
					ny = 0;
			}
			if (dynamic_cast<BlackEnemy*>(c->obj))
			{
				if (c->ny < 0)
					ny = 0;
			}*/
			if (dynamic_cast<Rocket*>(c->obj))
			{
				
					ny = 0;
			}


		/*	if (dynamic_cast<Star*>(c->obj))
			{
				ny = 0.001f;
			}*/

	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}
void CGimmick::CalcPotentialCollisions(
	vector<LPGAMEOBJECT>* coObjects,
	vector<LPCOLLISIONEVENT>& coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<Star*>(coObjects->at(i)))
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

CGimmick::CGimmick(float x, float y) : CGameObject()
{
	CGame* game = CGame::GetInstance();

	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);
	if (game->isSwitchScene)
	{
		this->x = game->playerX;
		this->y = game->playerY;
		game->isSwitchScene = false;
	}
	else
	{
		this->x = x;
		this->y = y;
		game->playerX = x;
		game->playerY = y;
	}



}
void CGimmick::FollowObject(LPGAMEOBJECT obj)
{
	vx = obj->GetVx();

	if (!dynamic_cast<SuspensionBridge*>(obj))
	{
		y = obj->GetY() + GIMMICK_BIG_BBOX_HEIGHT + 0.4f;
	}
	
}


void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetState() != GIMMICK_STATE_IDLE) {

		isFollow = false;
	}

	if (isFollow)
	{

		if (dynamic_cast<SuspensionBridge*>(obj))
		{
			SuspensionBridge* bridge = dynamic_cast<SuspensionBridge*>(obj);
			if (bridge->isOpening)
			{
				isFollow = false;
				isOnBridge = false;
			}
			else
			{
				FollowObject(obj);
			}
		}
		else
		{
			FollowObject(obj);
		}
	}
	else {
		obj = NULL;
	}
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if ( !isIncline && state != GIMMICK_STATE_DIE && !isPiping && !isFollow )
		vy -= GIMMICK_GRAVITY * dt;

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
	if (GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
	// set sẵn - false 

	// No collision occured, proceed normally
	if (coEvents.size() == 0 && !isFollow)
	{

		
			x += dx;
			y += dy;
		

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
			if (dynamic_cast<BlackEnemy*>(e->obj))
			{
				BlackEnemy* be = dynamic_cast<BlackEnemy*>(e->obj);
				if (e->t > 0 && e->t <= 1)
				{
					if (e->ny > 0) 
					{
					
						isFollow = true;
						obj = be;
					}
					if (e->nx != 0)
					{
						callDeclineLight();
					}
				}
			}
			if (dynamic_cast<Item*>(e->obj))
			{

				Item* item = dynamic_cast<Item*>(e->obj);
				GetItem(item->GetType());
				item->SetState(ITEM_STATE_DISAPPEAR);

			}
			
			else if (dynamic_cast<Rocket*>(e->obj))
			{
				callDeclineLight();
			}
			if (dynamic_cast<SuspensionBridge*>(e->obj))
			{
				SuspensionBridge* bridge = dynamic_cast<SuspensionBridge*>(e->obj);
				if (!bridge->isOpening)
				{
					isOnBridge = true;
					isFollow = true;
					obj = bridge;
				}
				if (bridge->GetState() != BRIDGE_STATE_MOVING && !bridge->GetIsOpening())
				{
					
					bridge->SetState(BRIDGE_STATE_MOVING);

					//DebugOut(L"[INFO] Vô đây hoài: \n");
				}
				else
				{
					
					isOnBridge = false;
					
				}
			
			}
		
			if (dynamic_cast<CPipes*>(e->obj)) {

				CPipes* pipe = dynamic_cast<CPipes*>(e->obj);
				isPiping = true;
				x = pipe->x;
				if (vy > 0) y += 0.1f;
				else if (vy < 0)y -= 0.1f;
			}
			else {
				isPiping = false;
			}
			if (dynamic_cast<CMovingBrick*>(e->obj)) {

				CMovingBrick* mb = dynamic_cast<CMovingBrick*>(e->obj);

				if (e->t > 0 && e->t <= 1)

					if (e->ny > 0) {

						isFollow = true;
						obj = mb;
					}

			}
			if (dynamic_cast<ElectricBoom*>(e->obj)) 
			{

				
				if (e->t > 0 && e->t <= 1)
				{
						callDeclineLight();
				}

			}
			if (dynamic_cast<CThunder*>(e->obj))
			{
				CThunder* thunder = dynamic_cast<CThunder*>(e->obj);
				this->SetState(GIMMICK_STATE_DIE);
			}
			else if (dynamic_cast<Bullet*>(e->obj)) {
				callDeclineLight();
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
			if (dynamic_cast<CPortal*>(e->obj))
			{
				if (dynamic_cast<CPortal*>(e->obj)) {
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->playerX = p->getOldX();
					CGame::GetInstance()->playerY = p->getOldY();
					CGame::GetInstance()->isSwitchScene = true;
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
					return;
				}
			}

			if (dynamic_cast<Cannon*>(e->obj))
			{
				Cannon* cannon = dynamic_cast<Cannon*>(e->obj);
				cannon->x += dx;
				x += dx;
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
		if (!isIncline && !isPiping && !isSlide) 
		{
			if (!isHitRocket)
			{
				x += min_tx * dx + nx * 0.4f;
				y += min_ty * dy + ny * 0.4f;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			
		}
		else {
			x += dx;
			if (isIncline || isSlide) {
				y += min_ty * dy + ny * 0.4f;
			}
			else if (isPiping)
			{
				y += dy;
			}
		}
	}
	//// di chuyển theo cầu
	//if (isOnBridge && !jump) this->x += this->dt * BRIDGE_MOVING_SPEED;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CGimmick::Render()
{
	int ani = -1;
	if (untouchable == 1)
	{
		if (nx > 0)
			ani = GIMMICK_ANI_COLLISION_RIGHT;
		else
			ani = GIMMICK_ANI_COLLISION_LEFT;
	}
	else if (jump == 1)
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
	else if (state == GIMMICK_STATE_DIE)
	{
		createDieEffect();
		return;
	}
	else //if (state == GIMMICK_STATE_AUTO_GO)
	{
		if (key_down == 1)
			ani = GIMMICK_ANI_IDLE_RIGHT;
		else
			ani = GIMMICK_ANI_IDLE_LEFT;
	}
	int alpha = 255;

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
	CGame* game = CGame::GetInstance();
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
		this->isDeath = true;
		this->vx = 0;
		this->vy = 0;
		positionX = this->x;
		positionY = this->y;
		if (game->GetRest() > 0) game->IncRest(-1);
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

void CGimmick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == GIMMICK_STATE_DIE) {
		left = top = right = bottom = 0;
	}
	else {
		left = x;
		top = y;
		right = x + GIMMICK_BIG_BBOX_WIDTH;
		bottom = y - GIMMICK_BIG_BBOX_HEIGHT;
	}

}

/*
	Reset Mario status to the beginning state of a scene
*/
void CGimmick::Reset()
{
	CGame* game = CGame::GetInstance();
	SetState(GIMMICK_STATE_IDLE);
	SetPosition(game->playerX, game->playerY);
	SetSpeed(0, 0);
	game->SetLight(4);
}

//void CGimmick::Fire()
//{
//	// call star 
//	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects();
//	// - vấn đề : ném 5 con ..5 con có STT 12345
//		//- Vì mỗi con đk ném ra có vận tốc khác nhau PHỤ THUỘC vào STT 12345
//		// int count =0;
//	for (UINT i = 0; i < objects.size(); i++)
//	{
//		// if count == 5 return ; 
//		if (dynamic_cast<Star*>(objects[i]))
//		{
//			Star* star = dynamic_cast<Star*>(objects[i]);
//			//if (star->GetIsUsed() == false)
//			//{
//			//	// get fired
//			//	
//			//	star->SetPosition(this->x, this->y + 5);
//			//	star->nx = this->nx;
//			//	star->SetState(STAR_STATE_FLYING);
//			// xét cả V (theo count )  // chơi kiểu switch case ...case 1 -> set cứng vận tốc = a , .../ 2. chơi theo công thức
//			// count ++ ;
//			//	continue;
//			//}
//		}
//		
//	}
//}


void CGimmick::createDieEffect() {
	deltaTimeDie += 10;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	animation_sets->Get(80)->at(0)->Render(positionX, positionY + GIMMICKDIEEFFECT_SPEED * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED * deltaTimeDie, positionY);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX, positionY + -GIMMICKDIEEFFECT_SPEED * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED * deltaTimeDie, positionY);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_450 * deltaTimeDie);
	animation_sets->Get(80)->at(0)->Render(positionX + -GIMMICKDIEEFFECT_SPEED_675 * deltaTimeDie, positionY + GIMMICKDIEEFFECT_SPEED_225 * deltaTimeDie);
}

void CGimmick::callDeclineLight()
{
	if (untouchable == 0) {
		if (CGame::GetInstance()->GetLight() == 1)
		{
			this->SetState(GIMMICK_STATE_DIE);
			CGame::GetInstance()->IncLight(-1);
		}
		else
		{
			// stun???
			CGame::GetInstance()->IncLight(-1);
			StartUntouchable();
		}
	}
}


void CGimmick::GetItem(int type)
{
	CGame* game = CGame::GetInstance();
	vector <int> itemlist = game->GetItem();
	if (type == ITEM_TYPE_MEDICINE_PINK || type == ITEM_TYPE_MEDICINE_PINK_BOMB || type == ITEM_TYPE_MEDICINE_BLACK_BOMB)
	{
		//cộng điểm 
		game->IncScore(720);
		if (type == ITEM_TYPE_MEDICINE_PINK)
		{
			game->IncLight(1);
		}
		// neu khong co san trong itemlist thì thêm 
		for each (int item in itemlist)
		{
			if (item == type)
				return; // khoi them nua 
		}
		itemlist.push_back(type);
		game->SetItem(itemlist);
		DebugOut(L"[ERROR] Vô hud!\n");

	}
	else if (type == ITEM_TYPE_MEDICINE_ORANGE)
	{
		// tăng mạng
		game->IncLight(2);
		DebugOut(L"[ERROR] Vô tăng mạng!\n");
	}
	else if (type == ITEM_TYPE_FLOWER)
	{
		game->IncScore(50000);
		game->IncRest(2);
		DebugOut(L"[ERROR] Vô hoa!\n");
	}

}