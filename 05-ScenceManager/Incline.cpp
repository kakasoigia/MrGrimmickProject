#include "Incline.h"

Incline::Incline(float x, float y, int _direct, int _size)
{
	size = _size;
	direct = _direct;
}

Incline::~Incline()
{
}

void Incline::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void Incline::Render()
{
}

void Incline::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	bottom = y - INCLINE_SIZE_QUAD;

	if (size == 1)
		right = x + INCLINE_SIZE_QUAD;
	else
		right = x + INCLINE_SIZE_REC;
}
