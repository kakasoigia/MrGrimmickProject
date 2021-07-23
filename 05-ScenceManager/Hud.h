#pragma once

#include "GameObject.h"
#include "Game.h"
#include "Utils.h"


// sprite number
#define HUD_SPRITE_PANEL	80710
#define	HUD_SPRITE_NUMBER_0	80740
#define	HUD_SPRITE_NUMBER_1	80741
#define	HUD_SPRITE_NUMBER_2	80742
#define	HUD_SPRITE_NUMBER_3	80743
#define	HUD_SPRITE_NUMBER_4	80744
#define	HUD_SPRITE_NUMBER_5	80745
#define	HUD_SPRITE_NUMBER_6	80746
#define	HUD_SPRITE_NUMBER_7	80747
#define	HUD_SPRITE_NUMBER_8	80748
#define	HUD_SPRITE_NUMBER_9	80749
#define	HUD_SPRITE_HP_0	80721
#define	HUD_SPRITE_HP_1	80722
#define	HUD_SPRITE_HP_2	80723
#define	HUD_SPRITE_HP_3	80724
#define	HUD_SPRITE_HP_4	80725
#define	HUD_SPRITE_ITEM_PINK_MEDICINE	80730
#define	HUD_SPRITE_ITEM_BOOM	80732
#define	HUD_SPRITE_ITEM_PINK_BALL	80731


class CHud : public CGameObject
{
	LPSPRITE hudpanel_sprite;


	vector<LPSPRITE> light_sprite_list;
	vector<LPSPRITE> item_sprite_list;
	vector<LPSPRITE> scoreSprite;

	long score = 000;
	int rest = 4;
	int light = 1;
	vector<int> item_list;

public:
	CHud();

	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void UpdateFigure();
	
	void RenderScore();

	void RenderRest();

	void RenderLight();

	void RenderItem();

};

