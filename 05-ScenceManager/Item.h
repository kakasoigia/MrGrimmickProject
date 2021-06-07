#pragma once
#include "GameObject.h"
#include "Gimmick.h"


#define ITEM_BBOX_WIDTH 16
#define ITEM_BBOX_HEIGHT 15

#define	ITEM_TYPE_BOTTLE_ORANGE 100
#define	ITEM_TYPE_BOTTLE_BLACK 200
#define	ITEM_TYPE_BOTTLE_RED 300
#define	ITEM_TYPE_BOTTLE_BIG_RED 400


#define ITEM_STATE_IDLING 100



#define ITEM_ANI_BOTTLE_ORANGE 0

#define	ITEM_ANI_BOTTLE_BLACK 1
#define	ITEM_ANI_BOTTLE_RED 2
#define	ITEM_ANI_BOTTLE_BIG_RED 3



class Item :
	public CGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	bool isAppear; 
	int Type;
	//virtual void FilterCollision(vector<LPCOLLISIONEVENT>& coEvents, vector<LPCOLLISIONEVENT>& coEventsResult, float& min_tx, float& min_ty, float& nx, float& ny, float& rdx, float& rdy);
public:
	Item();
	Item(int type);
	int GetType() { return Type; };
	void SetType(int type) { Type = type; };
	int GetAppear() { return isAppear; };
	void SetAppear(int isAppear) { isAppear = isAppear; };
	virtual void SetState(int state);
};

