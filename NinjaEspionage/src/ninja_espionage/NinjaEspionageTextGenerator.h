/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

NinjaEspionageTextGenerator.h

This class is a custom text generator for the EmptyGame
application. Basically, by defining the initText and updateText
methods, we can have it display whatever text we like.
*/

#pragma once
#include "ninja_espionage_VS\stdafx.h"
#include "sssf\text\TextGenerator.h"

class NinjaEspionageTextGenerator : public TextGenerator
{
private:
	// THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
	wstring textToGenerate;


public:
	// NOTHING TO INITIALIZE OR DESTROY
	NinjaEspionageTextGenerator()	{}
	~NinjaEspionageTextGenerator()	{}

	// DEFINED IN NinjaEspionageTextGenerator.cpp
	void initText(Game *game);
	void updateText(Game *game);
	void appendBotCount(Game *game);
};