#pragma once
#include "ninja_espionage_VS\stdafx.h"

/*
	NinjaEspionage.h

	This file contains setup data for making the NinjaEspionage game. Note that nearly
	all of this data would better serve us if loaded from files.
*/


// HERE IS THE NinjaEspionage GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
static const wstring	W_LEVEL_1_DIR						= L"data/levels/Level1/";
static const wstring	W_LEVEL_1_NAME						= L"Level1.tmx";
static const wstring	W_LEVEL_1_PATH						= W_LEVEL_1_DIR + W_LEVEL_1_NAME;
static const wstring	W_LEVEL_1_GUARDS_NAME				= L"Level1Guards.txt";
static const wstring	W_LEVEL_1_GUARDS					= W_LEVEL_1_DIR + W_LEVEL_1_GUARDS_NAME;
static const wstring	W_LEVEL_2_GUARDS_NAME				= L"Level2Guards.txt";
static const wstring	W_LEVEL_2_GUARDS					= W_LEVEL_1_DIR + W_LEVEL_2_GUARDS_NAME;
static const wstring	W_LEVEL_3_GUARDS_NAME				= L"Level3Guards.txt";
static const wstring	W_LEVEL_3_GUARDS					= W_LEVEL_1_DIR + W_LEVEL_3_GUARDS_NAME;

// Sound effects
static const LPCWSTR	LPCW_ATTACK_1			= L"sounds/SFX_attack1.wav";
static const LPCWSTR	LPCW_ATTACK_2			= L"sounds/SFX_attack2.wav";
static const LPCWSTR	LPCW_ATTACK_3			= L"sounds/SFX_attack3.wav";
static const LPCWSTR	LPCW_BLINK				= L"sounds/SFX_blink.wav";
static const LPCWSTR	LPCW_PLAYER_HIT			= L"sounds/SFX_playerHit.wav";

// TODO: ADDING LEVELS CORRECTLY

static const wstring	W_INIT_FILE				= L"data/NinjaEspionageInit.txt";
static const wstring	W_GUI_INIT_FILE			= L"data/NinjaEspionageGUI.txt";
static const string		W_DEBUG_FILE			= "DebugOutput.txt";

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const wstring	W_TITLE					= L"TITLE";
static const wstring	W_SCREEN_WIDTH			= L"SCREEN_WIDTH";
static const wstring	W_SCREEN_HEIGHT			= L"SCREEN_HEIGHT";
static const wstring	W_USE_FULLSCREEN_MODE	= L"USE_FULLSCREEN_MODE";
static const wstring	W_TEXT_FONT_SIZE		= L"TEXT_FONT_SIZE";
static const wstring	W_VIEWPORT_OFFSET_X		= L"VIEWPORT_OFFSET_X";
static const wstring	W_VIEWPORT_OFFSET_Y		= L"VIEWPORT_OFFSET_Y";
static const wstring	W_TOGGLE_OFFSET_Y		= L"TOGGLE_OFFSET_Y";
static const wstring	W_FONT_COLOR_RED		= L"FONT_COLOR_RED";
static const wstring	W_FONT_COLOR_GREEN		= L"FONT_COLOR_GREEN";
static const wstring	W_FONT_COLOR_BLUE		= L"FONT_COLOR_BLUE";
static const wstring	W_COLOR_KEY_RED			= L"COLOR_KEY_RED";
static const wstring	W_COLOR_KEY_GREEN		= L"COLOR_KEY_GREEN";
static const wstring	W_COLOR_KEY_BLUE		= L"COLOR_KEY_BLUE";

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring	W_GO_TO_MM_COMMAND		= L"Go To Main Menu";
static const wstring	W_GO_INTRO_COMMAND		= L"Go To Intro Screen"; //intro
static const wstring	W_START_COMMAND			= L"Start";
static const wstring	W_EXIT_COMMAND			= L"Exit";
static const wstring	W_QUIT_COMMAND			= L"Quit";
static const wstring	W_GO_TO_ABOUT_COMMAND	= L"About";
static const wstring	W_GO_TO_HELP_COMMAND	= L"Help";
static const wstring	W_CONTINUE_COMMAND		= L"Continue";

static const wstring	W_CURSOR_PATH			= L"data/gui/cursor/cursor.png";
static const wstring	W_RED_CURSOR_PATH		= L"data/gui/cursor/red_cursor.png";
static const wstring	W_SPLASH_SCREEN_PATH	= L"data/gui/overlays/splash_screen.png";
static const wstring	W_MAIN_MENU_PATH		= L"data/gui/overlays/menu2.png";
static const wstring	W_WON_PATH				= L"data/gui/overlays/won.png";
static const wstring	W_LOSS_PATH				= L"data/gui/overlays/loss.png";
static const wstring	W_ABOUT_SCREEN_PATH		= L"data/gui/overlays/about.png";
static const wstring	W_HELP_SCREEN_PATH		= L"data/gui/overlays/help.png";
static const wstring	W_INTRO_PATH			= L"data/gui/overlays/intro_screen.png"; //intro
static const wstring	W_BOT_COUNT_PATH		= L"data/gui/overlays/botCount.png";
static const wstring	W_BUTTON1_PATH			= L"data/gui/overlays/button1.png";
static const wstring	W_BUTTON2_PATH			= L"data/gui/overlays/button2.png";
static const wstring	W_COOLDOWN_PATH			= L"data/gui/overlays/cooldown.png";
static const wstring	W_TOOLBAR_BKG			= L"data/gui/overlays/ToolbarBkg.png";
static const wstring	W_LIFE_PATH				= L"data/gui/overlays/Lives3.png";
static const wstring	W_START_IMAGE_PATH		= L"data/gui/buttons/BU_START.png";
static const wstring	W_START_IMAGE_MO_PATH	= L"data/gui/buttons/BU_START2.png";
static const wstring	W_HELP_IMAGE_PATH		= L"data/gui/buttons/BU_HELP.png";
static const wstring	W_HELP_IMAGE_MO_PATH	= L"data/gui/buttons/BU_HELP2.png";
static const wstring	W_ABOUT_IMAGE_PATH		= L"data/gui/buttons/BU_ABOUT.png";
static const wstring	W_ABOUT_IMAGE_MO_PATH	= L"data/gui/buttons/BU_ABOUT2.png";
static const wstring	W_EXIT_IMAGE_PATH		= L"data/gui/buttons/BU_QUIT.png";
static const wstring	W_EXIT_IMAGE_MO_PATH	= L"data/gui/buttons/BU_QUIT2.png";
static const wstring	W_QUIT_IMAGE_PATH		= L"data/gui/buttons/BU_QUIT.png";
static const wstring	W_QUIT_IMAGE_MO_PATH	= L"data/gui/buttons/BU_QUIT2.png";

static const wstring	W_CONTINUE_IMAGE_PATH	= L"data/gui/buttons/start_game.png";
static const wstring	W_CONTINUE_IMAGE_MO_PATH = L"data/gui/buttons/start_game_mo.png";

// FOR OUR SPRITE
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
static const float		W_GRAVITY = 0.0f;
static const float		JUMPING_BOT_MIN_CYCLES = 30;
static const float		JUMPING_BOT_MAX_CYCLES = 120;
static const float		JUMPING_BOT_MAX_VELOCITY = 4.0f;

static const int		PLAYER_LEVEL1_X = 1677;  //Initial start coordinates per level
static const int		PLAYER_LEVEL1_Y = 9699;
static const int		PLAYER_LEVEL2_X = 1677;
static const int		PLAYER_LEVEL2_Y = 6600;
static const int		PLAYER_LEVEL3_X = 1670;
static const int		PLAYER_LEVEL3_Y = 3270;

static const int		GATE_LEVEL1_X = 1600;  //Initial start coordinates per level
static const int		GATE_LEVEL1_Y = 6722;
static const int		GATE1_LEVEL2_X = 1600; // Levels 2 and 3 need 2 gates
static const int		GATE1_LEVEL2_Y = 6722;
static const int		GATE2_LEVEL2_X = 1600;
static const int		GATE2_LEVEL2_Y = 3458;
static const int		GATE1_LEVEL3_X = 1600;
static const int		GATE1_LEVEL3_Y = 3485;
static const int		GATE2_LEVEL3_X = 1600;
static const int		GATE2_LEVEL3_Y = 66;

static const float		JUMP_SPEED = -24.0f;
static const float		PLAYER_SPEED = 10.0f;
static const int		MIN_FPS = 5;
static const int		MAX_FPS = 100;
static const int		FPS_INC = 1;

static const wstring	RUN_RIGHT = L"RUN_RIGHT";
static const wstring	RUN_LEFT = L"RUN_LEFT";
static const wstring	RUN_UP = L"RUN_UP";
static const wstring	RUN_DOWN = L"RUN_DOWN";
static const wstring	USE_RIGHT = L"USE_RIGHT";
static const wstring	USE_LEFT = L"USE_LEFT";
static const wstring	USE_UP = L"USE_UP";
static const wstring	USE_DOWN = L"USE_DOWN";
static const wstring	SPEC = L"SPEC";
static const wstring	IDLE = L"IDLE";
static const wstring	IDLE1 = L"IDLE1";
static const wstring	IDLE2 = L"IDLE2";
static const wstring	IDLE3 = L"IDLE3";
static const wstring	IDLE4 = L"IDLE4";
static const wstring	COOLDOWN1 = L"COOLDOWN1";
static const wstring	COOLDOWN2 = L"COOLDOWN2";
static const wstring	COOLDOWN3 = L"COOLDOWN3";
static const wstring	IDLE_UP = L"IDLE_UP";
static const wstring	IDLE_LEFT = L"IDLE_LEFT";
static const wstring	IDLE_RIGHT = L"IDLE_RIGHT";
static const wstring	DYING = L"DYING";
static const wstring	DEAD = L"DEAD";
static const wstring	LIVES1 = L"LIVES1";
static const wstring	LIVES2 = L"LIVES2";
static const wstring	LIVES3 = L"LIVES3";

// USED FOR MOVEMENT
const unsigned int W_KEY = (unsigned int)'W';
const unsigned int A_KEY = (unsigned int)'A';
const unsigned int S_KEY = (unsigned int)'S';
const unsigned int D_KEY = (unsigned int)'D';
const unsigned int G_KEY = (unsigned int)'G';
const unsigned int P_KEY = (unsigned int)'P';
const unsigned int T_KEY = (unsigned int)'T';
const unsigned int M_KEY = (unsigned int)'M';
const unsigned int N_KEY = (unsigned int)'N';
const unsigned int J_KEY = (unsigned int)'J';
const unsigned int K_KEY = (unsigned int)'K';
const unsigned int L_KEY = (unsigned int)'L';
const unsigned int ONE_KEY2 = (unsigned int)'1';
const unsigned int UP_KEY = VK_UP;
const unsigned int DOWN_KEY = VK_DOWN;
const unsigned int LEFT_KEY = VK_LEFT;
const unsigned int RIGHT_KEY = VK_RIGHT;
const unsigned int SPACE_KEY = VK_SPACE;
const unsigned int TAB_KEY = VK_TAB;
const unsigned int ZERO_KEY = VK_NUMPAD0;
const unsigned int ONE_KEY = VK_NUMPAD1;

// THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
const unsigned int C_KEY = (unsigned int)'C';