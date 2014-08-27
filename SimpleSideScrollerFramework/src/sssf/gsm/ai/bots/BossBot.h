#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class BossBot : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;
	unsigned int animationRandomizer;
	unsigned int i;
	int x;
	int y;
	int dist;
	AnimatedSprite player;
	boolean sees = false;
	int xVals[4];
	int yVals[4];

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	BossBot(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	BossBot(Physics *physics, unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity);
	~BossBot();
	Bot*	clone();
	void initBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity);
	//void	pickRandomCyclesInRange();
	//void	pickRandomJump(Physics *physics);
	void	think(Game *game);


	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	unsigned int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking; }
	unsigned int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking; }
	boolean getSees()								{ return sees; }

	void switchSees()
	{
		if (sees)
			sees = false;
		else
			sees = true;
	}
};