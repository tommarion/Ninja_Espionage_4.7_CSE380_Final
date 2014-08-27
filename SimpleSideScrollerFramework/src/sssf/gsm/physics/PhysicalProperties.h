#pragma once
#include "sssf_VS\stdafx.h"

class PhysicalProperties
{
protected:
	float buoyancy;
	float mass;
	bool  collidable;
	float coefficientOfRestitution;
	float x;
	float y;
	float z;
	float velocityX;
	float velocityY;
	float accelerationX;
	float accelerationY;
	int health = 1;
	int lives = 3;
	boolean sees = false;

	float xPos;
	float yPos;

public:
	PhysicalProperties()	{accelerationX = 0; accelerationY = 0;}
	~PhysicalProperties()	{}

	// INLINED METHODS
	float getBuoyancy()						{ return buoyancy;				}
	float getMass()							{ return mass;					}
	float getCoefficientOfRestitution()		{ return coefficientOfRestitution;	}
	bool isCollidable()						{ return collidable;			}
	float getX()							{ return x;						}
	float getY()							{ return y;						}
	float getZ()							{ return z;						}
	float getVelocityX()					{ return velocityX;				}
	float getVelocityY()					{ return velocityY;				}
	float getAccelerationX()				{ return accelerationX;			}
	float getAccelerationY()				{ return accelerationY;			}
	boolean getSees()						{ return sees;					}
	int getHealth()							{ return health;				}
	float getXPos()							{ return xPos;					}
	float getYPos()							{ return yPos;					}
	int getLives()							{ return lives;					}

	void setBuoyancy(float initBuoyancy)	{ buoyancy = initBuoyancy;		}
	void setMass(float initMass)			{ mass = initMass;				}
	void setCofficientOfRestitution(float cR) { coefficientOfRestitution = cR; }
	void setCollidable(bool initCollidable)	{ collidable = initCollidable;	}
	void setX(float initX)					{ x = initX;					}
	void incX(float xToInc)					{ x += xToInc;					}
	void setY(float initY)					{ y = initY;					}
	void incY(float yToInc)					{ y += yToInc;					}
	void setZ(float initZ)					{ z = initZ;					}
	void botHit()							{ health--;						}
	void setXPos(float x)					{ xPos = x;						}
	void setYPos(float y)					{ yPos = y;						}
	void decLives()							{ if (lives > 0) { lives--; }   }

	int round(float realNum)
	{
		return (int)floor(realNum + 0.5f);
	}

	void setPosition(float initX, float initY)
	{
		x = initX;
		y = initY;
	}

	void setVelocity(float initVx, float initVy)
	{
		velocityX = initVx;
		velocityY = initVy;
	}

	void incVelocity(float incVx, float incVy)
	{ 
		velocityX += incVx;
		velocityY += incVy;
	}
	void setAccelerationX(float initAx)		{ accelerationX = initAx;		}
	void setAccelerationY(float initAy)		{ accelerationY = initAy;		}

	float calcTotalVelocity()
	{
		float xSquared = x * x;
		float ySquared = y * y;
		float totalVelocity = xSquared + ySquared;
		totalVelocity = sqrtf(totalVelocity);
		return totalVelocity;
	}

	void applyAcceleration()
	{
		velocityX += accelerationX;
		velocityY += accelerationY;
	}

	void switchSees()
	{
		if (sees)
			sees = false;
		else
			sees = true;
	}

	void setHealth(int HP)
	{
		if (HP > 0)
			health = HP;
		else
			health = 0;
	}

};