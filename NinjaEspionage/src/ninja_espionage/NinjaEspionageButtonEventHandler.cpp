/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

NinjaEspionageButtonEventHandler.cpp

See NinjaEspionageButtonEventHandler.h for a class description.
*/

#include "ninja_espionage_VS\stdafx.h"
#include "ninja_espionage\NinjaEspionage.h"
#include "ninja_espionage\NinjaEspionageButtonEventHandler.h"
#include "ninja_espionage\NinjaEspionageDataLoader.h"
#include "ninja_espionage\NinjaEspionageKeyEventHandler.h"
#include "ninja_espionage\NinjaEspionageCollisionListener.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\game\WStringTable.h"

void NinjaEspionageButtonEventHandler::handleButtonEvents(Game *game,
	wstring command)
{
	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(W_EXIT_COMMAND) == 0)
	{
		game->shutdown();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(W_GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	else if (command.compare(W_GO_INTRO_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		SpriteManager *sm = gsm->getSpriteManager();
		if (!sm->getLoaded())
		{
			gsm->goToIntroScreen();
		}
		else
		{
			SpriteManager *sm = game->getGSM()->getSpriteManager();
			GameStateManager *gsm = game->getGSM();
			Physics *physics = gsm->getPhysics();
			AnimatedSprite *player = sm->getPlayer();

			physics->removeCollidableObjectBot(player);
			game->setCurrentLevelFileName(W_LEVEL_1_NAME);
			game->startGame();
		}
	}
	else if (command.compare(W_CONTINUE_COMMAND) == 0)
	{
		SpriteManager *sm = game->getGSM()->getSpriteManager();
		GameStateManager *gsm = game->getGSM();
		Physics *physics = gsm->getPhysics();
		AnimatedSprite *player = sm->getPlayer();

		sm->clearBots();
		physics->removeCollidableObjectBot(player);
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->startGame();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(W_START_COMMAND) == 0)
	{
		SpriteManager *sm = game->getGSM()->getSpriteManager();
		game->setCurrentLevelFileName(W_LEVEL_1_NAME);
		game->startGame();

	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(W_QUIT_COMMAND) == 0)
	{
		SpriteManager *sm = game->getGSM()->getSpriteManager();
		sm->clearBots();
		GameStateManager *gsm = game->getGSM();
		Physics *physics = gsm->getPhysics();
		AnimatedSprite *player = sm->getPlayer();
		physics->removeCollidableObjectBot(player);
		game->getGraphics()->setTextShouldBeRendered(false);
		game->quitGame();
	}
	else if (command.compare(W_GO_TO_ABOUT_COMMAND) == 0)
	{
		game->getGSM()->goToAboutScreen();
	}
	else if (command.compare(W_GO_TO_HELP_COMMAND) == 0)
	{
		game->getGSM()->goToHelpScreen();
	}
}