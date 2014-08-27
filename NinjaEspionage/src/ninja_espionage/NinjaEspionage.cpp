/*
	Author: Kiwan Kwon
			Tom Marion
			Mark Jensen

			Stony Brook University
			Computer Science Department

	NinjaEspionageGame.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "ninja_espionage_VS\stdafx.h"

// NinjaEspionage GAME INCLUDES
#include "ninja_espionage\NinjaEspionageButtonEventHandler.h"
#include "ninja_espionage\NinjaEspionageCollisionListener.h"
#include "ninja_espionage\NinjaEspionageDataLoader.h"
#include "ninja_espionage\NinjaEspionage.h"
#include "ninja_espionage\NinjaEspionageKeyEventHandler.h"
#include "ninja_espionage\NinjaEspionageTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\gui\ScreenGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\os\GameOS.h"
#include "sssf\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "sssf\platforms\Windows\WindowsOS.h"
#include "sssf\platforms\Windows\WindowsInput.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf\platforms\DirectX\DirectXGraphics.h"
#include "sssf\platforms\DirectX\DirectXTextureManager.h"

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/

struct Nums
{
	int num1;
	int num2;
};
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE THE GAME
	Game *ninjaEspionageGame = new Game();

	// FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
	// IN DATA TO SETUP OTHER STUFF
	NinjaEspionageDataLoader *ninjaEspionageDataLoader = new NinjaEspionageDataLoader();
	ninjaEspionageDataLoader->initWinHandle(hInstance, nCmdShow);
	ninjaEspionageGame->setDataLoader(ninjaEspionageDataLoader);

	ninjaEspionageDataLoader->loadGame(ninjaEspionageGame, W_INIT_FILE);
	
	// WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
	// MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
	// FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

	// LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
	// BE DONE FROM A FILE, NOT HARD CODED
	ninjaEspionageDataLoader->loadGUI(ninjaEspionageGame, W_GUI_INIT_FILE);

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	NinjaEspionageButtonEventHandler *ninjaEspionageButtonHandler = new NinjaEspionageButtonEventHandler();
	GameGUI *gui = ninjaEspionageGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)ninjaEspionageButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	NinjaEspionageKeyEventHandler *ninjaEspionageKeyHandler = new NinjaEspionageKeyEventHandler();
	ninjaEspionageGame->getInput()->registerKeyHandler((KeyEventHandler*)ninjaEspionageKeyHandler);

	mciSendString(LPCW_PLAY_SPLASH, NULL, 0, NULL);

	// THIS WILL HANDLE PHYSICS COLLISION EVENTS
	NinjaEspionageCollisionListener *ninjaEspionageCollisionListener = new NinjaEspionageCollisionListener();
	ninjaEspionageGame->getGSM()->getPhysics()->setCollisionListener(ninjaEspionageCollisionListener);
	ninjaEspionageGame->getGSM()->addLevel(W_LEVEL_1_NAME,W_LEVEL_1_PATH);
	
	//ninjaEspionageGame->getGSM()->addLevel(W_LEVEL_2_NAME, W_LEVEL_2_PATH);
	//ninjaEspionageGame->getGSM()->addLevel(W_LEVEL_3_NAME, W_LEVEL_3_PATH);
	// START THE GAME LOOP
	ninjaEspionageGame->runGameLoop();

	// GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
	delete (WindowsOS*)ninjaEspionageGame->getOS();
	delete (WindowsInput*)ninjaEspionageGame->getInput();
	delete (WindowsTimer*)ninjaEspionageGame->getTimer();
	delete (DirectXGraphics*)ninjaEspionageGame->getGraphics();
	delete (NinjaEspionageTextGenerator*)ninjaEspionageGame->getText()->getTextGenerator();
	delete ninjaEspionageButtonHandler;
	delete ninjaEspionageKeyHandler;
	delete ninjaEspionageGame;

	// AND RETURN
	return 0;
}