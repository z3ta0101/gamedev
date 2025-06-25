#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class Enemy {
public:
    float x, y; // Enemy's position
    int direction; // Direction the enemy is facing (0-7)
	int health = 4;
	int playerhealth;
	int playerarmor;
    const float detectionRange = 1000.0f; // Range to detect a target
	const float attackRange = 70.0f;
    float speed = 2.0f; // Enemy movement speed
	bool attacking = false;
    sf::Sprite sprite;  // The enemy's sprite
    sf::Texture texture; // The texture (spritesheet)
    sf::IntRect rectSourceSprite; // Source rect for texture (for animation)

	int generateRandomNumber() {
    	// Create a random number generator and seed it
    	static std::random_device rd;  // Random seed
    	static std::mt19937 gen(rd()); // Mersenne Twister engine
    	std::uniform_int_distribution<> dis(1, 20); // Define range [1, 20]

    	return dis(gen);  // Generate and return the random number
	}

	int enemyAttackRoll = generateRandomNumber();
    // Timing variables for animation frame updates
    float animationTimer;
    const float animationSpeed = 70.0f; // Speed of animation frame updates (seconds per frame)

    // Boolean to track whether the enemy is moving
    bool isMoving;

    // Constructor to initialize the enemy's position and load the texture
    Enemy(float startX, float startY, int& playerHealth, int& playerArmor) : x(startX), y(startY), direction(0), animationTimer(0.0f), isMoving(false), playerhealth(playerHealth), playerarmor(playerArmor) {
        if (!texture.loadFromFile("/home/z3ta/c++/SoV/images/sprites/skelspritesheetx.png")) {
            std::cerr << "Error loading enemy texture!" << std::endl;
        }

        sprite.setTexture(texture);
        rectSourceSprite = sf::IntRect(0, 0, 127, 100); // Initial position of the sprite (default direction 0)
        sprite.setTextureRect(rectSourceSprite);
		sf::Vector2f sizeIncrease(1.0f, 1.0f);
        sprite.setScale(sf::Vector2f(sprite.getScale().x + sizeIncrease.x, sprite.getScale().y + sizeIncrease.y));
        sprite.setPosition(x, y);
    }

    // Update the enemy's behavior based on a target position (e.g., player or other object)
    void update(float targetX, float targetY, float deltaTime) {
        float distance = std::sqrt(std::pow(targetX - x, 2) + std::pow(targetY - y, 2));

        // Only move and update animation if within detection range
        if (distance < detectionRange) {
            // Move the enemy towards the target
            moveTowardsTarget(targetX, targetY, deltaTime);

            // Update animation based on whether the enemy is moving
            updateAnimation(deltaTime);
			sprite.setPosition(x, y);
        } else {
            // If the enemy is outside detection range, set it to idle
            isMoving = false;
            updateAnimation(deltaTime);
			
        }
		
    }

	void updatehealth(float targetX, float targetY){
		float distance = std::sqrt(std::pow(targetX - x, 2) + std::pow(targetY - y, 2));
		std::cout << "distance: " << distance << std::endl;
		if(distance < attackRange){
			health -= 1;
			std::cout << "enemy health: " << health << std::endl;
		}
	
	}

private:
    // Method for moving the enemy towards the target (could be the player or any other target)
    void moveTowardsTarget(float targetX, float targetY, float deltaTime){
    	// Calculate direction to move
    	float dirX = targetX - x;
    	float dirY = targetY - y;
    	float length = std::sqrt(dirX * dirX + dirY * dirY);
		
    	if (length > 70 && health > 0){
        	// Normalize direction vector
        	dirX /= length;
        	dirY /= length;

        	// Move the enemy towards the target
        	x += dirX * speed * 1;  // Adjust movement by deltaTime for smooth movement
        	y += dirY * speed * 1;

        	// Update the direction the enemy is facing
        	updateMovementDirection(dirX, dirY);

        	// Set isMoving to true since the enemy is moving
        	isMoving = true;
			attacking = false;
    	} 
		
		else{
        	// If the distance to the target is too small, set isMoving to false (idle)
        	isMoving = false;
			attacking = true;
			updateAttackDirection(dirX, dirY);
    	}

		if(health <= 0){
			isMoving = false;
			updateDeathDirection(dirX, dirY);
		}

	}

    // Update the direction the enemy is facing based on the movement direction
    void updateMovementDirection(float dirX, float dirY){
    	// Calculate the angle in radians
    	float angle = std::atan2(dirY, dirX) * 180.0f / 3.14159f; // Convert radians to degrees

    	// Normalize the angle to be between 0 and 360 degrees
    	if (angle < 0) {
        	angle += 360.0f;
    	}

    	// Now map the angle to one of the 8 directions (0-7)
    	if (angle >= 337.5f || angle < 22.5f) {
        	direction = 46; // Right
    	} else if (angle >= 22.5f && angle < 67.5f) {
        	direction = 47; // Down-Right
    	} else if (angle >= 67.5f && angle < 112.5f) {
        	direction = 40; // Down
    	} else if (angle >= 112.5f && angle < 157.5f) {
        	direction = 41; // Down-Left
    	} else if (angle >= 157.5f && angle < 202.5f) {
        	direction = 42; // Left
    	} else if (angle >= 202.5f && angle < 247.5f) {
        	direction = 43; // Up-Left
    	} else if (angle >= 247.5f && angle < 292.5f) {
        	direction = 44; // Up
    	} else if (angle >= 292.5f && angle < 337.5f) {
        	direction = 45; // Up-Right
    	}
		
	}

	void updateAttackDirection(float dirX, float dirY) {
    	// Calculate the angle in radians
    	float angle = std::atan2(dirY, dirX) * 180.0f / 3.14159f; // Convert radians to degrees

    	// Normalize the angle to be between 0 and 360 degrees
    	if (angle < 0) {
        	angle += 360.0f;
    	}

    	// Now map the angle to one of the 8 directions (0-7)
    	if (angle >= 337.5f || angle < 22.5f) {
        	direction = 6; // Right
    	} else if (angle >= 22.5f && angle < 67.5f) {
        	direction = 7; // Down-Right
    	} else if (angle >= 67.5f && angle < 112.5f) {
        	direction = 0; // Down
    	} else if (angle >= 112.5f && angle < 157.5f) {
        	direction = 1; // Down-Left
    	} else if (angle >= 157.5f && angle < 202.5f) {
        	direction = 2; // Left
    	} else if (angle >= 202.5f && angle < 247.5f) {
        	direction = 3; // Up-Left
    	} else if (angle >= 247.5f && angle < 292.5f) {
        	direction = 4; // Up
    	} else if (angle >= 292.5f && angle < 337.5f) {
        	direction = 5; // Up-Right
    	}

	}

	void updateDeathDirection(float dirX, float dirY) {
		isMoving = false;
    	// Calculate the angle in radians
    	float angle = std::atan2(dirY, dirX) * 180.0f / 3.14159f; // Convert radians to degrees

    	// Normalize the angle to be between 0 and 360 degrees
    	if (angle < 0) {
        	angle += 360.0f;
    	}

    	// Now map the angle to one of the 8 directions (0-7)
    	if (angle >= 337.5f || angle < 22.5f) {
        	direction = 14; // Right
    	} else if (angle >= 22.5f && angle < 67.5f) {
        	direction = 15; // Down-Right
    	} else if (angle >= 67.5f && angle < 112.5f) {
        	direction = 8; // Down
    	} else if (angle >= 112.5f && angle < 157.5f) {
        	direction = 9; // Down-Left
    	} else if (angle >= 157.5f && angle < 202.5f) {
        	direction = 10; // Left
    	} else if (angle >= 202.5f && angle < 247.5f) {
        	direction = 11; // Up-Left
    	} else if (angle >= 247.5f && angle < 292.5f) {
        	direction = 12; // Up
    	} else if (angle >= 292.5f && angle < 337.5f) {
        	direction = 13; // Up-Right
    	}

		if (rectSourceSprite.left >= 1778) {
			rectSourceSprite.left = 1778;
		}
	}

    // Update the enemy's animation based on the direction and isMoving status
    void updateAnimation(float deltaTime) {
        // Increment the animation timer by the delta time
        animationTimer += 10;
		
        // If enough time has passed, move to the next animation frame
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.0f; // Reset timer
			if (health > 0){
            	// Calculate the top position in the sprite sheet based on the current direction
            	rectSourceSprite.top = direction * 99.4; // 100 is the height of each sprite (height per row)
			}

			else{
				rectSourceSprite.top = 800;
			}
            // Determine the animation state based on movement (e.g., 0 = idle, 1 = moving)
            int animationState = isMoving ? 1 : 0;  // 1 for moving, 0 for idle

            // Update the left value based on the animation state
            
			if (health > 0){
            	// Move to the next frame in the animation (assuming 8 frames per direction)
            	if (rectSourceSprite.left >= 127 * 7) {  // If the left position exceeds the max frame (7 frames per direction)
                	rectSourceSprite.left = 0; // Reset to the first frame
            	} else {
                	rectSourceSprite.left += 127; // Move to the next frame
            	}
			}

			else if (health <= 0){
				if (rectSourceSprite.left >= 1778){
					rectSourceSprite.left = 1778;
				}
				else{
					rectSourceSprite.left += 127;
				}
			}

			if (health > 0 && attacking == true){
				if (rectSourceSprite.left == 889){
					enemyAttackRoll = generateRandomNumber();
					std::cout << "enemy attack roll: " << enemyAttackRoll << std::endl;
					if (enemyAttackRoll >= playerArmor){
						playerHealth -= 1;
                    	std::cout << "player health: " << playerHealth << std::endl;
					}
				}
			}

            // Set the updated texture rect for the sprite
            sprite.setTextureRect(rectSourceSprite);
        }
    }

public:
    // Getter for the sprite
    sf::Sprite& getSprite() {
        return sprite;
    }
};

#endif // ENEMY_H
