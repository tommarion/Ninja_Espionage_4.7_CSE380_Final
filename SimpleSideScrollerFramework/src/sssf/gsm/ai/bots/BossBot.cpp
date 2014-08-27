#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\BossBot.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"


//header error?

/*
This private constructor is only to be used for cloning bots, note
that this does not setup the velocity for this bot.
*/
BossBot::BossBot(unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);

	cyclesRemainingBeforeThinking = 30;
	pp.setVelocity(0.0f, 0.0f);

	//pickRandomCyclesInRange();
}

/*
This is the public constructor used by other classes for
creating these types of bots.
*/
BossBot::BossBot(Physics *physics,
	unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
	this->pp.setVelocity(0, 0);
	//pickRandomJump(physics);
	//pickRandomCyclesInRange();


}

/*
clone - this method makes another BossBot object, but does
not completely initialize it with similar data to this. Most of the
object, like velocity and position, are left uninitialized.
*/
Bot* BossBot::clone()
{
	BossBot *botClone = new BossBot(minCyclesBeforeThinking,
		maxCyclesBeforeThinking,
		maxVelocity);
	return botClone;
}

/*
initBot - this initialization method sets up the basic bot
properties, but does not setup its velocity.
*/
void BossBot::initBot(unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// IF THE MAX IS SMALLER THAN THE MIN, SWITCH THEM
	if (initMax < initMin)
	{
		unsigned int temp = initMax;
		initMax = initMin;
		initMin = temp;
	}
	// IF THEY ARE THE SAME, ADD 100 CYCLES TO THE MAX
	else if (initMax == initMin)
		initMax += 100;

	// INIT OUR RANGE VARIABLES
	minCyclesBeforeThinking = initMin;
	maxCyclesBeforeThinking = initMax;

	// AND OUR VELOCITY CAPPER
	maxVelocity = 20;
	this->pp.setHealth(20);
	i = 0;


}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void BossBot::think(Game *game)
{
	x = (game->getGSM()->getSpriteManager()->getPlayer()->getBoundingVolume()->getCenterX() - this->getPhysicalProperties()->getX() - 128);
	y = (game->getGSM()->getSpriteManager()->getPlayer()->getBoundingVolume()->getCenterY() - this->getPhysicalProperties()->getY() - 128);
	dist = pow(x, 2) + pow(y, 2);

	if (this->pp.getHealth() <= 1)
	{
	}
	else
	{

		if (this->pp.getHealth() == 1)
		{
			this->setCurrentState(L"DYING");
		}
		if (dist <= 250000)
		{
			if (i < 80)
			{
				this->setCurrentState(L"IDLE");
			}
			else if (i < 130)
			{
				this->getPhysicalProperties()->setVelocity(15 * (x / sqrt(dist)), 15 * (y / sqrt(dist)));
				this->setCurrentState(L"WARP");
			}
			else if (i < 180)
			{
				this->pp.setVelocity(0, 0);
				this->setCurrentState(L"ATTACK");
				if (i > 150 && i < 170)
				{
					if (dist < 15000)
					{
						game->getGSM()->getSpriteManager()->getPlayer()->setCurrentState(L"DYING");
						game->getGSM()->getSpriteManager()->getPlayer()->getPhysicalProperties()->setVelocity(0, 0);
					}
				}
			}
			else
				i = 0;
			i++;
		}
	}



	//this->getPhysicalProperties()->setCollidable(false);
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO MOVE IN
	/*
	//if (sees != 0) // If he can't see player
	//{
	if (cyclesRemainingBeforeThinking == 0)
	{
	//if (this->wasOnTileLastFrame())
	//{

	//GameStateManager *gsm = game->getGSM();
	//pickRandomJump(gsm->getPhysics());
	//pickRandomCyclesInRange();

	}
	else
	//cyclesRemainingBeforeThinking--;
	//}
	animationRandomizer--;
	if (animationRandomizer == 0)
	{
	animationCounter++;
	animationRandomizer = (rand() % 45) + 5;
	}
	*/
}