/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

SpriteManager.h

This class manages all of the sprites in a given game level. Note
that the player sprite is also managed by this class.
*/

#pragma once
#include "sssf_VS\stdafx.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\ai\BotRecycler.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\sprite\AnimatedSpriteType.h"
#include "sssf\gsm\ai\bots\RandomObject.h"

static const LPCWSTR	LPCW_BOT_SEES = L"sounds/SFX_botSees.wav";
static const LPCWSTR	LPCW_PLAYER_DIE = L"sounds/SFX_playerDies.wav";
static const LPCWSTR	LPCW_LEVEL_COMPLETE = L"sounds/SFX_levelComplete.wav";
static const int		GOAL_1 = 6600;
static const int		GOAL_2 = 3340;
static const int		GOAL_3 = 0;

class SpriteManager
{
private:
	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	AnimatedSprite player;

	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
	BotRecycler recyclableBots;

	// CHECK FOR NINJA STAR REMOVAL
	boolean star;

	boolean won = false;
	boolean loaded = false;

	int starsThrown = 0;
	int start = 0;
	int lossCount = 0;
	int lives = 3;
	boolean dead = false;

	float xCheck = 0;
	float yCheck = 0;

	AABB *tempBB = new AABB();
	int	BOT_COUNT = 0; // GUARD COUNTER

	boolean gameWon = false; // used for current game won and guard removal afterward
	boolean finalWin = false; // used for final game win screen

	boolean killAll = false; // used to kill all guards


public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager()		{}
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	int						getNumberOfSprites()	{ return bots.size(); }
	AnimatedSprite*			getPlayer()				{ return &player; }
	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin(); }
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end(); }
	int						getStarsThrown()		{ return starsThrown; }

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addSpriteItemsToRenderList(Game *game);
	unsigned int		addSpriteType(AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport);
	void				clearSprites();
	void				clearBots();
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	Bot*				removeBot(Bot *botToRemove);
	void				unloadSprites();
	void				update(Game *game);
	void				checkForLevelWin(Game *game);
	void				levelWinResults(Game *game);
	void				decrementStars(){ if (starsThrown > 0) starsThrown--; }
	void				incrementStars(){ starsThrown++; }


	// GET AND SETS
	boolean		getLoaded()	{ return loaded; }
	void		setLoaded(boolean l) { loaded = l; }
	void		resetDead(){ dead = false; }
	void		resetWon(){ won = false; }
	int			getStartCounter() { return start; }
	boolean		getDead(){ return dead; }
	void		setDead(boolean val){ dead = val; }
	int			getLives() { return lives; }
	void		setLives(int newLives){ lives = newLives; }
	boolean		getGameWon(){ return gameWon; }
	boolean		getKillAll(){ return killAll; }
	void		setKillAll(boolean value){ killAll = value; }
	int			getBotCount() { return BOT_COUNT; }
	void		setBotCount(int count)
	{
		if (count >= 0)
			BOT_COUNT = count;
	}
};