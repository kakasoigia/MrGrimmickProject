#include "Hud.h"
#include "Scence.h"
#include "PlayScence.h"
#include "Item.h"
 CHud::CHud()
{
	

}
void CHud::Update(DWORD dt)
{
	float cx, cy;
	
	CGame::GetInstance()->GetCamPos(cx, cy);
	SetPosition(cx, cy - CGame::GetInstance()->GetScreenHeight()+5);
	this->dt = dt;
	//update figure
	UpdateFigure();

}

void CHud::Render()
{
	CSprites* sprites = CSprites::GetInstance();
	CGame* game = CGame::GetInstance();

	hudpanel_sprite = sprites->Get(HUD_SPRITE_PANEL);
	
	//draw a  background
	hudpanel_sprite->Draw(x, y +25 );

	

	RenderScore();
	RenderRest();
	RenderLight();
	RenderItem();
}

void CHud::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = r = t = b = 0;
	
}

void CHud::RenderScore()
{
	CSprites* sprites = CSprites::GetInstance();
	int tmpScore = score;

	float x_right = this->x + 80;
	while (tmpScore > 0) 
	{
		int i = tmpScore % 10;
		tmpScore /= 10;
		
		switch (i)
		{
		case 0:
			sprites->Get(HUD_SPRITE_NUMBER_0)->Draw (x_right - 8, y + 9, 255);
			break;
		case 1:
			sprites->Get(HUD_SPRITE_NUMBER_1)->Draw(x_right - 8, y + 9, 255);
			break;
		case 2:
			sprites->Get(HUD_SPRITE_NUMBER_2)->Draw(x_right - 8, y + 9, 255);
			break;
		case 3:
			sprites->Get(HUD_SPRITE_NUMBER_3)->Draw(x_right - 8, y + 9, 255);
			break;
		case 4:
			sprites->Get(HUD_SPRITE_NUMBER_4)->Draw(x_right - 8, y + 9, 255);
			break;
		case 5:
			sprites->Get(HUD_SPRITE_NUMBER_5)->Draw(x_right - 8, y + 9, 255);
			break;
		case 6:
			sprites->Get(HUD_SPRITE_NUMBER_6)->Draw(x_right - 8, y + 9, 255);
			break;
		case 7:
			sprites->Get(HUD_SPRITE_NUMBER_7)->Draw(x_right - 8, y +9 , 255);
			break;
		case 8:
			sprites->Get(HUD_SPRITE_NUMBER_8)->Draw(x_right - 8, y + 9, 255);
			break;
		case 9:
			sprites->Get(HUD_SPRITE_NUMBER_9)->Draw(x_right - 8, y + 9, 255);
			break;
		}
		
		x_right -= 8;
	}


}

void CHud::RenderRest()
{

	CSprites* sprites = CSprites::GetInstance();
	int tmpRest = rest;

	float x_right = this->x  + 112;
	while (tmpRest > 0)
	{
		int i = tmpRest % 10;
		tmpRest /= 10;

		switch (i)
		{
		case 0:
			sprites->Get(HUD_SPRITE_NUMBER_0)->Draw(x_right - 8, y + 9, 255);
			break;
		case 1:
			sprites->Get(HUD_SPRITE_NUMBER_1)->Draw(x_right - 8, y + 9, 255);
			break;
		case 2:
			sprites->Get(HUD_SPRITE_NUMBER_2)->Draw(x_right - 8, y + 9, 255);
			break;
		case 3:
			sprites->Get(HUD_SPRITE_NUMBER_3)->Draw(x_right - 8, y + 9, 255);
			break;
		case 4:
			sprites->Get(HUD_SPRITE_NUMBER_4)->Draw(x_right - 8, y + 9, 255);
			break;
		case 5:
			sprites->Get(HUD_SPRITE_NUMBER_5)->Draw(x_right - 8, y + 9, 255);
			break;
		case 6:
			sprites->Get(HUD_SPRITE_NUMBER_6)->Draw(x_right - 8, y + 9, 255);
			break;
		case 7:
			sprites->Get(HUD_SPRITE_NUMBER_7)->Draw(x_right - 8, y + 9, 255);
			break;
		case 8:
			sprites->Get(HUD_SPRITE_NUMBER_8)->Draw(x_right - 8, y + 9, 255);
			break;
		case 9:
			sprites->Get(HUD_SPRITE_NUMBER_9)->Draw(x_right - 8, y + 9, 255);
			break;
		}

		x_right -= 8;
	}


}



void CHud::RenderLight()
{
	CSprites* sprites = CSprites::GetInstance();

;
	switch (light)
	{
	case 0:
		sprites->Get(HUD_SPRITE_HP_0)->Draw(x + 120, y + 17, 255);
		break;
	case 1:
		sprites->Get(HUD_SPRITE_HP_1)->Draw(x + 120, y + 17, 255);
		break;
	case 2:
		sprites->Get(HUD_SPRITE_HP_2)->Draw(x + 120, y + 17, 255);
		break;
	case 3:
		sprites->Get(HUD_SPRITE_HP_3)->Draw(x + 120, y + 17, 255);
		break;
	case 4:
		sprites->Get(HUD_SPRITE_HP_4)->Draw(x + 120, y + 17, 255);
		break;
	}

}

void CHud::RenderItem()
{

	CSprites* sprites = CSprites::GetInstance();
	int space = 0;
	for each ( int item in item_list)
	{
		
		switch (item)
		{
		case ITEM_TYPE_MEDICINE_PINK:
			sprites->Get(HUD_SPRITE_ITEM_PINK_MEDICINE)->Draw(x + 177 + 24*space, y+20 , 255);
			space++;
			break;
		case ITEM_TYPE_MEDICINE_PINK_BOMB:
			sprites->Get(HUD_SPRITE_ITEM_BOOM)->Draw(x + 177+ 24 * space, y+ 20 , 255);
			space++;
			break;
		case ITEM_TYPE_MEDICINE_BLACK_BOMB:
			sprites->Get(HUD_SPRITE_ITEM_PINK_BALL)->Draw(x + 177+ 24 * space, y + 20, 255);
			space++;
			break;
		}
	}

}
void CHud::UpdateFigure()
{

	score = CGame::GetInstance()->GetScore();
	rest = CGame::GetInstance()->GetRest();
	light = CGame::GetInstance()->GetLight();
	item_list = CGame::GetInstance()->GetItem();
}
