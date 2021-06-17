#pragma once
#include "GameObject.h"

//#include "Rocket.h"
#define GIMMICK_JUMP_HIGHT_SPEED_Y 0.1
#define GIMMICK_WALKING_SPEED 0.15f 
#define GIMMICK_JUMP_SPEED_Y 0.24f
#define GIMMICK_DOUBLE_JUMP_SPEED 0.245f
#define GIMMICK_JUMP_DEFLECT_SPEED 0.2f
#define GIMMICK_GRAVITY 0.0008f
#define GIMMICK_DIE_DEFLECT_SPEED 0.5f
#define GIMMICK_STATE_IDLE 0
#define GIMMICK_STATE_WALKING_RIGHT 100
#define GIMMICK_STATE_WALKING_LEFT 200
#define GIMMICK_STATE_JUMP 300
#define GIMMICK_STATE_DIE 400
#define GIMMICK_STATE_JUMP_HIGH_SPEED	500
#define GIMMICK_ANI_IDLE_RIGHT 1
#define GIMMICK_ANI_IDLE_LEFT 0
#define GIMMICK_ANI_WALKING_LEFT 2
#define GIMMICK_ANI_WALKING_RIGHT 3
#define GIMMICK_ANI_JUMPING_RIGHT 4
#define GIMMICK_ANI_JUMPING_LEFT 5




#define GIMMICK_BIG_BBOX_WIDTH  16
#define GIMMICK_BIG_BBOX_HEIGHT 19



#define GIMMICK_UNTOUCHABLE_TIME 5000


class CGimmick : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	int jump = 0;
	int maxjumping = 0;
	bool isMaxJumping = false;
	bool isOnTopBlackEnemy = false;
	bool isOnBridge = false;
public: 
	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();
	int Jumping() { return jump; };
	void SetJumping(int jump) { this->jump = jump; };
	int GetJumping() { return jump; };
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	DWORD time_maxjumping = 0;
	DWORD doubleJump_start = 0; // Check for double jump
	void StartJumpingMax() { maxjumping = 1, time_maxjumping = GetTickCount(); }
	void ResetDoubleJumpStart() { doubleJump_start = 0; }
	void SetDoubleJumpStart() { doubleJump_start = GetTickCount(); }

	DWORD GetDoubleJumpStart() { return doubleJump_start; }

	void SetOnTopBlackEnemy(bool isOnTopBlackEnemy) { this->isOnTopBlackEnemy = isOnTopBlackEnemy; };
	bool GetOnTopBlackEnemy() { return isOnTopBlackEnemy; };
	void SetOnBridge(bool isOnBridge) { this->isOnBridge = isOnBridge; };
	bool GetOnBridge() { return isOnBridge; };
};