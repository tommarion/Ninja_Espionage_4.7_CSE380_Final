#include "ninja_espionage_VS\stdafx.h"

// NinjaEspionage GAME INCLUDES
#include "ninja_espionage\NinjaEspionageButtonEventHandler.h"
#include "ninja_espionage\NinjaEspionageDataLoader.h"
#include "ninja_espionage\NinjaEspionage.h"
#include "ninja_espionage\NinjaEspionageKeyEventHandler.h"
#include "ninja_espionage\NinjaEspionageTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf\game\Game.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\bots\RandomGuard.h"
#include "sssf\gsm\ai\bots\BossBot.h"
#include "sssf\gsm\ai\bots\RandomObject.h"
#include "sssf\gsm\ai\bots\NinjaStar.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\world\TiledLayer.h"
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

// TMX MAP LOADING
#include "tmxmi\TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti\PoseurSpriteTypesImporter.h"

//BOT DATA LOADING
#include "sssf\text\BufferedTextFileReader.h"

/*
	loadGame - This method loads the setup game data into the game and
	constructs all the needed objects for the game to work.
*/
void NinjaEspionageDataLoader::loadGame(Game *game, wstring gameInitFile)
{
	// AND LET'S READ IN THE GAME SETUP INFO
	// FIRST LOAD ALL THE PROPERTIES
	map<wstring,wstring> *properties = new map<wstring,wstring>();
	loadGameProperties(game, properties, gameInitFile);

	// WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
	wstring titleProp = (*properties)[W_TITLE];
	wstring useFullscreenProp = (*properties)[W_USE_FULLSCREEN_MODE];
	bool useFullscreen = false;
	if (useFullscreenProp.compare(L"true") == 0)
		useFullscreen = true;

	// GET THE SCREEN WIDTH AND HEIGHT
	int screenWidth, screenHeight;
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;

	// MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
	// PLATFORM STUFF, INCLUDING A Window OF COURSE
	WindowsOS *ninjaEspionageOS = new WindowsOS(	hInstance, 
										nCmdShow,
										useFullscreen,
										titleProp,
										screenWidth, screenHeight,
										game);
	
	int textFontSize;
	wstring textFontSizeProp = (*properties)[W_TEXT_FONT_SIZE];
	wstringstream(textFontSizeProp) >> textFontSize;

	// RENDERING WILL BE DONE USING DirectX
	DirectXGraphics *ninjaEspionageGraphics = new DirectXGraphics(game);
	ninjaEspionageGraphics->init(screenWidth, screenHeight);
	ninjaEspionageGraphics->initGraphics(ninjaEspionageOS, useFullscreen);
	ninjaEspionageGraphics->initTextFont(textFontSize);

	// AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
	// AS A COLOR KEY AND FOR RENDERING TEXT
	initColors(ninjaEspionageGraphics, properties);

	// WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT
	WindowsInput *ninjaEspionageInput = new WindowsInput();

	// AND WINDOWS FOR THE TIMER AS WELL
	WindowsTimer *ninjaEspionageTimer = new WindowsTimer();

	// NOW INITIALIZE THE Game WITH ALL THE
	// PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
	game->initPlatformPlugins(	(GameGraphics*)ninjaEspionageGraphics,
								(GameInput*)ninjaEspionageInput,
								(GameOS*)ninjaEspionageOS,
								(GameTimer*)ninjaEspionageTimer);

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
	// TEXT ON THE SCREEN EACH FRAME
	NinjaEspionageTextGenerator *ninjaEspionageTextGenerator = new NinjaEspionageTextGenerator();
	ninjaEspionageTextGenerator->initText(game);
	GameText *text = game->getText();
	text->initDebugFile(W_DEBUG_FILE);
	text->setTextGenerator((TextGenerator*)ninjaEspionageTextGenerator);

	// INIT THE VIEWPORT TOO
	initViewport(game->getGUI(), properties);
	Viewport *view = game->getGUI()->getViewport();
	view->setViewportX(930 - (view->getViewportWidth()/2));
	view->setViewportY(1530 - (view->getViewportHeight()/2));

	// WE DON'T NEED THE PROPERTIES MAP ANYMORE, THE GAME IS ALL LOADED
	delete properties;
}

/*
	initColors - this helper method loads the color key, used for loading
	images, and the font color, used for rendering text.
*/
void NinjaEspionageDataLoader::initColors(	GameGraphics *graphics,
									map<wstring,wstring> *properties)
{
	int fontRed, fontGreen, fontBlue;
	wstring fontRedProp = (*properties)[W_FONT_COLOR_RED];
	wstring fontGreenProp = (*properties)[W_FONT_COLOR_GREEN];
	wstring fontBlueProp = (*properties)[W_FONT_COLOR_BLUE];
	wstringstream(fontRedProp) >> fontRed;
	wstringstream(fontGreenProp) >> fontGreen;
	wstringstream(fontBlueProp) >> fontBlue;

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(fontRed, fontGreen, fontBlue);

	int keyRed, keyGreen, keyBlue;
	wstring keyRedProp = (*properties)[W_COLOR_KEY_RED];
	wstring keyGreenProp = (*properties)[W_COLOR_KEY_GREEN];
	wstring keyBlueProp = (*properties)[W_COLOR_KEY_BLUE];
	wstringstream(keyRedProp) >> keyRed;
	wstringstream(keyGreenProp) >> keyGreen;
	wstringstream(keyBlueProp) >> keyBlue;

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	// NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
	// CHANNEL DATA, YOU DON'T NEED A COLOR KEY
	graphics->setColorKey(keyRed, keyGreen, keyBlue);
}

/*
	loadGUI - This method loads all the GUI assets described in the guiInitFile
	argument. Note that we are loading all GUI art for all GUIs when the application
	first starts. We'll learn a better technique later in the semester.
*/
void NinjaEspionageDataLoader::loadGUI(Game *game, wstring guiInitFile)
{
	// WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
	// FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
	hardCodedLoadGUIExample(game);
}


/*
	initNinjaEspionageGUI - This method builds a GUI for the NinjaEspionage Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void NinjaEspionageDataLoader::hardCodedLoadGUIExample(Game *game)
{
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// SETUP THE CURSOR VIA OUR HELPER METHOD
	initCursor(gui, guiTextureManager);
	initSplashScreen(game, gui, guiTextureManager);
	initMainMenu(gui, guiTextureManager);
	initInGameGUI(gui, guiTextureManager);
	initGameWonGUI(gui, guiTextureManager);
	initGameLossGUI(gui, guiTextureManager);
	initGameFailGUI(gui, guiTextureManager);
	initHelpMenuGUI(gui, guiTextureManager);
	initAboutMenuGUI(gui, guiTextureManager);
	initGameIntroGUI(game, gui, guiTextureManager);

}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void NinjaEspionageDataLoader::initCursor(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(W_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);
}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void NinjaEspionageDataLoader::initSplashScreen(Game *game, GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							game->getGraphics()->getScreenWidth(),
							game->getGraphics()->getScreenHeight(),
							false,
							W_GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void NinjaEspionageDataLoader::initMainMenu(GameGUI *gui,	DirectXTextureManager *guiTextureManager)
{
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_MO_PATH);

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							185,
							615,
							0,
							255,
							300,
							100,
							false,
							W_EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							185,
							315,
							0,
							255,
							300,
							100,
							false,
							W_GO_INTRO_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	
	// AND LET'S ADD A HELP BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_HELP_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_HELP_IMAGE_MO_PATH);

	// - INIT THE HELP BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		185,
		415,
		0,
		255,
		300,
		100,
		false,
		W_GO_TO_HELP_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A ABOUT BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_MO_PATH);

	// - INIT THE ABOUT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		185,
		515,
		0,
		255,
		300,
		100,
		false,
		W_GO_TO_ABOUT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);
	

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MAIN_MENU,		mainMenuGUI);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void NinjaEspionageDataLoader::initInGameGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	//############ Toolbar image ###############
	unsigned int imageID = guiTextureManager->loadTexture(W_TOOLBAR_BKG);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 480;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 800;
	imageToAdd->height = 200;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);
	
	//############ Bot counter image ############
	imageID = guiTextureManager->loadTexture(W_BOT_COUNT_PATH);
	imageToAdd = new OverlayImage();
	imageToAdd->x = 746;
	imageToAdd->y = 710;
	imageToAdd->z = 0;
	imageToAdd->alpha = 255;
	imageToAdd->width = 200;
	imageToAdd->height = 46;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							655,
							0,
							255,
							200,
							100,
							false,
							W_QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_IN_PROGRESS,	inGameGUI);

	
}

void NinjaEspionageDataLoader::initGameWonGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *gameWonGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_WON_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	gameWonGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							648,
							0,
							255,
							300,
							100,
							false,
							W_QUIT_COMMAND);
	gameWonGUI->addButton(buttonToAdd);

	normalTextureID = guiTextureManager->loadTexture(W_CONTINUE_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CONTINUE_IMAGE_MO_PATH);

	
	// INIT THE CONTINUE BUTTON
	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		400,
		648,
		0,
		255,
		200,
		100,
		false,
		W_CONTINUE_COMMAND);
	gameWonGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_WON,	gameWonGUI);
}

void NinjaEspionageDataLoader::initGameLossGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *gameLossGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_LOSS_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	gameLossGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		648,
		0,
		255,
		200,
		100,
		false,
		W_QUIT_COMMAND);
	gameLossGUI->addButton(buttonToAdd);


	normalTextureID = guiTextureManager->loadTexture(W_CONTINUE_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CONTINUE_IMAGE_MO_PATH);

	// INIT THE CONTINUE BUTTON
	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		400,
		648,
		0,
		255,
		200,
		100,
		false,
		W_CONTINUE_COMMAND);
	gameLossGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_LOST, gameLossGUI);
}

void NinjaEspionageDataLoader::initGameFailGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *gameFailGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_LOSS_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	gameFailGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		648,
		0,
		255,
		200,
		100,
		false,
		W_QUIT_COMMAND);
	gameFailGUI->addButton(buttonToAdd);


	normalTextureID = guiTextureManager->loadTexture(W_CONTINUE_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(W_CONTINUE_IMAGE_MO_PATH);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_GAME_FAIL, gameFailGUI);
}


void NinjaEspionageDataLoader::initAboutMenuGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *aboutScreenGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_ABOUT_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	aboutScreenGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		655,
		0,
		255,
		200,
		100,
		false,
		W_QUIT_COMMAND);
	aboutScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MENU_ABOUT_MENU, aboutScreenGUI);
}


void NinjaEspionageDataLoader::initHelpMenuGUI(GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *helpScreenGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(W_HELP_SCREEN_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 442;
	imageToAdd->imageID = imageID;
	helpScreenGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		655,
		0,
		255,
		200,
		100,
		false,
		W_QUIT_COMMAND);
	helpScreenGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	gui->addScreenGUI(GS_MENU_HELP_MENU, helpScreenGUI);
}

void NinjaEspionageDataLoader::initGameIntroGUI(Game *game, GameGUI *gui, DirectXTextureManager *guiTextureManager)
{
	// NOW ADD THE IN-GAME GUI
	ScreenGUI *gameIntroGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(W_INTRO_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	// INIT THE SCREEN BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		game->getGraphics()->getScreenWidth(),
		game->getGraphics()->getScreenHeight(),
		false,
		W_START_COMMAND);
	gameIntroGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	gui->addScreenGUI(GS_INTRO_SCREEN, gameIntroGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void NinjaEspionageDataLoader::initViewport(GameGUI *gui, map<wstring,wstring> *properties)
{
	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportOffsetX, viewportOffsetY, toggleOffsetY, screenWidth, screenHeight;
	wstring viewportOffsetXProp = (*properties)[W_VIEWPORT_OFFSET_X];
	wstring viewportOffsetYProp = (*properties)[W_VIEWPORT_OFFSET_Y];
	wstring toggleOffsetYProp = (*properties)[W_TOGGLE_OFFSET_Y];
	wstring screenWidthProp = (*properties)[W_SCREEN_WIDTH];
	wstring screenHeightProp = (*properties)[W_SCREEN_HEIGHT];
	wstringstream(viewportOffsetXProp) >> viewportOffsetX;
	wstringstream(viewportOffsetYProp) >> viewportOffsetY;
	wstringstream(toggleOffsetYProp) >> toggleOffsetY;
	wstringstream(screenWidthProp) >> screenWidth;
	wstringstream(screenHeightProp) >> screenHeight;
	int viewportWidth = screenWidth - viewportOffsetX;
	int viewportHeight = screenHeight - viewportOffsetY;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setViewportOffsetX(viewportOffsetX);
	viewport->setViewportOffsetY(-18);
	viewport->setToggleOffsetY(toggleOffsetY);
}

/*
loadLevel - This method should load the data the level described by the
levelInitFile argument in to the Game's game state manager.
*/
void NinjaEspionageDataLoader::loadWorld(Game *game, wstring levelInitFile)	// Initiates game map, and guards
{
	// LOAD THE LEVEL'S BACKGROUND TILES
	TMXMapImporter tmxMapImporter;
	tmxMapImporter.loadWorld(game, W_LEVEL_1_DIR, W_LEVEL_1_NAME);

	Physics *physics = game->getGSM()->getPhysics();

	

	//initiate the guard array
	for (int i = 0; i < 500; i++)
	{
		guards[i] = new RandomGuard(physics, 30, 60, 4);
	}

	reloadWorld(game, levelInitFile);

}


void NinjaEspionageDataLoader::reloadWorld(Game *game, wstring levelInitFile)
{
	// LOAD THE LEVEL'S SPRITE IMAGES
	PoseurSpriteTypesImporter psti;
	psti.loadSpriteTypes(game, SPRITE_TYPES_LIST);

	GameStateManager *gsm = game->getGSM();
	Physics *physics = gsm->getPhysics();
	physics->setGravity(W_GRAVITY);
	SpriteManager *spriteManager = gsm->getSpriteManager();

	//spriteManager->clearBots();

	// LET'S MAKE A PLAYER SPRITE
	// @TODO - IT WOULD BE BETTER TO LOAD THIS STUFF FROM A FILE
	AnimatedSprite *player = spriteManager->getPlayer();
	AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(0);
	botCount = 0;


	//loadboss

	BossBot *boss = new BossBot(physics, 0, 0, 0);
	physics->addCollidableObject(boss);
	PhysicalProperties *pp6 = boss->getPhysicalProperties();

	//////////////////////// LEVEL 1-3 PLAYER, GATE, AND GUARD LOADING //////////////////////////////
	switch (player->getCurrentLevel())
	{
	case 1:
		createPlayer(spriteManager, physics, player, PLAYER_LEVEL1_X, PLAYER_LEVEL1_Y);
		createGate(spriteManager, physics, GATE_LEVEL1_X, GATE_LEVEL1_Y);

		// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
		loadGuardInfo(W_LEVEL_1_GUARDS, botSpriteType, game);
		break;

	case 2:
		createPlayer(spriteManager, physics, player, PLAYER_LEVEL2_X, PLAYER_LEVEL2_Y);
		createGate(spriteManager, physics, GATE1_LEVEL2_X, GATE1_LEVEL2_Y);
		createGate(spriteManager, physics, GATE2_LEVEL2_X, GATE2_LEVEL2_Y);
		
		// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
		loadGuardInfo(W_LEVEL_2_GUARDS, botSpriteType, game);
		
		break;

	case 3:
		createPlayer(spriteManager, physics, player, PLAYER_LEVEL3_X, PLAYER_LEVEL3_Y);
		createGate(spriteManager, physics, GATE1_LEVEL3_X, GATE1_LEVEL3_Y);
		createGate(spriteManager, physics, GATE2_LEVEL3_X, GATE2_LEVEL3_Y);
		

		
		pp6->setPosition(1600, 1000);
		boss->setSpriteType(spriteManager->getSpriteType(6));
		boss->setCurrentState(L"IDLE");
		boss->setAlpha(255);
		spriteManager->addBot(boss);
		boss->affixTightAABBBoundingVolume();
		pp6->setXPos(1600);
		pp6->setYPos(1000);
		pp6->setCollidable(true);

		botCount++;

		// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
		//loadGuardInfo(W_LEVEL_3_GUARDS, botSpriteType, game);
		break;

	default:
		createPlayer(spriteManager, physics, player, PLAYER_LEVEL1_X, PLAYER_LEVEL1_Y);
		createGate(spriteManager, physics, GATE_LEVEL1_X, GATE_LEVEL1_Y);

		// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
		loadGuardInfo(W_LEVEL_1_GUARDS, botSpriteType, game);
		player->setCurrentLevel(1);
	}

	spriteManager->setBotCount(botCount); /// sets Level Guard Count
	game->getGraphics()->setTextShouldBeRendered(true);

	Viewport *view = game->getGUI()->getViewport();
	view->setViewportX((int)player->getPhysicalProperties()->getX() + 30 - (view->getViewportWidth() / 2));
	view->setViewportY((int)player->getPhysicalProperties()->getY() + 30 - (view->getViewportHeight() / 2));
	
	///// NINJA STAR GUI /////////
	PhysicalProperties *pp = attack1->getPhysicalProperties();
	pp->setPosition(293, 672);
	attack1->setSpriteType(spriteManager->getSpriteType(3));
	attack1->setCurrentState(IDLE1);
	attack1->setAlpha(255);
	spriteManager->addBot(attack1);
	attack1->affixTightAABBBoundingVolume();
	pp->setXPos(295);
	pp->setYPos(672);

	/////// BLINK GUI ////////////////
	PhysicalProperties *pp2 = attack2->getPhysicalProperties();
	pp2->setPosition(405, 672);
	attack2->setSpriteType(spriteManager->getSpriteType(3));
	attack2->setCurrentState(IDLE2);
	attack2->setAlpha(255);
	spriteManager->addBot(attack2);
	attack2->affixTightAABBBoundingVolume();
	pp2->setXPos(405);
	pp2->setYPos(672);

	////////// TRIPLE ATTACK GUI ///////////////
	PhysicalProperties *pp4 = attack3->getPhysicalProperties();
	pp4->setPosition(515, 672);
	attack3->setSpriteType(spriteManager->getSpriteType(3));
	attack3->setCurrentState(IDLE3);
	attack3->setAlpha(255);
	spriteManager->addBot(attack3);
	attack3->affixTightAABBBoundingVolume();
	pp4->setXPos(530);
	pp4->setYPos(680);

	////////// FAT ATTACK GUI ///////////////
	PhysicalProperties *pp5 = attack4->getPhysicalProperties();
	pp5->setPosition(625, 672);
	attack4->setSpriteType(spriteManager->getSpriteType(3));
	attack4->setCurrentState(IDLE4);
	attack4->setAlpha(255);
	spriteManager->addBot(attack4);
	attack4->affixTightAABBBoundingVolume();
	pp5->setXPos(530);
	pp5->setYPos(680);

	////////// LIVES COUNTER GUI //////////////
	PhysicalProperties *pp3 = lives->getPhysicalProperties();
	pp3->setPosition(746, 676);
	lives->setSpriteType(spriteManager->getSpriteType(4));
	lives->setCurrentState(LIVES3);
	lives->setAlpha(255);
	spriteManager->addBot(lives);
	lives->affixTightAABBBoundingVolume();
	pp3->setXPos(531);
	pp3->setYPos(680);
}

void NinjaEspionageDataLoader::createPlayer(SpriteManager *sm, Physics *physics, AnimatedSprite *player, int xVal, int yVal)
{
	physics->addCollidableObject(player);
	AnimatedSpriteType *playerSpriteType = sm->getSpriteType(1);
	player->setSpriteType(playerSpriteType);
	player->setAlpha(255);
	player->setCurrentState(IDLE);
	PhysicalProperties *playerProps = player->getPhysicalProperties();
	playerProps->setX((float)xVal);
	playerProps->setY((float)yVal);
	playerProps->setVelocity(0.0f, 0.0f);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	playerProps->setCollidable(true);
	player->setCurrentlyCollidable(true);
	sm->resetDead();
	player->setOnTileThisFrame(false);
	player->setOnTileLastFrame(false);
	player->setJumpedOnce(false);
	player->affixTightAABBBoundingVolume();
}

void NinjaEspionageDataLoader::makeRandomGuard(Game *game, AnimatedSpriteType *RandomGuardType, float initX, float initY, int i)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	RandomGuard *bot = guards[i];
	physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	bot->setSpriteType(RandomGuardType);
	bot->setCurrentState(IDLE);
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	bot->affixTightAABBBoundingVolume();
	pp->setXPos(initX);
	pp->setYPos(initY);
}

void NinjaEspionageDataLoader::makeNinjaStar(Game *game, AnimatedSpriteType *ninjaStar, float initX, float initY, float velocityX, float velocityY, bool spec)
{
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	Physics *physics = game->getGSM()->getPhysics();
	NinjaStar *bot = new NinjaStar(physics, 30, 60, 4);
	physics->addCollidableObject(bot);
	PhysicalProperties *pp = bot->getPhysicalProperties();
	pp->setPosition(initX, initY);
	bot->setSpriteType(ninjaStar);
	pp->setVelocity(velocityX, velocityY);
	if (spec){
		bot->setCurrentState(SPEC);
	} else {
		bot->setCurrentState(IDLE);
	}
	bot->setAlpha(255);
	spriteManager->addBot(bot);
	bot->affixTightAABBBoundingVolume();

}

void NinjaEspionageDataLoader::loadGuardInfo(wstring gameInitFile, AnimatedSpriteType *botSpriteType, Game *game)
{
	// OPEN THE READER AND LOAD IT WITH
	// THE CONTENTS OF gameInitFile
	BufferedTextFileReader reader;
	reader.initFile(gameInitFile);
	wstring line;
	// NOW WE CAN READ IN ALL THE GAME DATA. 
	// LOAD ALL THE PROPERTIES
	while (reader.hasMoreLines())
	{
		line = reader.getNextLine();
		loadLineInfo(&line, botSpriteType, game, temp);
		botCount++;
		temp++;
	}
	reader.closeReader();
}

void NinjaEspionageDataLoader::loadLineInfo(wstring *line, AnimatedSpriteType *botSpriteType, Game *game, int i)
{
	unsigned int delimiterIndex = line->find(DEFAULT_DELIMITER);
	wstring xString = line->substr(0, delimiterIndex);
	int x = wstringToInt(xString);
	wstring yString = line->substr(delimiterIndex + 1);
	int y = wstringToInt(yString);

	// AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
	makeRandomGuard(game, botSpriteType, (float)x, (float)y, i);

}

unsigned int NinjaEspionageDataLoader::wstringToInt(wstring input)
{
	std::wistringstream wstrm(input);
	unsigned int number;
	wstrm >> number;
	cout << number << "\n";
	number = _wtoi(input.c_str());
	cout << number << "\n";
	return number;
}

void NinjaEspionageDataLoader::createGate(SpriteManager *sm, Physics *physics, int xVal, int yVal)
{
	// Gate Bot
	RandomObject *bot4 = new RandomObject(physics, 0, 0, 0);
	physics->addCollidableObject(bot4);
	PhysicalProperties *pp4 = bot4->getPhysicalProperties();
	pp4->setPosition((float)xVal, (float)yVal);
	bot4->setSpriteType(sm->getSpriteType(5));
	bot4->setCurrentState(IDLE);
	bot4->setAlpha(255);
	sm->addBot(bot4);
	bot4->affixTightAABBBoundingVolume();
	pp4->setXPos((float)xVal);
	pp4->setYPos((float)yVal);
}