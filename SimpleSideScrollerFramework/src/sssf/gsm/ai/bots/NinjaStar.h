#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "sssf_VS\stdafx.h"
#include "sssf\gsm\ai\Bot.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\game\Game.h"


static const LPCWSTR	LPCW_BOT_HIT = L"sounds/SFX_botHit.wav";
static const LPCWSTR	LPCW_BOT_DIE = L"sounds/SFX_botDies.wav";

class NinjaStar : public Bot
{
private:
	unsigned int cyclesRemainingBeforeThinking;
	unsigned int minCyclesBeforeThinking;
	unsigned int maxCyclesBeforeThinking;
	unsigned int maxVelocity;
	unsigned int animationRandomizer;
	unsigned int i;
	unsigned int initX;
	unsigned int initY;
	int xVals[4];
	int yVals[4];

	// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
	NinjaStar(unsigned int initMin,
		unsigned int initMax,
		unsigned int initMaxVelocity);

public:
	NinjaStar(Physics *physics, unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity);
	~NinjaStar();
	Bot*	clone();
	void    initBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity); 
	void	pickRandomCyclesInRange();
	void	pickRandomJump(Physics *physics);
	void	think(Game *game);

	// INLINED METHODS
	unsigned int getCyclesRemainingBeforeThinking()	{ return cyclesRemainingBeforeThinking; }
	unsigned int getMinCyclesBeforeThinking()		{ return minCyclesBeforeThinking; }
	unsigned int getMaxCyclesBeforeThinking()		{ return maxCyclesBeforeThinking; }
};