
#include "Gimmick.h"
#include "Window.h"
#include "PlayScence.h"
Window::Window()
{
	SetState(WINDOW_STATE_CLOSE);
	nx = 1;
}

void Window::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WINDOW_BBOX_WIDTH;
	bottom = y - WINDOW_BBOX_HEIGHT;
}

void Window::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	
	if (state == WINDOW_STATE_CLOSE)
	{
		
		CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		
		if (abs(gimmick->x - this->x) < 20 && abs(gimmick->y - this->y) < 500)
		{

			
			this->SetState(WINDOW_STATE_OPEN);
			CallBoom();
		}
			
	}

	//vector<LPCOLLISIONEVENT> coEvents;
	//vector<LPCOLLISIONEVENT> coEventsResult;

	//coEvents.clear();


	//for (UINT i = 0; i < coObjects->size(); i++)
	//{
	//	LPGAMEOBJECT obj = coObjects->at(i);

	//}
	//CalcPotentialCollisions(coObjects, coEvents);
	//// No collision occured, proceed normally
	//if (coEvents.size() == 0)
	//{

	//	x += dx;
	//	y += dy;

	//}
	//else
	//{

	//	// land ...fly
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	x += min_tx * dx + nx * 0.4f;
	//	y += min_ty * dy + ny * 0.4f;

	//	/*if (nx!=0) vx = 0;*/
	//	if (ny != 0) vy = 0;

	//	// Collision logic with other objects
	//	//
	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		if (e->nx != 0 && ny == 0)
	//		{
	//			this->vx = -this->vx;
	//			this->nx = -this->nx;

	//		}
	//	}
	//}

	//// clean up collision events
	//for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Window::Render()
{
	
	int ani = WINDOW_ANI_CLOSE;
	if (state == WINDOW_STATE_CLOSE)
	{

		ani = WINDOW_ANI_CLOSE;
	}
	else if (state == WINDOW_STATE_OPEN)
	{
		ani = WINDOW_ANI_OPEN;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Window::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WINDOW_STATE_CLOSE:

		break;
	case WINDOW_STATE_OPEN:
		break;
	}
}
void Window:: CallBoom()
{
	
	vector<LPGAMEOBJECT> objects = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->get_objects();
	for (UINT i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<ElectricBoom*>(objects[i]))
		{
			ElectricBoom* boom = dynamic_cast<ElectricBoom*>(objects[i]);
			if (boom->GetState() == ELECTRICBOOM_STATE_DISAPPEAR)
			{
				// call blackenemy

				boom->SetPosition(this->x + WINDOW_BBOX_WIDTH/2 +5, this->y - WINDOW_BBOX_HEIGHT/2 - 5);
				boom->nx = 1;
				boom->SetState(ELECTRICBOOM_STATE_WALKING);
				return;
			}
		}

	}
}
