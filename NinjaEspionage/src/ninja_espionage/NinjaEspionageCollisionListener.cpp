#include "sssf\game\Game.h"
#include "sssf\gsm\physics\Collision.h"
#include "ninja_espionage\NinjaEspionageCollisionListener.h"

#include "sssf\gsm\physics\Physics.h"

void NinjaEspionageCollisionListener::respondToCollision(Game *game, Collision *collision)
{
	// NOTE FROM THE COLLIDABLE OBJECTS, WHICH ARE IN THE COLLISION,
	// WE CAN CHECK AND SEE ON WHICH SIDE THE COLLISION HAPPENED AND
	// CHANGE SOME APPROPRIATE STATE ACCORDINGLY

	if (!collision->isCollisionWithTile())
	{
		CollidableObject *sprite = collision->getCO1();
		if (sprite->getCollisionEdge() == BOTTOM_EDGE)
		{
			// ENEMY IS DEAD - WE SHOULD PLAY A DEATH ANIMATION
			// AND MARK IT FOR REMOVAL
			//GameStateManager *gsm = game->getGSM();
			//Physics *physics = gsm->getPhysics();
			CollidableObject *bot = collision->getCO2();
			
			
		}
		else
		{
			// PLAYER IS DEAD - WE SHOULD PLAY A DEATH ANIMATION
			// AND MARK IT FOR REMOVAL/RESPAWN/RESTART GAME, WHATEVER
			// THE DEMANDS OF THE GAME ARE
		}
	}
}