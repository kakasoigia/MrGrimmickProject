#include "Portal.h"
#include "Utils.h"


CPortal::CPortal(float l, float t, float r, float b, int _scene_id, float ox, float oy)
{
	scene_id = _scene_id;
	x = l;
	y = t;
	width = r - l + 1;
	height = t - b;
	oldX = ox;
	oldY = oy;
}

CPortal::CPortal(int scene_id)
{
	this->scene_id = scene_id;
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

void CPortal::Render()
{
	RenderBoundingBox();
}

void CPortal::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y - height;
}