#pragma once
#include "GameObject.h"

//#include "Rocket.h"
#define GIMMICK_JUMP_HIGHT_SPEED_Y 0.1
#define GIMMICK_WALKING_SPEED 0.08f 
#define GIMMICK_JUMP_SPEED_Y 0.4f
#define GIMMICK_DOUBLE_JUMP_SPEED 0.245f
#define GIMMICK_JUMP_DEFLECT_SPEED 0.2f
#define GIMMICK_GRAVITY 0.0008f
#define GIMMICK_DIE_DEFLECT_SPEED 0.25f

#define GIMMICK_WALKING_SPEED_SLIDE_TRUE		0.1f 
#define GIMMICK_WALKING_SPEED_SLIDE_FALSE		0.05f 

#define GIMMICK_GRAVITY			0.002f
#define GIMMICK_DIE_DEFLECT_SPEED	 0.5f
#define GIMMICK_INCLINE_UP_SPEED_X_1		0.05f
#define GIMMICK_INCLINE_UP_SPEED_X_2		0.075f
#define GIMMICK_INCLINE_DOWN_SPEED_X_1		0.125f
#define GIMMICK_INCLINE_DOWN_SPEED_X_2		0.1f

#define GIMMICK_INCLINE_UP_SPEED_Y_1		0.065
#define GIMMICK_INCLINE_UP_SPEED_Y_2		0.04f
#define GIMMICK_INCLINE_DOWN_SPEED_Y_1		0.125f
#define GIMMICK_INCLINE_DOWN_SPEED_Y_2		0.05f

#define GIMMICK_TREND_INCLINE_RIGHT	1
#define GIMMICK_TREND_INCLINE_LEFT	-1

#define GIMMICK_STATE_IDLE 0
#define GIMMICK_STATE_WALKING_RIGHT 100
#define GIMMICK_STATE_WALKING_LEFT 200
#define GIMMICK_STATE_JUMP 300
#define GIMMICK_STATE_DIE 400
#define GIMMICK_STATE_STUN 800
#define GIMMICK_STATE_JUMP_HIGH_SPEED	500
#define GIMMICK_STATE_INCLINE_DOWN	600
#define GIMMICK_STATE_INCLINE_UP		700

#define GIMMICK_STATE_AUTO_GO		1000

#define GIMMICK_ANI_IDLE_RIGHT 1
#define GIMMICK_ANI_IDLE_LEFT 0
#define GIMMICK_ANI_WALKING_LEFT 2
#define GIMMICK_ANI_WALKING_RIGHT 3
#define GIMMICK_ANI_JUMPING_RIGHT 4
#define GIMMICK_ANI_JUMPING_LEFT 5
#define GIMMICK_ANI_COLLISION_RIGHT 6
#define GIMMICK_ANI_COLLISION_LEFT 7


#define GIMMICK_BIG_BBOX_WIDTH  16
#define GIMMICK_BIG_BBOX_HEIGHT 19

#define GIMMICK_STATE_AUTO_GO_SLIDE_RIGHT		1001
#define GIMMICK_STATE_AUTO_GO_SLIDE_LEFT		1002

#define GIMMICK_UNTOUCHABLE_TIME 2000


// die effect
#define GIMMICKDIEEFFECT_SPEED 0.1f
#define GIMMICKDIEEFFECT_SPEED_225 0.0924f
#define GIMMICKDIEEFFECT_SPEED_450 0.0707f
#define GIMMICKDIEEFFECT_SPEED_675 0.0383f

#define GIMMICKDIEEFFECT_BBOX_WIDTH		16
#define GIMMICKDIEEFFECT_BBOX_HEIGHT	16

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
	bool isHitRocket = false;
public:
	// pipe
	bool isPiping = false;		// checking gimmick is piping
	float pipeVx, pipeVy;		// set speed when piping

	bool isFollow = false;
	CGameObject* obj = NULL;
	//Incline
	bool isIncline = false;				// check for sliding
	int incline_size;						// size of slide
	int direct_incline;
	int direct_go;						// trend of slide
	int key_down = 0;



	// silde
	bool isSlide = false;
	int isDirSlide = 0;
	int slideType = 0;
	float incline_vx, incline_vy;

	//set gimmick die
	float positionX, positionY;
	float deltaTimeDie = 0;
	bool isDeath = false;

	CGimmick(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx, float& min_ty,
		float& nx, float& ny, float& rdx, float& rdy);
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void CalcPotentialCollisions(
		vector<LPGAMEOBJECT>* coObjects,
		vector<LPCOLLISIONEVENT>& coEvents);
	void KeyState(BYTE* state);

	void Reset();
	int Jumping() { return jump; };
	void SetJumping(int jump) { this->jump = jump; };
	int GetJumping() { return jump; };
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	DWORD time_maxjumping = 0;
	DWORD time_stun = 0;
	DWORD doubleJump_start = 0; // Check for double jump
	void StartJumpingMax() { maxjumping = 1, time_maxjumping = GetTickCount(); }
	void ResetDoubleJumpStart() { doubleJump_start = 0; }
	void SetDoubleJumpStart() { doubleJump_start = GetTickCount(); }

	DWORD GetDoubleJumpStart() { return doubleJump_start; }
	void FollowObject(LPGAMEOBJECT obj);

	void SetOnTopBlackEnemy(bool isOnTopBlackEnemy) { this->isOnTopBlackEnemy = isOnTopBlackEnemy; };
	bool GetOnTopBlackEnemy() { return isOnTopBlackEnemy; };
	void SetOnBridge(bool isOnBridge) { this->isOnBridge = isOnBridge; };
	bool GetOnBridge() { return isOnBridge; };
	void Fire();
	void GetItem(int type);
	void createDieEffect();
	void callDeclineLight();
};