#include "Thunder.h"

void CThunder::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CThunder::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + THUNDER_BBOX_WIDTH;
	b = y - THUNDER_BBOX_HEIGHT;
}

