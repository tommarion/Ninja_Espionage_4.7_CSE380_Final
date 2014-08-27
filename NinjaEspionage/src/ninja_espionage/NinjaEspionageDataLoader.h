/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaEspionageDataLoader.h

	This class provides a custom importer for the NinjaEspionageGame to import
	game data, gui data, and world (i.e. level) data.
*/
#pragma once
#include "ninja_espionage_VS\stdafx.h"
#include "sssf\data_loader\GameDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"
#include "sssf\gsm\ai\bots\NinjaStar.h"
#include "sssf\gsm\ai\bots\RandomObject.h"
#include "sssf\gsm\ai\bots\RandomGuard.h"
#include "sssf\gsm\ai\bots\BossBot.h"
#include "psti\PoseurSpriteTypesImporter.h"



class NinjaEspionageDataLoader : public GameDataLoader
{
	RandomObject *attack1 = new RandomObject(new Physics(), 0, 0, 0);
	RandomObject *attack2 = new RandomObject(new Physics(), 0, 0, 0);
	RandomObject *attack3 = new RandomObject(new Physics(), 0, 0, 0);
	RandomObject *attack4 = new RandomObject(new Physics(), 0, 0, 0);
	RandomObject *lives = new RandomObject(new Physics(), 0, 0, 0);
	RandomGuard *guards[500];
	

private:
	// WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
	HINSTANCE hInstance;
	int nCmdShow;
	

public:

	int temp = 0;
	int botCount = 0;
	NinjaEspionageDataLoader()	{}
	~NinjaEspionageDataLoader()	{}

	// INLINED MUTATOR METHOD
	void initWinHandle(HINSTANCE initHInstance, int initNCmdShow)
	{
		hInstance = initHInstance;
		nCmdShow = initNCmdShow;
	}


	// THESE SHOULD BE LOADING DATA FROM FILES. THEY
	// ARE ALL DEFINED IN NinjaEspionageDataLoader.cpp
	void loadGame(Game *game, wstring gameInitFile);
	void loadGUI(Game *game, wstring guiInitFile);
	void loadWorld(Game *game, wstring levelInitFile);
	void reloadWorld(Game *game, wstring levelInitFile);

	// THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
	void hardCodedLoadGUIExample(Game *game);

	// SOME HELPER METHODS FOR SETTING UP THE GAME
	void initColors(GameGraphics *graphics,	map<wstring,wstring> *properties);
	void initViewport(GameGUI *gui, map<wstring,wstring> *properties);
	void makeRandomGuard(Game *game, AnimatedSpriteType *RandomGuardType, float initX, float initY, int i);
	void makeNinjaStar(Game *game, AnimatedSpriteType *ninjaStar, float initX, float initY, float velocityX, float velocityY, bool spec);

	// AND SOME HELPER METHODS FOR LOADING THE GUI
	void initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager);
	void initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initGameWonGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initGameLossGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initAboutMenuGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initHelpMenuGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);

	void initGameFailGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager);
	void initGameIntroGUI(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager); /// NEED INTRO SCREEN

	void attack1Cool() { attack1->setCurrentState(L"COOLDOWN1"); }
	void attack2Cool() { attack2->setCurrentState(L"COOLDOWN2"); }
	void attack3Cool() { attack3->setCurrentState(L"COOLDOWN3"); }
	void attack4Cool() { attack4->setCurrentState(L"COOLDOWN4"); }
	void attack2Idle() { attack2->setCurrentState(L"IDLE2");     }
	void attack3Idle() { attack3->setCurrentState(L"IDLE3");	 }
	void attack4Idle() { attack4->setCurrentState(L"IDLE4");	}
	void lives3() { attack2->setCurrentState(L"3"); }
	void lives2() { attack2->setCurrentState(L"2"); }
	void lives1() { attack2->setCurrentState(L"1"); }

	// HELPER METHODS FOR LOADING BOT INFO
	void NinjaEspionageDataLoader::loadGuardInfo(wstring gameInitFile, AnimatedSpriteType *botSpriteType, Game *game);
	void NinjaEspionageDataLoader::loadLineInfo(wstring *line, AnimatedSpriteType *botSpriteType, Game *game, int i);
	unsigned int wstringToInt(wstring line);
	void createPlayer(SpriteManager *sm, Physics *physics, AnimatedSprite *player, int xVal, int yVal);
	void createGate(SpriteManager *sm, Physics *physics, int xVal, int yVal);
};