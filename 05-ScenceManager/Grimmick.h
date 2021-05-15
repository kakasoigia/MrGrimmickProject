#pragma once
#include "GameObject.h"

#define GRIMMICK_WALKING_SPEED		0.15f 

#define GRIMMICK_JUMP_SPEED_Y		0.5f
#define GRIMMICK_JUMP_DEFLECT_SPEED 0.2f
#define GRIMMICK_GRAVITY			0.002f
#define GRIMMICK_DIE_DEFLECT_SPEED	 0.5f

#define GRIMMICK_STATE_IDLE			0
#define GRIMMICK_STATE_WALKING_RIGHT	100
#define GRIMMICK_STATE_WALKING_LEFT	200
#define GRIMMICK_STATE_JUMP			300
#define GRIMMICK_STATE_DIE				400

#define GRIMMICK_ANI_BIG_IDLE_RIGHT		1
#define GRIMMICK_ANI_BIG_IDLE_LEFT			0


#define GRIMMICK_ANI_BIG_WALKING_RIGHT			3
#define GRIMMICK_ANI_BIG_WALKING_LEFT			2




#define GRIMMICK_BIG_BBOX_WIDTH  16
#define GRIMMICK_BIG_BBOX_HEIGHT 19



#define GRIMMICK_UNTOUCHABLE_TIME 5000


class CGrimmick : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CGrimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};