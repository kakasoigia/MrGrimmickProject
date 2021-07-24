#include "Pipe.h"

CPipes::CPipes(float _x, float _y, int _width, int _height)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
}

CPipes::~CPipes()
{
}

void CPipes::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + width;
	bottom = y - height;
}

void CPipes::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CPipes::Render()
{
}
