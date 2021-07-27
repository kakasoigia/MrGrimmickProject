#include "Boat.h"

CBoat::CBoat()
{
}

CBoat::~CBoat()
{
}

void CBoat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (moving)
    {
        vx = BOAT_SPEED_X;
    }
    CGameObject::Update(dt);
    x += dx;

    if (x > 1696)
    {
        x = 1696;
        vx = 0;
    }
}

void CBoat::Render()
{
    animation_set->at(0)->Render(x, y);
}

void CBoat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x;
    top = y;
    right = x + 36;
    bottom = y - 200;
}