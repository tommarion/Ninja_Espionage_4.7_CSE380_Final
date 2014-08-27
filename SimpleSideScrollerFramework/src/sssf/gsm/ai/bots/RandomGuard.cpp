#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\bots\RandomGuard.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gsm\physics\Physics.h"

/*
This private constructor is only to be used for cloning bots, note
that this does not setup the velocity for this bot.
*/
RandomGuard::RandomGuard(unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);

	cyclesRemainingBeforeThinking = 30;
	pp.setVelocity(0.0f, 0.0f);

	pickRandomCyclesInRange();
}

/*
This is the public constructor used by other classes for
creating these types of bots.
*/
RandomGuard::RandomGuard(Physics *physics,
	unsigned int initMin,
	unsigned int initMax,
	unsigned int initMaxVelocity)
{
	// INIT THE BASIC STUFF
	initBot(initMin, initMax, initMaxVelocity);
	this->pp.setVelocity(0, 0);
	pickRandomJump(physics);
	pickRandomCyclesInRange();


}

/*
clone - this method makes another RandomGuard object, but does
not completely initialize it with similar data to this. Most of the
object, like velocity and position, are left uninitialized.
*/
Bot* RandomGuard::clone()
{
	RandomGuard *botClone = new RandomGuard(minCyclesBeforeThinking,
		maxCyclesBeforeThinking,
		maxVelocity);
	return botClone;
}

/*
initBot - this initialization method sets up the basic bot
properties, but does not setup its velocity.
*/
void RandomGuard::initBot(unsigned int initMin,
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
	maxVelocity = 4;

	i = rand() % 4;
	xVals[0] = 0;
	xVals[1] = -100;
	xVals[2] = 0;
	xVals[3] = 100;

	yVals[0] = -100;
	yVals[1] = 0;
	yVals[2] = 100;
	yVals[3] = 0;


	// AND WE'LL USE THIS TO ENSURE OUR BOTS ALL LOOK A LITTLE DIFFERENT
	//animationRandomizer = (rand() % 45) + 5;
}

/*
pickRandomCyclesInRange - a randomized method for determining when this bot
will think again. This method sets that value.
*/
void RandomGuard::pickRandomCyclesInRange()
{
	cyclesRemainingBeforeThinking = maxCyclesBeforeThinking - minCyclesBeforeThinking + 1;
	cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
	cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
}

/*
pickRandomVelocity - calculates a random velocity vector for this
bot and initializes the appropriate instance variables.
*/
void RandomGuard::pickRandomJump(Physics *physics)
{
	float jumpVelocity = (float)maxVelocity;
	float jumpVelocityX;
	float jumpVelocityY;

	if (i > 3)
		i = 0;
	if (i == 0) //RIGHT
	{
		this->setCurrentState(L"RUN_RIGHT");
		jumpVelocityX = jumpVelocity;
		jumpVelocityY = 0;
	} else if (i == 1) { //UP
		this->setCurrentState(L"RUN_LEFT");
		jumpVelocityX = 0;
		jumpVelocityY = -jumpVelocity;
	} else if (i == 2) { //LEFT
		this->setCurrentState(L"RUN_LEFT");
		jumpVelocityX = -jumpVelocity;
		jumpVelocityY = 0;
	} else if (i == 3) { //DOWN
		this->setCurrentState(L"RUN_RIGHT");
		jumpVelocityX = 0;
		jumpVelocityY = jumpVelocity;
	}

	i++;
	
	pp.setVelocity(jumpVelocityX, jumpVelocityY);
	//	pp.setVelocity(0.0f, jumpVelocity);
}

/*
think - called once per frame, this is where the bot performs its
decision-making. Note that we might not actually do any thinking each
frame, depending on the value of cyclesRemainingBeforeThinking.
*/
void RandomGuard::think(Game *game)
{
	// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
	// TO PICK A DIFFERENT DIRECTION TO MOVE IN
	GameStateManager *gsm = game->getGSM();
	SpriteManager *sm = gsm->getSpriteManager();

	if (sm->getGameWon())
	{
		sm->setBotCount(sm->getBotCount() - 1);
		sm->removeBot(this);
		gsm->getPhysics()->removeCollidableObjectBot(this);
		return;
	}

	if (sm->getKillAll())
	{
		sm->setBotCount(sm->getBotCount() - 1);
		this->setCurrentState(L"DYING");
		this->getPhysicalProperties()->setVelocity(0, 0);
	}


	if (getCurrentState().compare(L"DEAD") != 0 && getCurrentState().compare(L"DYING") != 0)
	{
		/// PATROL MOVEMENT //////////////////
		if (cyclesRemainingBeforeThinking == 0)
		{
			pickRandomJump(gsm->getPhysics());
			pickRandomCyclesInRange();
		}
		else
			cyclesRemainingBeforeThinking--;

		animationRandomizer--;
		if (animationRandomizer == 0)
		{
			animationCounter++;
			animationRandomizer = (rand() % 45) + 5;
		}

		//////////////////////////////


		/////// Collision Testing ////////////////

			//CHECK HOW FAR THE PLAYER IS FROM THE GUARD
			float xCheck = sm->getPlayer()->getPhysicalProperties()->getX() - this->getPhysicalProperties()->getX();
			float yCheck = sm->getPlayer()->getPhysicalProperties()->getY() - this->getPhysicalProperties()->getY();

			if ((xCheck < 500 && xCheck > -500) && (yCheck < 500 && yCheck > -500) && (sm->getStartCounter() > 1000)) // if the start counter is past 1000, and the
																													  // guard is within range, check for collisions and 
																													  // if the guard sees the player
			{
				AABB *playBB = sm->getPlayer()->getSweptShape();
				tempBB->setCenterX(playBB->getCenterX());
				tempBB->setCenterY(playBB->getCenterY());
				tempBB->setHeight(playBB->getHeight() + 500);
				tempBB->setWidth(playBB->getWidth() + 500);

				AABB *botBB = this->getSweptShape();
				float botH = botBB->getHeight();
				float botW = botBB->getWidth();
				float playH = playBB->getHeight();
				float playW = playBB->getWidth();
				botBB->setHeight(botH);
				botBB->setWidth(botW);

				if (!sm->getDead() && playBB->overlaps(botBB)) // if player is not dead and they collide
				{

					tempBB->setCenterX(playBB->getCenterX());
					tempBB->setCenterY(playBB->getCenterY());
					tempBB->setHeight(playBB->getHeight() - 20);
					tempBB->setWidth(playBB->getWidth() - 15);

					if (sm->getPlayer()->getPhysicalProperties()->isCollidable() && tempBB->overlaps(botBB)) // if the guard hits the player
					{
						// death collision

						sm->getPlayer()->setCurrentlyCollidable(false);
						sm->getPlayer()->getPhysicalProperties()->setCollidable(false);
						sm->setDead(true);
						sm->setLives(sm->getLives()-1); // decrement lives
						sm->getPlayer()->setCurrentState(L"DYING");
						sm->getPlayer()->getPhysicalProperties()->setVelocity(0, 0);
					}
				}

				botBB->setHeight(botH);
				botBB->setWidth(botW);

				if (sm->getStartCounter() > 1000)
				{
					if (sm->getDead()) // if the player is dead, go back to your original starting point
					{
						this->getPhysicalProperties()->switchSees();
						float diffX = this->getPhysicalProperties()->getXPos() - this->getPhysicalProperties()->getX();
						float diffY = this->getPhysicalProperties()->getYPos() - this->getPhysicalProperties()->getY();
						float distance = sqrt((diffX * diffX) + (diffY * diffY));
						float unitX = diffX / distance;
						float unitY = diffY / distance;
						float vX;
						float vY;

						if (distance > 5)
						{
							vX = unitX * 10;
							vY = unitY * 10;
						}
						else {
							vX = 0;
							vY = 0;
						}

						if (diffX >= 0.0f)
							this->setCurrentState(L"RUN_RIGHT");
						else
							this->setCurrentState(L"RUN_LEFT");

						this->getPhysicalProperties()->setVelocity(vX, vY);
					}


					else if (this->getPhysicalProperties()->getSees() == false && tempBB->overlaps(botBB) == true) // if the guard does not see, and player is within range
					{
						PlaySound(LPCW_BOT_SEES, NULL, SND_FILENAME | SND_ASYNC);	// play laugh
						this->getPhysicalProperties()->switchSees();				// then switch to bot sees
					}
					else if (this->getPhysicalProperties()->getSees()) // if the guard sees, set to follow the player
					{
						float diffX = sm->getPlayer()->getPhysicalProperties()->getX() - this->getPhysicalProperties()->getX();
						float diffY = sm->getPlayer()->getPhysicalProperties()->getY() - this->getPhysicalProperties()->getY();
						float distance = sqrt((diffX * diffX) + (diffY * diffY));
						float unitX = diffX / distance;
						float unitY = diffY / distance;
						float vX;
						float vY;

						vX = unitX * 10;
						vY = unitY * 10;

						if (diffX >= 0) //if the diff is positive, the player is to the right
							this->setCurrentState(L"RUN_RIGHT");
						else
							this->setCurrentState(L"RUN_LEFT");

						this->getPhysicalProperties()->setVelocity(vX, vY);
					}
				}
				tempBB->~AABB();
				playBB->~AABB();
				botBB->~AABB();
			}
	}

	else // if the guard is dead, continue the timer
	{
		int i = this->getDeadTimer();
		if (i >= 25)
		{
			sm->removeBot(this);
			gsm->getPhysics()->removeCollidableObjectBot(this);
		}
	}
}