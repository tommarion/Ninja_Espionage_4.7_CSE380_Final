/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.cpp

	See SpriteManager.h for a class description.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\sprite\SpriteManager.h"
#include "sssf\gsm\state\GameStateManager.h"

/*
	addSpriteToRenderList - This method checks to see if the sprite
	parameter is inside the viewport. If it is, a RenderItem is generated
	for that sprite and it is added to the render list.
*/
void SpriteManager::addSpriteToRenderList(AnimatedSprite *sprite,
										  RenderList *renderList,
										  Viewport *viewport)
{
	// GET THE SPRITE TYPE INFO FOR THIS SPRITE
	AnimatedSpriteType *spriteType = sprite->getSpriteType();
	PhysicalProperties *pp = sprite->getPhysicalProperties();

	// IS THE SPRITE VIEWABLE?
	if ((viewport->areWorldCoordinatesInViewport(	pp->getX(),
													pp->getY(),
													spriteType->getTextureWidth(),
													spriteType->getTextureHeight()))
		|| ( spriteType->getSpriteTypeID() == 3 )
		|| (spriteType->getSpriteTypeID() == 4))
	{
		int x, y;
		if (spriteType->getSpriteTypeID() == 3
			|| spriteType->getSpriteTypeID() == 4)
		{
			x = (int)pp->getX();
			y = (int)pp->getY();
		}
		else{
			x = (int)pp->round(pp->getX() - viewport->getViewportX());
			y = (int)pp->round(pp->getY() - viewport->getViewportY());
		}
		// SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
		RenderItem itemToAdd;
		itemToAdd.id = sprite->getFrameIndex();
		renderList->addRenderItem(	sprite->getCurrentImageID(),
									x,
									y,
									pp->round(pp->getZ()),
									sprite->getAlpha(),
									spriteType->getTextureWidth(),
									spriteType->getTextureHeight());
	}
}

/*
	addSpriteItemsToRenderList - This method goes through all of the sprites,
	including the player sprite, and adds the visible ones to the render list.
	This method should be called each frame.
*/
void SpriteManager::addSpriteItemsToRenderList(	Game *game)
{
	GameStateManager *gsm = game->getGSM();
	GameGUI *gui = game->getGUI();
	if (gsm->isWorldRenderable())
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorldRenderList();
		Viewport *viewport = gui->getViewport();

		// ADD THE PLAYER SPRITE
		addSpriteToRenderList(&player, renderList, viewport);
		if(player.getPhysicalProperties()->getY() > game->getGSM()->getWorld()->getWorldHeight())
		{
			game->getGSM()->getSpriteManager()->clearSprites();
			
		}

		// NOW ADD THE REST OF THE SPRITES
		list<Bot*>::iterator botIterator;
		botIterator = bots.begin();
		while (botIterator != bots.end())
		{			
			Bot *bot = (*botIterator);
			addSpriteToRenderList(bot, renderList, viewport);
			botIterator++;
		}
	}
}

/*
	addSprite - This method is for adding a new sprite to 
	this sprite manager. Once a sprite is added it can be 
	scheduled for rendering.
*/
void SpriteManager::addBot(Bot *botToAdd)
{
	bots.push_back(botToAdd);
}

/*
	addSpriteType - This method is for adding a new sprite
	type. Note that one sprite type can have many sprites. For
	example, we may say that there may be a "Bunny" type of
	sprite, and specify properties for that type. Then there might
	be 100 different Bunnies each with their own properties, but that
	share many things in common according to what is defined in
	the shared sprite type object.
*/
unsigned int SpriteManager::addSpriteType(AnimatedSpriteType *spriteTypeToAdd)
{
	spriteTypes.push_back(spriteTypeToAdd);
	return spriteTypes.size()-1;
}

/*
	clearSprites - This empties all of the sprites and sprite types.
*/
void SpriteManager::clearSprites()
{
	spriteTypes.clear();
	bots.clear();
}

void SpriteManager::clearBots()
{
	bots.clear();
}

/*
	getSpriteType - This gets the sprite type object that corresponds
	to the index argument.
*/
AnimatedSpriteType* SpriteManager::getSpriteType(unsigned int typeIndex)
{
	if (typeIndex < spriteTypes.size())
		return spriteTypes.at(typeIndex);
	else
		return NULL;
}

/*
	unloadSprites - This method removes all artwork from memory that
	has been allocated for game sprites.
*/
void SpriteManager::unloadSprites()
{

}

Bot* SpriteManager::removeBot(Bot *botToRemove)
{
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);
		if (botToRemove == bot)
		{
			bots.remove(bot);
			star = true;
			break;
		}
		botIterator++;
	}
	return NULL;
	// @TODO - WE'LL DO THIS LATER WHEN WE LEARN MORE ABOUT MEMORY MANAGEMENT
}


/*
update - This method should be called once per frame. It
goes through all of the sprites, including the player, and calls their
update method such that they may update themselves.
*/
void SpriteManager::update(Game *game)
{

	/// CHECK WIN CONDITIONS
	checkForLevelWin(game); // did we win? if so inform necessary bots

	// UPDATE THE PLAYER SPRITE
	player.updateSprite();
	star = false;
	// NOW UPDATE THE REST OF THE SPRITES
	list<Bot*>::iterator botIterator;
	botIterator = bots.begin();
	while (botIterator != bots.end())
	{
		Bot *bot = (*botIterator);

		if (dead) // if player is dead, start the death timer
		{
			if (lossCount < 40)
			{
				starsThrown = 0;
				if (lossCount == 4) // after 4 cycles, play death sound
					PlaySound(LPCW_PLAYER_DIE, NULL, SND_FILENAME | SND_ASYNC);

				lossCount++; // increment death timer
			}
			else { // once 40 cycles have passed, exit the screen
				lossCount = 0;	// re-initiate counter for next time
				if (lives <= 0)
				{
					lives = 3;
					player.setCurrentLevel(1);
					game->getGSM()->goToGameFail(game); // no more lives back to main menu and restart
				}
				else // else go to normal game loss screen
					game->getGSM()->goToGameLost(game);
			}
		}	
					
		start++; //continue start counter
		bot->think(game);
		bot->updateSprite();
		if (star == false)
			botIterator++;
		else
			return;
	}

	if (BOT_COUNT == 0)
	{
		levelWinResults(game); // if the game is WON, reset win flag, then go to correct win screen
		killAll = false;
	}
}

void SpriteManager::checkForLevelWin(Game *game)
{
	// WIN CONDITIONS
	if (!gameWon)
	{
		if (player.getPhysicalProperties()->getY() < GOAL_1 && player.getCurrentLevel() == 1) // if on level 1
		{
			player.setCurrentLevel(2);
			start = 0;					// reset level start immunity
			player.getPhysicalProperties()->setVelocity(0, 0); //set movement to 0;
			player.setCurrentState(L"IDLE");
			gameWon = true;
		}

		else if (player.getPhysicalProperties()->getY() < GOAL_2 && player.getCurrentLevel() == 2) // if on level 2
		{
			player.setCurrentLevel(3);
			start = 0;
			player.getPhysicalProperties()->setVelocity(0, 0);
			player.setCurrentState(L"IDLE");
			gameWon = true;
		}

		else if (player.getPhysicalProperties()->getY() < GOAL_3 && player.getCurrentLevel() == 3) // if on level 3
		{
			lives = 3;
			player.setCurrentLevel(1);
			start = 0;
			player.getPhysicalProperties()->setVelocity(0, 0);
			player.setCurrentState(L"IDLE");
			gameWon = true;
			finalWin = true;
		}
	}
}

void SpriteManager::levelWinResults(Game *game)
{
	// if the game is WON, reset win flag, then go to correct win screen
	if (finalWin == true)	// if the last level is won go to final win screen
	{
		starsThrown = 0;
		player.setCurrentLevel(1);
		finalWin = false;		// reset flag
		gameWon = false;		// reset flag

		//game->getGSM()->goToGameEnd(game); /// Go to final screen TODO
		game->getGSM()->goToGameWon(game); //Temp
	}

	if (gameWon == true)		// check if we won
	{
		starsThrown = 0;
		gameWon = false;		// reset flag
		game->getGSM()->goToGameWon(game);		// else load normal win screen
	}
}