#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "Sprites.h"
#include "MovingBrick.h"
#include "Brick.h"
#include "Gimmick.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "BlackEnemy.h"
#include "BoomBoss.h"
#include "Electricboom.h"
#include "Worm.h"
#include "Slide.h"
#include "Cannon.h"
#include "Window.h"
#include "SuspensionBridge.h"
#include "Rocket.h"
#include "FloatBox.h"
#include "Item.h"
#include "Bullet.h"
#include "NoCollisionObject.h"
#include "Fish.h"
#include "Star.h"
#include "Thunder.h"
#include "Quadtree.h"
#include "Star.h"
#include "Hud.h"
#include "Incline.h"
#include "Utils.h"
#include "Pipe.h"
#include "AniBrick.h"
#include "Portal.h"

#include "GimmickDieEffect.h"
class CPlayScene: public CScene
{
protected: 
	Map* map;
	CGimmick* player;					// A play scene has to have player, right? 
	Star* star = nullptr;
	CGimmickDieEffect* dieEffect = nullptr;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsNoColliMove;

	CHud* hud = new CHud();
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void SetCamPos();
	
public: 
	Quadtree* quadtree;
	CPlayScene(int id, LPCWSTR filePath);


	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	vector<LPGAMEOBJECT> get_objects() { return objects; };


	CGimmick* GetPlayer() { return player; }
	Quadtree* GetQuadtree() { return quadtree; }
	void addObject(LPGAMEOBJECT a) { objects.push_back(a); }
	Star* GetStar() { return star; };
	CGimmickDieEffect* GetDieEffect() { return dieEffect; }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public: 
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) ;
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

