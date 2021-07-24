#pragma once

#include "GameObject.h"

/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	int width;
	int height;

	int oLeft = 0;
	int oRight = 0;

	float oldX = 0;
	float oldY = 0;
public:
	CPortal(float l, float t, float r, float b, int scene_id, float ox, float oy);
	CPortal(int scene_id);
	CPortal(float l, float t, int scene_id);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	int GetSceneId() { return scene_id; }
	float getOldX() { return oldX; }
	float getOldY() { return oldY; }
};