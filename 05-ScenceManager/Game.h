#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "GameObject.h"


#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "Scence.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	int screen_width;
	int screen_height; 

	unordered_map<int, LPSCENE> scenes;
	int current_scene; 

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);
	
	long score = 000;
	int rest = 4;
	int light = 1;
	vector<int> item ;
public:
	long GetScore() { return score; };
	int GetRest() { return rest; };
	int GetLight() { return light; };
	vector<int> GetItem() { return item; };

	void SetScore(int score) { this->score = score; };
	void SetLight(int light) { this->light = light; };
	void SetRest(int score) { this->rest = rest; };
	void SetItem(vector<int> item) { this->item = item; };

	void IncScore(int score) 
	{
		this->score += score;
		if (score > 99999999) score = 99999999;
	};
	void IncLight(int elight) 
	{
		this->light += elight; if (this->light > 4) this->light = 4;
	};
	void IncRest(int score) 
	{ 
		this->rest += rest;
		if (rest > 99) rest = 99;
	};

	void InitKeyboard();
	bool ObjectInCamera(CGameObject* object);
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x, float y) { cam_x = x; cam_y = y; }

	static CGame * GetInstance();

	~CGame();
};


