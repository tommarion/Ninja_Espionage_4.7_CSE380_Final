#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\RandomObject.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
	This private constructor is only to be used for cloning bots, note
	that this does not setup the velocity for this bot.
*/
RandomObject::RandomObject(	unsigned int initMin, 
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
RandomObject::RandomObject(	Physics *physics,
										unsigned int initMin, 
										unsigned int initMax, 
										unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);

	// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
	// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
	pickRandomVelocity(physics);
	pickRandomCyclesInRange();
}

/*
	clone - this method makes another RandomObject object, but does
	not completely initialize it with similar data to this. Most of the 
	object, like velocity and position, are left uninitialized.
*/
Bot* RandomObject::clone()
{
	RandomObject *botClone = new RandomObject(	minCyclesBeforeThinking, 
															maxCyclesBeforeThinking, 
															maxVelocity);
	return botClone;
}

/*
	initBot - this initialization method sets up the basic bot
	properties, but does not setup its velocity.
*/
void RandomObject::initBot(	unsigned int initMin,
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
	maxVelocity = initMaxVelocity;
}

/*
	pickRandomCyclesInRange - a randomized method for determining when this bot
	will think again. This method sets that value.
*/
void RandomObject::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/*
	pickRandomVelocity - calculates a random velocity vector for this
	bot and initializes the appropriate instance variables.
*/
void RandomObject::pickRandomVelocity(Physics *physics)
{
	// FIRST GET A RANDOM float FROM 0.0 TO 1.0
	float randomAngleInRadians = ((float)rand())/((float)RAND_MAX);

	// NOW SCALE IT FROM 0 TO 2 PI
	randomAngleInRadians *= 2.0f;
	randomAngleInRadians *= PI;

	// NOW WE CAN SCALE OUR X AND Y VELOCITIES
	this->pp.setVelocity(maxVelocity * sin(randomAngleInRadians), maxVelocity * cos(randomAngleInRadians));
}

/*
	think - called once per frame, this is where the bot performs its
	decision-making. Note that we might not actually do any thinking each
	frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void RandomObject::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

	SpriteManager *sm = game->getGSM()->getSpriteManager();

	///////////// First Check the GUI Sprites //////////////

	if (sm->getStarsThrown() == 0 && this->getCurrentState().compare(L"COOLDOWN1") == 0)
	{
		this->setCurrentState(L"IDLE1");
	}

	if (this->getSpriteType()->getSpriteTypeID() == 4)		// lives animation Counter changing
	{
		switch (sm->getLives())
		{
		case 1:
			this->setCurrentState(L"LIVES1");
			break;
		case 2:
			this->setCurrentState(L"LIVES2");
			break;
		case 3:
			this->setCurrentState(L"LIVES3");
			break;
		default:
			break;
		}
	}

	/////////////////////////////////////////////////////////


	/////////// Now Check the Gates /////////////////////////

	if (this->getSpriteType()->getSpriteTypeID() == 5) // if its a gate
	{
		if (sm->getBotCount() == 0) // if the number of bots left = 0    open the gate
		{
			float diffX = 1792 - this->getPhysicalProperties()->getX();
			float vX = 0.0;
			if (diffX <= 7.0)
				float unitX = 0;
			else
			{
				float distance = sqrt(diffX * diffX);
				float unitX = diffX / distance;
				vX = unitX * 10;
			}
			this->getPhysicalProperties()->setVelocity(vX, 0);
		}

		
		AABB *playBB = sm->getPlayer()->getBoundingVolume();

		tempBB->setCenterX(playBB->getCenterX());
		tempBB->setCenterY(playBB->getCenterY());
		tempBB->setHeight(playBB->getHeight());
		tempBB->setWidth(playBB->getWidth());

		AABB *botBB = this->getSweptShape();
		float botH = botBB->getHeight();
		float botW = botBB->getWidth();
		float playH = playBB->getHeight();
		float playW = playBB->getWidth();

		if (sm->getPlayer()->getPhysicalProperties()->isCollidable() && tempBB->overlaps(botBB) == true)
		{
			gateCheck(this, sm);
		}

	}
}
void RandomObject::gateCheck(Bot* bot, SpriteManager *sm)
{
	// MOVES PLAYER AWAY FROM GATE
	// A LITTLE BUGGY IF YOU REALLY TRY TO GET THROUGH
	// BY BLINKING AND RUNNING BACKWARDS
	// ....BUT IT IS FUNCTIONAL
	int direction = sm->getPlayer()->getDirectionFacing();

	switch (direction)
	{
	case 1:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(5, -5);
		break;
	case 2:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(0, -5);
		break;
	case 3:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(-5, -5);
		break;
	case 4:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(5, 0);
		break;
	case 6:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(-5, 0);
		break;
	case 7:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(5, 5);
		break;
	case 8:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(0, 5);
		break;
	case 9:
		sm->getPlayer()->getPhysicalProperties()->setVelocity(-5, 5);
		break;
	default:
		break;
	}
}