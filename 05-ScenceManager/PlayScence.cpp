
#include <fstream>

#include "PlayScence.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP				7

#define OBJECT_TYPE_GRIMMICK	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_WORM	2
#define OBJECT_TYPE_ELECTRICBOOM	3
#define OBJECT_TYPE_BLACKENEMY 4
#define OBJECT_TYPE_BOOMBOSS 5

#define OBJECT_TYPE_CANNON 7
#define OBJECT_TYPE_WINDOW 8
#define OBJECT_TYPE_SUSPENSIONBRIDGE 9
#define OBJECT_TYPE_ROCKET 10
#define OBJECT_TYPE_FLOATBOX 11

#define	OBJECT_TYPE_MEDICINE_PINK 12
#define	OBJECT_TYPE_MEDICINE_ORANGE 13
#define	OBJECT_TYPE_MEDICINE_PINK_BOMB 14
#define	OBJECT_TYPE_MEDICINE_BLACK_BOMB 15
#define OBJECT_TYPE_FLOWER 16
#define OBJECT_TYPE_MOVING_BRICK 17

#define OBJECT_TYPE_BULLET 20

#define OBJECT_TYPE_INCLINE	22

#define OBJECT_TYPE_GRIMMICK_DIE 23


#define OBJECT_TYPE_NOCOLLISIONOBJECT 49
#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_SLIDE_RIGHT 51
#define OBJECT_TYPE_SLIDE_LEFT	52

#define OBJECT_TYPE_FISH_RED 60
#define OBJECT_TYPE_FISH_BLACK	61
#define OBJECT_TYPE_FISH_YELLOW	62
#define OBJECT_TYPE_THUNDER 63
#define OBJECT_TYPE_STAR	21



#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());
		
		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_GRIMMICK:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CGimmick(x,y);
		player = (CGimmick*)obj;  

		DebugOut(L"[INFO] Player object created!\n");
		break;
	
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_BLACKENEMY: obj = new BlackEnemy(); break;
	case OBJECT_TYPE_WORM: obj = new Worm(); break;
	case OBJECT_TYPE_ELECTRICBOOM: obj = new ElectricBoom(); break;
	case OBJECT_TYPE_BOOMBOSS: obj = new BoomBoss(); break;
	case OBJECT_TYPE_SLIDE_LEFT: obj = new Slide(SLIDE_TYPE_LEFT); break;
	case OBJECT_TYPE_SLIDE_RIGHT: obj = new Slide(SLIDE_TYPE_RIGHT); break;
	case OBJECT_TYPE_CANNON: obj = new Cannon(); break;
	case OBJECT_TYPE_WINDOW: obj = new Window(); break;
	case OBJECT_TYPE_SUSPENSIONBRIDGE: obj = new SuspensionBridge(); break;
	case OBJECT_TYPE_ROCKET: obj = new Rocket(); break;
	case OBJECT_TYPE_FLOATBOX: obj = new FloatBox(); break;
	case OBJECT_TYPE_MEDICINE_PINK: obj = new Item(ITEM_TYPE_MEDICINE_PINK); break;
	case OBJECT_TYPE_MEDICINE_ORANGE: obj = new Item(ITEM_TYPE_MEDICINE_ORANGE); break;
	case OBJECT_TYPE_MEDICINE_PINK_BOMB: obj = new Item(ITEM_TYPE_MEDICINE_PINK_BOMB); break;
	case OBJECT_TYPE_MEDICINE_BLACK_BOMB: obj = new Item(ITEM_TYPE_MEDICINE_BLACK_BOMB); break;
	case OBJECT_TYPE_FLOWER: obj = new Item(ITEM_TYPE_FLOWER); break;
	case OBJECT_TYPE_MOVING_BRICK:
	{
		int min = atof(tokens[4].c_str());
		int max = atof(tokens[5].c_str());
		int type = atof(tokens[6].c_str());
		obj = new CMovingBrick(min, max, type);
		break;
	}
	case OBJECT_TYPE_GRIMMICK_DIE: 
		if (dieEffect != NULL)
		{
			DebugOut(L"[ERROR] Have die already man!\n");
			return;
		}
		obj = new CGimmickDieEffect();
		this->dieEffect = (CGimmickDieEffect*)obj;
		break;


	case OBJECT_TYPE_STAR: 
		if (star != NULL)
		{
			DebugOut(L"[ERROR] Have star already man!\n");
			return;
		}
		obj = new Star();
		this->star = (Star*)obj;
		; break;
	case OBJECT_TYPE_FISH_RED: obj = new Fish(FISH_TYPE_RED); break;
	case OBJECT_TYPE_FISH_BLACK: obj = new Fish(FISH_TYPE_BLACK); break;
	case OBJECT_TYPE_FISH_YELLOW: obj = new Fish(FISH_TYPE_YELLOW); break;
	case OBJECT_TYPE_BULLET: obj = new Bullet(); break;
	case OBJECT_TYPE_NOCOLLISIONOBJECT: obj = new NoCollisionObject(); break;

	case OBJECT_TYPE_INCLINE:
	{
		float d = atof(tokens[4].c_str());
		float s = atof(tokens[5].c_str());

		obj = new Incline(x, y, d, s);
		break;
	}

	case OBJECT_TYPE_PORTAL:
		{	
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
	case OBJECT_TYPE_THUNDER: obj = new CThunder(); break;

	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (ids == 1)
	{
		quadtree = new Quadtree(1, 0.0f, 384.0f, 1024.0f, 0.0f);
	}
	else if (ids == 2)
	{
		quadtree = new Quadtree(1, 0.0f, 760.0f, 1536.0f, 0.0f);
	}
	else if (ids == 3)
	{
		quadtree = new Quadtree(1, 0.0f, 192.0f, 1024.0f, 0.0f);
	}
}

void CPlayScene::Update(DWORD dt)
{
	//code 
	for (size_t i = 0; i < objects.size(); i++) {
		if (dynamic_cast<CGimmick*>(objects[i]))
			continue;
		if (dynamic_cast<CPortal*>(objects[i]))
			continue;
		quadtree->Insert(objects[i]);
	}

	// Update player
	vector<LPGAMEOBJECT> coObjects;

		


	quadtree->Retrieve(&coObjects, player);
	player->Update(dt, &coObjects);

	if (player == NULL) return;
	// update star
	if (star->state == STAR_STATE_READY_TO_SHOT || star->state == STAR_STATE_LOADING)
	{
		star->SetPosition(player->x -2, player->y + 16);
	}
	// update fish
	vector<LPGAMEOBJECT> temp_coObjects;
	quadtree->Retrieve(&temp_coObjects, star);
	star->Update(dt, &temp_coObjects);

	// Duyệt các object cần update 
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!CGame::GetInstance()->ObjectInCamera(objects[i]))
			continue;
		if (dynamic_cast<CGimmick*>(objects[i]))
			continue;
		if (dynamic_cast<Star*>(objects[i]))
			continue;
		
		vector<LPGAMEOBJECT> coObjectsUpdate;
		quadtree->Retrieve(&coObjectsUpdate, objects[i]);
		objects[i]->Update(dt, &coObjectsUpdate);

		/*}*/
	}
	// Làm trống quadtree
	if (quadtree)
		quadtree->Clear();
	CGame* game = CGame::GetInstance();
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	// Update camera to follow mario
	SetCamPos();
	// update hud
	hud->Update(dt);

}
void CPlayScene::SetCamPos() {

	int ids = CGame::GetInstance()->GetCurrentScene()->GetId();

	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	/*cx -= game->GetScreenWidth() / 2;
	cy += game->GetScreenHeight() / 2;*/


	switch (ids)
	{
	case 1:
	{
		if (cx < game->GetScreenWidth() / 2)
		{
			cx = 0;
		}
		else
		{
			if (cx > 1024 - (game->GetScreenWidth() / 2))
			{
				cx = 759;
			}
			else
			{
				cx -= game->GetScreenWidth() / 2;

			}
		}
		if (cy > 193)
		{
			cy = 384;
		}
		else
		{
			cy = 192;
		}
		break;
	}
	case 2:
	{
		if ((cx < game->GetScreenWidth() / 2))
		{
			cx = 0;
		}
		else
		{
			if (cx > 512 - (game->GetScreenWidth() / 2) && cy > 172)
			{
				cx = 247;
			}
			else
			{
				cx -= game->GetScreenWidth() / 2;
			}
		}
		if (cy > 193)
		{
			cy = 384;
		}
		else
		{
			cy = 192;
		}
		/*cx -= game->GetScreenWidth() / 2;
		cy += game->GetScreenHeight() / 2;*/
		break;
	}
	case 3:
	{
		if (cx < game->GetScreenWidth() / 2)
		{
			cx = 0;
		}
		else
		{
			if (cx > 1024 - (game->GetScreenWidth() / 2))
			{
				cx = 759;
			}
			else
			{
				cx -= game->GetScreenWidth() / 2;
			}
		}
		cy = 192;
		break;
	}
	default:
		break;
	}
	// Update camera to follow mario
	CGame::GetInstance()->SetCamPos((int)cx, (int)cy);

}

void CPlayScene::Render()
{
	if (map)
	{
		this->map->Render();
	}
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	hud->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	if (quadtree) {
		delete quadtree;
		quadtree = nullptr;
	}

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CGimmick *gimmick = ((CPlayScene*)scence)->GetPlayer();
	Star* star = ((CPlayScene*)scence)->GetStar();
	if (gimmick->GetState() == GIMMICK_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_S:
		if (gimmick->GetJumping() == 0)
		{
			gimmick->SetState(GIMMICK_STATE_JUMP);
			gimmick->SetJumping(1);

			if (gimmick->GetDoubleJumpStart() == 0)
			{
				//gimmick->SetState(MARIO_STATE_JUMP_HIGH_SPEED);
				gimmick->SetDoubleJumpStart();

			}
		}
		break;
	case DIK_M: 
		gimmick->Reset();
		gimmick->deltaTimeDie = 0;
		break;
	case DIK_V:
		if (star != nullptr) {
			star->GetReady();
		}
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();
	Star* star = ((CPlayScene*)scence)->GetStar();
	float x, y;
	gimmick->GetPosition(x, y);
	switch (KeyCode)
	{
	case DIK_S:
		gimmick->ResetDoubleJumpStart();
		break;
	case DIK_V:
		star->Shot();

	}

}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	//CGame *game = CGame::GetInstance();
	//CGimmick* grimmick = ((CPlayScene*)scence)->GetPlayer();

	//// disable control key when Mario die 
	//if (grimmick->GetState() == GIMMICK_STATE_DIE) return;
	//if (game->IsKeyDown(DIK_RIGHT))
	//	grimmick->SetState(GIMMICK_STATE_WALKING_RIGHT);
	//else if (game->IsKeyDown(DIK_LEFT))
	//	grimmick->SetState(GIMMICK_STATE_WALKING_LEFT);
	//else 
	//	grimmick->SetState(GIMMICK_STATE_IDLE);
	/*if (game->IsKeyDown(DIK_SPACE))
	{
		grimmick->SetState(GRIMMICK_STATE_HOLD_JUMP);
	}*/
	CGame* game = CGame::GetInstance();

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();

	gimmick->KeyState(states);

}