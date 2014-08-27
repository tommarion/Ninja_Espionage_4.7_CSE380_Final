#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"

class RandomGuard : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;
	unsigned int animationRandomizer;
	unsigned int i;
	boolean sees = false;
	int xVals[4];
	int yVals[4];
	AABB *tempBB = new AABB(); // Used for collision comparisons

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	RandomGuard(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	RandomGuard(Physics *physics,unsigned int initMin,unsigned int initMax,unsigned int initMaxVelocity);
	~RandomGuard();
	Bot*	clone();
	void	initBot(unsigned int initMin,unsigned int initMax, unsigned int initMaxVelocity); 
	void	pickRandomCyclesInRange();
	void	pickRandomJump(Physics *physics);
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