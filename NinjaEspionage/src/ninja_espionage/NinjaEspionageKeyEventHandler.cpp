/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	NinjaEspionageKeyEventHandler.cpp

	See NinjaEspionageKeyEventHandler.h for a class description.
*/

#include "ninja_espionage_VS\stdafx.h"
#include "ninja_espionage\NinjaEspionage.h"
#include "ninja_espionage\NinjaEspionageKeyEventHandler.h"
#include "ninja_espionage\NinjaEspionageDataLoader.h"
#include "sssf\game\Game.h"
#include "sssf\game\WStringTable.h"
#include "sssf\graphics\GameGraphics.h"
#include "sssf\gsm\physics\Physics.h"
#include "sssf\gsm\physics\PhysicalProperties.h"
#include "sssf\gsm\sprite\AnimatedSprite.h"
#include "sssf\gsm\state\GameState.h"
#include "sssf\gsm\state\GameStateManager.h"
#include "sssf\gui\Cursor.h"
#include "sssf\gui\GameGUI.h"
#include "sssf\input\GameInput.h"
#include "sssf\timer\GameTimer.h"
#include "sssf\platforms\Windows\WindowsTimer.h"

/*
	handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
	gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
	key states since the last frame, which can allow us to respond to key presses, including when keys
	are held down for multiple frames.
*/
void NinjaEspionageKeyEventHandler::handleKeyEvents(Game *game)
{
	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	PhysicalProperties *pp = player->getPhysicalProperties();
	Viewport *viewport = game->getGUI()->getViewport();
	
	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		// WASD KEY PRESSES WILL CONTROL THE PLAYER
		// SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		// PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		float vX = pp->getVelocityX();
		float vY = pp->getVelocityY();
		bool viewportMoved = false;
		float viewportVx = 0.0f;
		float viewportVy = 0.0f;

		// YOU MIGHT WANT TO UNCOMMENT THIS FOR SOME TESTING,
		// BUT IN THIS ASSIGNMENT, THE USER MOVES VIA MOUSE BUTTON PRESSES
		if (player->getCurrentState().compare(DYING) != 0 && player->getCurrentState().compare(DEAD) != 0 && gsm->getSpriteManager()->getGameWon() == false) // If DEAD NO ACTIONS
		{
			if (input->isKeyDown(W_KEY) && input->isKeyDown(A_KEY)) // CHECK DIAGONALS FIRST TO ENSURE ANIMATIONS
			{
				vX = (float)(-PLAYER_SPEED * .7);
				viewportVx -= (float)(PLAYER_SPEED * .7);
				vY = (float)(-PLAYER_SPEED * .7);
				viewportVy -= (float)(PLAYER_SPEED * .7);
				player->setCurrentState(RUN_LEFT);
				player->setDirectionFacing(7);
				viewportMoved = true;
			}
			else if (input->isKeyDown(W_KEY) && input->isKeyDown(D_KEY))
			{
				vX = (float)(PLAYER_SPEED * .7);
				viewportVx += (float)(PLAYER_SPEED * .7);
				vY = (float)(-PLAYER_SPEED * .7);
				viewportVy -= (float)(PLAYER_SPEED * .7);
				player->setCurrentState(RUN_RIGHT);
				player->setDirectionFacing(9);
				viewportMoved = true;
			}
			else if (input->isKeyDown(S_KEY) && input->isKeyDown(A_KEY))
			{
				vX = (float)(-PLAYER_SPEED * .7);
				viewportVx -= (float)(PLAYER_SPEED * .7);
				vY = (float)(PLAYER_SPEED * .7);
				viewportVy += (float)(PLAYER_SPEED * .7);
				player->setCurrentState(RUN_LEFT);
				player->setDirectionFacing(1);
				viewportMoved = true;
			}
			else if (input->isKeyDown(S_KEY) && input->isKeyDown(D_KEY))
			{
				vX = (float)(PLAYER_SPEED * .7);
				viewportVx += (float)(PLAYER_SPEED * .7);
				vY = (float)(PLAYER_SPEED * .7);
				viewportVy += (float)(PLAYER_SPEED * .7);
				player->setCurrentState(RUN_RIGHT);
				player->setDirectionFacing(3);
				viewportMoved = true;
			}

			else // IF NOT DIAGONAL THEN CHECK OTHER KEYS
			{
				if (input->isKeyDown(A_KEY))
				{
					if (!input->isKeyDown(D_KEY))
					{
						vX = -PLAYER_SPEED;
						player->setCurrentState(RUN_LEFT);
						player->setDirectionFacing(4);
						viewportVx -= PLAYER_SPEED;
						viewportMoved = true;
					} if (input->isKeyDown(D_KEY)){ viewport->setXPass(false); }
				}
				else  { if (!input->isKeyDown(D_KEY)) { vX = 0; } }

				if (input->isKeyDown(W_KEY))
				{
					if (!input->isKeyDown(S_KEY))
					{
						vY = -PLAYER_SPEED;
						player->setCurrentState(RUN_UP);
						player->setDirectionFacing(8);
						viewportVy -= PLAYER_SPEED;
						viewportMoved = true;
					}
					else { viewport->setYPass(false); }
				}
				else  { if (!input->isKeyDown(S_KEY)) { vY = 0; } }

				if (input->isKeyDown(D_KEY))
				{
					if (!input->isKeyDownForFirstTime(A_KEY))
					{
						vX = PLAYER_SPEED;
						player->setCurrentState(RUN_RIGHT);
						player->setDirectionFacing(6);
						viewportVx += PLAYER_SPEED;
						viewportMoved = true;
					}	if (input->isKeyDown(A_KEY)){ viewport->setXPass(false); }
				}

				if (input->isKeyDown(S_KEY))
				{
					vY = PLAYER_SPEED;
					player->setCurrentState(RUN_DOWN);
					player->setDirectionFacing(2);
					viewportVy += PLAYER_SPEED;
					viewportMoved = true;
				}
				if (input->isKeyDownForFirstTime(G_KEY))
				{
					int x = (int)player->getPhysicalProperties()->getX();
					int y = (int)player->getPhysicalProperties()->getY();
					game->getGSM()->getSpriteManager()->setBotCount(game->getGSM()->getSpriteManager()->getBotCount() - 1);

					//viewport->toggleDebugView();
					//game->getGraphics()->toggleDebugTextShouldBeRendered();
					x = y;
				}
				if (!input->isKeyDown(W_KEY) && !input->isKeyDown(A_KEY) && !input->isKeyDown(S_KEY) && !input->isKeyDown(D_KEY))
				{
					vX = 0.0f;
					vY = 0.0f;
					player->setCurrentState(IDLE);
					viewportMoved = true;
				}
				if (input->isKeyDown(W_KEY) && input->isKeyDown(S_KEY) || input->isKeyDown(A_KEY) && input->isKeyDown(D_KEY))
				{
					vX = 0.0f;
					vY = 0.0f;
					player->setCurrentState(IDLE);
					viewportMoved = true;
				}
			}
			if (input->isKeyDownForFirstTime(J_KEY))
			{
				if (player->isCurrentlyCollidable() && player->getPhysicalProperties()->isCollidable())
				{
					player->setCurrentlyCollidable(false);
					player->getPhysicalProperties()->setCollidable(false);
				}
				else
				{
					player->setCurrentlyCollidable(true);
					player->getPhysicalProperties()->setCollidable(true);
				}
			}

			if (input->isKeyDownForFirstTime(K_KEY) && input->isKeyDown(VK_SHIFT)) // KILL ALL SHIFT + 'K'
			{
				gsm->getSpriteManager()->setKillAll(true);
			}


			if (input->isKeyDownForFirstTime(ONE_KEY) || input->isKeyDownForFirstTime(M_KEY))
			{
				SpriteManager *spriteManager = gsm->getSpriteManager();
				if (spriteManager->getStarsThrown() < 2)
				{
					NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *) game->getDataLoader();
					if (spriteManager->getStarsThrown() == 1)
						nedl->attack1Cool();

					AnimatedSpriteType *star = spriteManager->getSpriteType(2);

					PlaySound(LPCW_ATTACK_1, NULL, SND_FILENAME | SND_ASYNC);

					int num = player->getDirectionFacing();
					if (input->isKeyDown(VK_SHIFT)){
						num = 10 - num;
					}
					spriteManager->incrementStars();
					switch (num)
					{
					case 1:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 22, -17, 17, false);
						break;
					case 2:
						nedl->makeNinjaStar(game, star, pp->getX() + 6, pp->getY() + 32, 0, 25, false);
						break;
					case 3:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 17, 17, false);
						break;
					case 4:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 9, -25, 0, false);
						break;
					case 6:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 9, 25, 0, false);
						break;
					case 7:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -17, -17, false);
						break;
					case 8:
						nedl->makeNinjaStar(game, star, pp->getX() + 6, pp->getY() + 32, 0, -25, false);
						break;
					case 9:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 12, 17, -17, false);
						break;
					default:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 9, 25, 0, false);
					}
				}
			}
			if (input->isKeyDownForFirstTime(N_KEY))
			{
				SpriteManager *spriteManager = gsm->getSpriteManager();
				if (triCool == 0)
				{
					NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
					triCool = 120;

					AnimatedSpriteType *star = spriteManager->getSpriteType(2);

					PlaySound(LPCW_ATTACK_3, NULL, SND_FILENAME | SND_ASYNC);
					nedl->attack3Cool();
					int num = player->getDirectionFacing();
					if (input->isKeyDown(VK_SHIFT)){
						num = 10 - num;
					}
					//spriteManager->incrementStars();
					switch (num)
					{
					case 1:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -17, 17, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -20, 15, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -15, 20, false);
						break;
					case 2:
						nedl->makeNinjaStar(game, star, pp->getX() + 8, pp->getY() + 10, 0, 25, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 8, pp->getY() + 10, 5, 20, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 8, pp->getY() + 10, -5, 20, false);
						break;
					case 3:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 17, 17, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 20, 15, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 15, 20, false);
						break;
					case 4:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, -25, 0, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, -20, 5, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, -20, -5, false);
						break;
					case 6:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, 25, 0, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, 20, 5, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, 20, -5, false);
						break;
					case 7:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -17, -17, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -15, -20, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, -20, -15, false);
						break;
					case 8:
						nedl->makeNinjaStar(game, star, pp->getX() + 8, pp->getY() + 10, 0, -25, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 8, pp->getY() + 10, 3, -20, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 8, pp->getY() + 10, -3, -20, false);
						break;
					case 9:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 17, -17, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 15, -20, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 32, 20, -15, false);
						break;
					default:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, 25, 0, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, 20, 5, false);
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() + 10, 20, -5, false);
					}
				}
			}

			if (input->isKeyDownForFirstTime(L_KEY))
			{

				SpriteManager *spriteManager = gsm->getSpriteManager();
				if (fatCool == 0)
				{
					NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
					fatCool = 150;
					nedl->attack4Cool();
					AnimatedSpriteType *star = spriteManager->getSpriteType(2);

					PlaySound(LPCW_ATTACK_2, NULL, SND_FILENAME | SND_ASYNC);
					nedl->attack3Cool();
					int num = player->getDirectionFacing();
					if (input->isKeyDown(VK_SHIFT)){
						num = 10 - num;
					}
					//spriteManager->incrementStars();
					switch (num)
					{
					case 1:
						nedl->makeNinjaStar(game, star, pp->getX() - 20, pp->getY() + 12, -17, 17, true);
						break;
					case 2:
						nedl->makeNinjaStar(game, star, pp->getX() - 10, pp->getY() + 10, 0, 25, true);
						break;
					case 3:
						nedl->makeNinjaStar(game, star, pp->getX() + 10, pp->getY() + 32, 17, 17, true);
						break;
					case 4:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() - 8, -25, 0, true);
						break;
					case 6:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() - 8, 25, 0, true);
						break;
					case 7:
						nedl->makeNinjaStar(game, star, pp->getX() + 10, pp->getY() + 32, -17, -17, true);
						break;
					case 8:
						nedl->makeNinjaStar(game, star, pp->getX() - 10, pp->getY() + 10, 0, -25, true);
						break;
					case 9:
						nedl->makeNinjaStar(game, star, pp->getX() - 20, pp->getY() + 12, 17, -17, true);
						break;
					default:
						nedl->makeNinjaStar(game, star, pp->getX() + 20, pp->getY() - 8, 25, 0, true);
					}
				}
			}
			if (input->isKeyDownForFirstTime(P_KEY))
			{
				gsm->getPhysics()->togglePhysics();
			}


			if (input->isKeyDownForFirstTime(T_KEY)) //TELEPORT KEY

			{
				GameStateManager *gsm = game->getGSM();
				Physics *physics = gsm->getPhysics();
				physics->removeCollidableObjectBot(player); //remove

				SpriteManager *sm = gsm->getSpriteManager();
				AnimatedSprite *player = sm->getPlayer();
				physics->addCollidableObject(player);
				AnimatedSpriteType *playerSpriteType = sm->getSpriteType(1);
				player->setSpriteType(playerSpriteType);
				player->setAlpha(255);
				player->setCurrentState(IDLE);
				PhysicalProperties *playerProps = player->getPhysicalProperties();
				playerProps->setX(900);					//SET NEW X
				playerProps->setY(1000);				//SET NEW Y
				playerProps->setVelocity(0.0f, 0.0f);
				playerProps->setAccelerationX(0);
				playerProps->setAccelerationY(0);
				player->setOnTileThisFrame(false);
				player->setOnTileLastFrame(false);
				player->setJumpedOnce(false);
				player->affixTightAABBBoundingVolume();
			}

			if ((input->isKeyDown(ZERO_KEY)) || input->isKeyDown(SPACE_KEY))
			{
				if ((blinkCool == 0 && blinkLength < 5))
				{
					NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
					nedl->attack2Cool();
					player->setCurrentlyCollidable(false);
					PlaySound(LPCW_BLINK, NULL, SND_FILENAME | SND_ASYNC);
					PhysicalProperties *pp = player->getPhysicalProperties();
					// TODO: BLINK
					switch (player->getDirectionFacing())
					{
					case 1: // lower left
						vY = PLAYER_SPEED * 5;
						vX = -PLAYER_SPEED * 5;
						break;
					case 2: // down
						vY = PLAYER_SPEED * 8;
						break;
					case 3: // lower right
						vY = PLAYER_SPEED * 5;
						vX = PLAYER_SPEED * 5;
						break;
					case 4: // left
						vX = -PLAYER_SPEED * 8;
						break;
					case 6: // right
						vX = PLAYER_SPEED * 8;
						break;
					case 7:// upper left
						vX = -PLAYER_SPEED * 5;
						vY = -PLAYER_SPEED * 5;
						break;
					case 8: // up
						vY = -PLAYER_SPEED * 8;
						break;
					case 9: // upper right
						vY = -PLAYER_SPEED * 5;
						vX = PLAYER_SPEED * 5;
						break;
					default:
						return;
					}
					blinkLength++;
				}
				else {
					if (blinkCool == 0)
					{
						player->setCurrentlyCollidable(true);
						blinkCool = 100;
						blinkLength = 0;
					}
				}

			}
			else {

				if (blinkLength > 0)
				{
					player->setCurrentlyCollidable(true);
					blinkCool = 60 + (40 / (6 - blinkLength));
					blinkLength = 0;
				}
			}

			if (blinkCool > 0)
				blinkCool--;
			else if (blinkCool == 0 && blinkLength == 0){
				NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
				nedl->attack2Idle();
			}

			if (triCool > 0)
			{
				triCool--;
			} else {
				NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
				nedl->attack3Idle();
			}

			if (fatCool > 0)
			{
				fatCool--;
			}
			else {
				NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
				nedl->attack4Idle();
			}



			// NOW SET THE ACTUAL PLAYER VELOCITY
			pp->setVelocity(vX, vY);


			if (input->isKeyDown(UP_KEY))
			{
				viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
				viewportMoved = true;
			}
			if (input->isKeyDown(DOWN_KEY))
			{
				viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
				viewportMoved = true;
			}
			if (input->isKeyDown(LEFT_KEY))
			{
				viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
				viewportMoved = true;
			}
			if (input->isKeyDown(RIGHT_KEY))
			{
				viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
				viewportMoved = true;
			}
			float playerX = pp->getX();
			float playerY = pp->getY();
			Viewport *viewport = game->getGUI()->getViewport();
			if (viewportMoved)
				viewport->moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f), (int)game->getGSM()->getWorld()->getWorldWidth(), (int)game->getGSM()->getWorld()->getWorldHeight(), (int)playerX, (int)playerY, (int)vX, (int)vY, player->getCurrentState());
		}
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if (input->isKeyDownForFirstTime(ONE_KEY2)
		&& input->isKeyDown(VK_SHIFT))
	{
		if (game->getGSM()->isGameInProgress())
		{
			game->wonGame();
		}
		else if (game->getGSM()->isGameWon())
		{
			game->getGSM()->goToGame();
		}
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	WindowsTimer *timer = (WindowsTimer*)game->getTimer();
	int fps = timer->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_F2) && (fps < MAX_FPS))
		timer->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_F1) && (fps > MIN_FPS))
		timer->setTargetFPS(fps - FPS_INC);
}

void NinjaEspionageKeyEventHandler::cooldownControl(Game *game, int i)
{
	if (i == 1)
	{
		NinjaEspionageDataLoader *nedl = (NinjaEspionageDataLoader *)game->getDataLoader();
	}
}