#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\NinjaStar.h"
#include "sssf\gsm\ai\botRecycler.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
This private constructor is only to be used for cloning bots, note
that this does not setup the velocity for this bot.
*/
NinjaStar::NinjaStar(unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
}

/*
This is the public constructor used by other classes for
creating these types of bots.
*/
NinjaStar::NinjaStar(Physics *physics,
	unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
	this->pp.setVelocity(0, 0);

}

/*
clone - this method makes another RandomGuard object, but does
not completely initialize it with similar data to this. Most of the
object, like velocity and position, are left uninitialized.
*/
Bot* NinjaStar::clone()
{
	NinjaStar *botClone = new NinjaStar(minCyclesBeforeThinking,
		maxCyclesBeforeThinking,
		maxVelocity);
	return botClone;
}

/*
initBot - this initialization method sets up the basic bot
properties, but does not setup its velocity.
*/

void NinjaStar::initBot(unsigned int initMin, unsigned int initMax, unsigned int initMaxVelocity)
{
	i = 0;
}

void NinjaStar::think(Game *game)
{
	SpriteManager *sm = game->getGSM()->getSpriteManager();
	Physics * physics = game->getGSM()->getPhysics();
	int maxDis = 35;
	if (getCurrentState().compare(L"IDLE"))
		maxDis = 28;

	if (i > maxDis)
	{
		i = 0;
		sm->removeBot(this);
		physics->removeCollidableObjectBot(this);
		sm->decrementStars();
		/*if (sm->getStarsThrown() == 0)
		{
			
		}*/
	}
	i++;

	list<Bot*>::iterator botIterator;
	botIterator = sm->getBotsIterator();
	while (botIterator != sm->getEndOfBotsIterator())
	{
		Bot *bot = (*botIterator);
		if (bot->getSpriteType()->getSpriteTypeID() == 0 || bot->getSpriteType()->getSpriteTypeID() == 6)
		{
			AABB *botAABB = bot->getSweptShape();
			AABB *starAABB = this->getSweptShape();
			starAABB->setHeight(starAABB->getHeight() - 15);
			starAABB->setWidth(starAABB->getWidth() -15);

			// Compare each bot's location to the star location. If they're within 'x'
			// range of eachother, the star would be removed as well as the bot (by way of dying animation,
			// unless we want this as a separate function).
			if (starAABB->overlaps(botAABB) == true && bot->getCurrentState().compare(L"DYING") != 0 && bot->getCurrentState().compare(L"DEAD") != 0)
			{
				if (bot->getPhysicalProperties()->getHealth() > 0)
				{
					bot->getPhysicalProperties()->botHit();
					PlaySound(LPCW_BOT_HIT, NULL, SND_FILENAME | SND_ASYNC);
					bot->getPhysicalProperties()->switchSees();

				} else {
					sm->setBotCount(sm->getBotCount()-1);
					PlaySound(LPCW_BOT_DIE, NULL, SND_FILENAME | SND_ASYNC);
					bot->setCurrentState(L"DYING");
					bot->getPhysicalProperties()->setVelocity(0, 0);
				}
				if (getCurrentState().compare(L"IDLE") == 0)
				{
					sm->removeBot(this);
					physics->removeCollidableObjectBot(this);
					sm->decrementStars();
				}
				else if (bot->getSpriteType()->getSpriteTypeID() == 6)
				{
					this->setCurrentlyCollidable(false);
				}

					starAABB->setHeight(starAABB->getHeight() + 15);
					starAABB->setWidth(starAABB->getWidth() + 15);
					return;
			}

			starAABB->setHeight(starAABB->getHeight() + 15);
			starAABB->setWidth(starAABB->getWidth() + 15);

		}
		botIterator++;
	}
}

