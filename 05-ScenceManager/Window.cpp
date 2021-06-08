
#include "Window.h"
Window::Window()
{
	SetState(WINDOW_STATE_CLOSE);
	nx = 1;
}

void Window::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

void Window::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	// Simple fall down
	

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

			if (e->nx != 0 && ny == 0)
			{
				this->vx = -this->vx;
				this->nx = -this->nx;

			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
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
