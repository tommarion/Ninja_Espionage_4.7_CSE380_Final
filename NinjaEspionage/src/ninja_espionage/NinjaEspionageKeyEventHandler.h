/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaEspionageKeyEventHandler.h

	This is a custom key event handler. Each game can choose
	to use different keyboard keys and respond differently to when
	they are pressed. This class will handle key presses for
	this particular game following the KeyEventHandler pattern.
*/

#pragma once
#include "ninja_espionage_VS\stdafx.h"
#include "sssf\input\KeyEventHandler.h"

class Game;

class NinjaEspionageKeyEventHandler : public KeyEventHandler
{
public:
	int blinkLength = 0;
	int blinkCool = 0;
	int triCool = 0;
	int fatCool = 0;

	NinjaEspionageKeyEventHandler()		{}
	~NinjaEspionageKeyEventHandler()		{}
	void handleKeyEvents(Game *game);

	void cooldownControl(Game *game, int i);
};