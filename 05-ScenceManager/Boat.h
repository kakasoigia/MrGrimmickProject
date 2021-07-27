#pragma once
#include "GameObject.h"

#define BOAT_SPEED_X    0.035f

class CBoat : public CGameObject
{
public:

    bool moving = false;


public:

    CBoat();
    ~CBoat();


    void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    void Render();
    void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};