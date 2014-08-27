#pragma once

#include "sssf\game\Game.h"
#include "sssf\gsm\physics\CollisionListener.h"
#include "sssf\gsm\physics\Collision.h"

class NinjaEspionageCollisionListener : public CollisionListener
{
public:
	void respondToCollision(Game *game, Collision *collision);
};