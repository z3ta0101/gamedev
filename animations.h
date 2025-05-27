#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

//Directional rotation
        if (sf::Keyboard::isKeyPressed(Keyboard::W) && playerHealth > 0)
		{
			direction = 5;
			idle = 0;
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::D) && playerHealth > 0)
		{
			direction = 7;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::S) && playerHealth > 0)
		{
			direction = 1;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::A) && playerHealth > 0)
		{
			direction = 3;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::W) && (Keyboard::isKeyPressed(sf::Keyboard::A)) && playerHealth > 0)
		{
			direction = 4;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::W) && (sf::Keyboard::isKeyPressed(Keyboard::D)) && playerHealth > 0)
		{
			direction = 6;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::D) && (sf::Keyboard::isKeyPressed(Keyboard::S)) && playerHealth > 0)
		{
			direction = 8;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::S) && (sf::Keyboard::isKeyPressed(Keyboard::A)) && playerHealth > 0)
		{
			direction = 2;
			idle = 0;
			
		}

		//Death animations

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 1){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3048;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 2){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3175;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 3){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3302;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 4){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3429;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 5){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3556;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 6){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3683;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 7){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3810;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 8){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3937;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		//Walking animations
		
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 1 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 0;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
        if (clock.getElapsedTime().asSeconds() > 0.1f && direction == 2 && idle == 0)
        {
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 156;
			clothesSourceSprite.height = 156;
			clothesSourceSprite.width = 156;
			clothesSourceSprite.top = 156;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			if (clothesSourceSprite.left >= 2340)
				clothesSourceSprite.left = 0;
			else
				clothesSourceSprite.left += 156;
			
			player.setTextureRect(rectSourceSprite);
			playerclothes.setTextureRect(clothesSourceSprite);
			clock.restart();
        }
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 3 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 312;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1f && direction == 4 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 468;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 5 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 624;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 6 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 780;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 7 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 936;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 8 && idle == 0)
		{
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 1092;
			isMoving = true;
			if (rectSourceSprite.left >= 1404)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 156;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		

		//Idle animations
		if (event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::W)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::A)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::S)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::D)
			{
				
				isMoving = false;
				
			}

		if (idle == 1 && direction == 1 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 2496;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 2 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 2652;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 3 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 2808;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 4 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 2964;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 5 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 3120;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 6 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 3276;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 7 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 3432;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 8 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 156;
			rectSourceSprite.width = 156;
			rectSourceSprite.top = 3588;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 156)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 0;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		}

		//Attack animations
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asSeconds() > 0.1)
        {
			
			std::cout << "PositionX: " << playerCenter.x << std::endl;
			std::cout << "PositionY: " << playerCenter.y << std::endl;
			if (direction == 1)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 1248;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
					
				}
			    else
			        rectSourceSprite.left += 156;

				if (rectSourceSprite.left == 0 && round == true){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
					
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 2)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 1404;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 3)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 1560;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 4)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 1716;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 5)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 1872;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 6)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 2028;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 7)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 2184;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 8)
			{
				rectSourceSprite.height = 156;
				rectSourceSprite.width = 156;
			    rectSourceSprite.top = 2340;
			    if (rectSourceSprite.left >= 2184){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 156;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			
	    
        }

		if (fireplace && fireclock.getElapsedTime().asSeconds() > 0.2){
			if (fjelhearthSourceSprite.left >= 2478){
				fjelhearthSourceSprite.left = 0;
			}

			else{
				fjelhearthSourceSprite.left += 414;
			}
			fjelfireplace.setTextureRect(fjelhearthSourceSprite);
			fireclock.restart();
		}

		if (barrelhealth <= 0 && clock.getElapsedTime().asSeconds() > 0.1)
		{
			
			barrelxSourceSprite.left += 100;
			if (barrelxSourceSprite.left >= 400){
				barrelxSourceSprite.left = 400;
			}
			barrel.setTextureRect(barrelxSourceSprite);
			clock.restart();
		}

#endif // ANIMATIONS_H
