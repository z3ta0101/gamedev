#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <tinyxml2.h>
#include <limits>
#include <random>
#include <functional>
#include <memory>
#include <fstream>
#include <sstream>
#include </home/z3ta/c++/SoV/main/vectorx2f.h>

using namespace sf;

//Player Coordinates
sf::Vector2f playerPosition(1900, -3000);

sf::Sprite player;

int area = 3;

#include </home/z3ta/c++/SoV/main/doors.h>

bool hasPlayed = false;

#include </home/z3ta/c++/SoV/main/intromovie.h>

int generateRandomNumber() {
    // Create a random number generator and seed it
    static std::random_device rd;  // Random seed
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dis(1, 20); // Define range [1, 20]

    return dis(gen);  // Generate and return the random number
}

#include </home/z3ta/c++/SoV/main/minimap.h>

#include </home/z3ta/c++/SoV/main/npc.h>

int playerHealth = 10;
int playerArmor = 17;

#include </home/z3ta/c++/SoV/main/enemy.h>

#include </home/z3ta/c++/SoV/main/polygon.h>

#include </home/z3ta/c++/SoV/main/inv.h>


int main()
{
	// Load font for button text
    sf::Font font;
    if (!font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    Enemy enemy(50, 50, playerHealth, playerArmor);
    Enemy enemy1(5000, 5000, playerHealth, playerArmor);
	
	
	Minimap minimap1("/home/z3ta/c++/SoV/images/backgrounds/town1xmm.jpg", 0.1f, sf::Vector2f(100.f, 100.f)); // Example values
	Minimap minimap2("/home/z3ta/c++/SoV/images/backgrounds/town2mm.jpg", 0.1f, sf::Vector2f(100.f, 100.f)); // Example values
	Minimap minimap3("/home/z3ta/c++/SoV/images/backgrounds/wilderness1mm.jpg", 0.1f, sf::Vector2f(100.f, 100.f)); // Example values
	Door fjeltaverndoor(sf::Vector2f(900, -3472), "/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
	Door2 halfgiantsdoor(sf::Vector2f(249, 1733), "/home/z3ta/c++/SoV/images/sprites/hgdoorspritesheet.png");
	Door3 smithdoor(sf::Vector2f(7835, -3364), "/home/z3ta/c++/SoV/images/sprites/smithdoor.png");
	Door4 herbalistdoor(sf::Vector2f(9664, 967), "/home/z3ta/c++/SoV/images/sprites/herbalistdoorspritesheet.png");
	std::string textureFile = "/home/z3ta/c++/SoV/images/sprites/buttons/button1.png";

	int playerAttackRoll = generateRandomNumber();

	#include </home/z3ta/c++/SoV/main/vertices.h>

	srand(time(NULL));

	const unsigned WINDOW_WIDTH = 1400;
	const unsigned WINDOW_HEIGHT = 1400;
	const unsigned WINDOW_WIDTHX = 1400;
	const unsigned WINDOW_HEIGHTX = 800;
    bool walkright = 0;
	bool walkleft = 0;
    bool movediag1 = 0;
	bool movediag2 = 0;
    int playervelx = 0;
	int playervely = 0;
	int moveupleftdiagkey1 = 0;
	int moveupleftdiagkey2 = 0;
	int movedownleftdiagkey1 = 0;
	int movedownleftdiagkey2 = 0;
	int moveuprightdiagkey1 = 0;
	int moveuprightdiagkey2 = 0;
	int movedownrightdiagkey1 = 0;
	int movedownrightdiagkey2 = 0;
	int up = 1;
	int right = 2;
	int down = 3;
	int left = 4;
	int upleft = 5;
	int upright = 6;
	int downright = 7;
	int downleft = 8;
	int direction = 1;
    int directionx = 0;
	int idle = 1;
	bool isMoving = false;
	bool isMovingx = true;
	bool attacking = false;
	bool round = false;
	int repose = 0;
	float playerSpeed = 5.0f;
	int barrelhealth = 2;
	bool fireplace = true;

	
	
	//Vectors
	
	Vector2f playerCenter;
	Vector2f playerRight;
	Vector2f playerLeft;
	Vector2f enemyxCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	Vector2f loc;
	
	RenderWindow window(VideoMode(1400, 1400), "Shadows of Vaalundroth", Style::Default);
	window.setFramerateLimit(60);
	
    //Felstrond backdrop
    sf::Texture backgroundtxtr;
    backgroundtxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town1x.jpg");
    
	Inventory inventory(10);

    sf::Texture daggerTexture;
    if (!daggerTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/daggericon.png")) {
        std::cerr << "Error loading item icon!" << std::endl;
        return -1;
    }

    // Initialize items correctly
    for (int i = 0; i < 1; ++i) {
        Item dagger = {
            "dagger " + std::to_string(i + 1),  // Name
            i + 1,  // ID
            1,  // Quantity
            daggerTexture,  // Icon
            ItemType::Weapon,  // Type
            1,  // Value
            10,  // Durability
            {"", 0},  // Effect
            false,  // Stackable
            {0, ""},  // UseLimitations
            "A simple dagger"  // Description
        };
        inventory.addItem(dagger);
        
    }

    sf::Texture inventoryBackgroundTexture;
    if (!inventoryBackgroundTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/invpanel.png")) {
        std::cerr << "Error loading inventory background!" << std::endl;
        return -1;
    }

    sf::Texture emptySlotTexture;
    if (!emptySlotTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/invslot.png")) {
        std::cerr << "Error loading empty slot texture!" << std::endl;
        return -1;
    }

    sf::Texture groundSlotTexture;
    if (!groundSlotTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/invslot.png")) {
        std::cerr << "Error loading ground slot texture!" << std::endl;
        return -1;
    }

    InventoryUI inventoryUI(&inventory, inventoryBackgroundTexture, emptySlotTexture, {700, 275}, 2, 5, groundSlotTexture);

    sf::Sprite background;
    background.setTexture(backgroundtxtr);
    background.setPosition(0, -4200);

	//Felstrond cave
	sf::Texture cavetxtr;
	cavetxtr.loadFromFile("/home/z3ta/c++/SoV/images/layers/fcaveoverlayer.png");

	sf::Sprite overhang;
	overhang.setTexture(cavetxtr);
	overhang.setPosition(9228, -560);

	//fjelenvår backdrop
    sf::Texture fjelenvartxtr;
    fjelenvartxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town2.jpg");
    
    sf::Sprite fjelenvar;
    fjelenvar.setTexture(fjelenvartxtr);
    fjelenvar.setPosition(0, -4200);

	//fjeltaverninn
	sf::Texture fjeltaverntxtr;
	fjeltaverntxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/fjeltaverninn.png");
	sf::Sprite fjeltaverninn;
	fjeltaverninn.setTexture(fjeltaverntxtr);
	fjeltaverninn.setPosition(2700, 1700);
	fjeltaverninn.setRotation(71);

	//fjeltavernfireplace
	sf::IntRect fjelhearthSourceSprite(0, 0, 414, 155);
	sf::Texture fjelfireplacetxtr;
	fjelfireplacetxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/fireplacespritesheet.png");
	sf::Sprite fjelfireplace;
	fjelfireplace.setTexture(fjelfireplacetxtr);
	fjelfireplace.setPosition(2940, 2400);
	fjelfireplace.setRotation(71);

	//fjelhearthfirebox
	sf::Texture fjelfireboxtxtr;
	fjelfireboxtxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/fireplacefirebox.png");
	sf::Sprite fjelfirebox;
	fjelfirebox.setTexture(fjelfireboxtxtr);
	fjelfirebox.setPosition(2940, 2400);
	fjelfirebox.setRotation(71);
	
	//fhouse1
	sf::Texture fhouse1txtr;
	fhouse1txtr.loadFromFile("/home/z3ta/c++/SoV/images/layers/fhouse1top.png");

	sf::Sprite fhouse1x;
	fhouse1x.setTexture(fhouse1txtr);
	fhouse1x.setPosition(2439, 1367);

	//wilderness1 backdrop
	sf::Texture wilderness1txtr;
	wilderness1txtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/wilderness1.jpg");

	sf::Sprite wilderness1;
	wilderness1.setTexture(wilderness1txtr);
	wilderness1.setPosition(0, -4200);

	sf::Vector2f minimapPosition(1100, 1100);
	sf::Vector2f xPosition(1500, 1500);
	sf::Vector2f xAdjust(104, 140);
	//Object Coordinates
	sf::Vector2f barrelPosition(1720, 990);
	sf::Vector2f barreltopPosition(720, 1936);
	//Building Coordinates
	sf::Vector2f tavernPosition(1000, -1000);
	sf::Vector2f tavernTopPosition(1000, -1000);
	sf::Vector2f whouseTopPosition(6500, 4184);
	sf::Vector2f wtowerTopPosition(8453, 1015);
	sf::Vector2f bhouseTopPosition(6732, -3775);
	sf::Vector2f fjeltreePosition(4995, 3455);
	sf::Vector2f fjeldoor1Position(905, -3473);

	//Player
	sf::IntRect rectSourceSprite(0, 0, 127, 127);
    sf::Texture playertxtr;
    playertxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");
	player.setTexture(playertxtr);
	player.setPosition(playerPosition);
	
	sf::IntRect barrelSourceSprite(0, 0, 71, 98);
	sf::IntRect xSprite(0, 0, 24, 19);
    
	sf::Sprite xcollision(playertxtr, xSprite);
	
	sf::Vector2f mapPosition(500, -200);
	sf::Texture collisionxtxtr;
	collisionxtxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");
	
    sf::Clock clock;
	sf::Clock clockx;
	sf::Clock fireclock;
	
	xcollision.setTexture(playertxtr);
	xcollision.setPosition(xPosition);
	sf::Vector2f sizeIncrease(1.0f, 1.0f);
	sf::Vector2f sizeDecrease(0.1f, 0.1f);
	player.setScale(sf::Vector2f(player.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
	xcollision.setScale(sf::Vector2f(xcollision.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
	
	
	//Barrels
	sf::Texture barreltxtr;
	barreltxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barreldestructsequence1.png");
	sf::IntRect barrelxSourceSprite(0, 0, 100, 100);
	sf::Sprite barrel(barreltxtr, barrelxSourceSprite);
	barrel.setPosition(barrelPosition);
	


	sf::Texture barreltoptxtr;
	barreltoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barrel1top.png");
	sf::Sprite barreltop(barreltoptxtr);
	barreltop.setPosition(barreltopPosition);

	//Buildings
	sf::Texture taverntxtr;
	taverntxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/longhousetavernsprite.png");
	sf::Sprite tavern(taverntxtr);
	tavern.setPosition(tavernPosition);

	//TavernTop
	sf::Texture taverntoptxtr;
	taverntoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/taverntop.png");
	sf::Sprite taverntop(taverntoptxtr);
	taverntop.setPosition(tavernTopPosition);

	//whousetop
	sf::Texture whousetoptxtr;
	whousetoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/whousetop.png");
	sf::Sprite whousetop(whousetoptxtr);
	whousetop.setPosition(whouseTopPosition);

	//wtowertop
	sf::Texture wtowertoptxtr;
	wtowertoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/wtowertop.png");
	sf::Sprite wtowertop(wtowertoptxtr);
	wtowertop.setPosition(wtowerTopPosition);

	//bhousetop
	sf::Texture bhousetoptxtr;
	bhousetoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/bhousetop.png");
	sf::Sprite bhousetop(bhousetoptxtr);
	bhousetop.setPosition(bhouseTopPosition);

	//fjeltree
	sf::Texture fjeltreetxtr;
	fjeltreetxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/fjeltreex.png");
	sf::Sprite fjeltree(fjeltreetxtr);
	fjeltree.setPosition(fjeltreePosition);

	//fjeltaverndoor
	sf::IntRect door1SourceSprite(0, 0, 338, 281);
	sf::Texture door1txtr;
	door1txtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
	sf::Sprite taverndoor(door1txtr, door1SourceSprite);
	taverndoor.setPosition(fjeldoor1Position);
	NPC npc(660.f, 2480.f, font, window);
	bool isPlayerFrozen = false;
	while (window.isOpen())
	{
		Event event;
		Polygon playerOriginal = collisionx;
		Polygon attackxOriginal = attackx;
		sf::Vector2f playerPos = player.getPosition();  // Get the player's position
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			
			npc.handleInput(event, mousePos, window);

			// Toggle the visibility of the inventory with the "I" key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
                inventoryUI.isVisible = !inventoryUI.isVisible;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < inventoryUI.itemSlots.size(); ++i) {
                    if (inventoryUI.itemSlots[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)))) {
                        inventoryUI.startDragging(i); // Start dragging the item
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                inventoryUI.stopDragging(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))); // Stop dragging and drop the item
            }
		}

		if (hasPlayed == false){
			playIntroMovie(window);
		}
        
		#include </home/z3ta/c++/SoV/main/animations.h>

		//Vectors
		playerCenter = Vector2f(player.getPosition());
		playerRight = Vector2f(player.getPosition());
		playerLeft = Vector2f(player.getPosition());
		
		mousePosWindow = Vector2f(Mouse::getPosition(window));

		aimDir = mousePosWindow - playerCenter;
		float aimDirMagnitude = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		
        if (aimDirMagnitude != 0) 
		{
            aimDirNorm = aimDir / aimDirMagnitude;
        } 
		
		else 
		
		{
            aimDirNorm = aimDir; // Handle division by zero or very small magnitude case
        }

		float PI = 3.14159265f;
		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
		//player.setRotation(deg + 90);

		//Notes
		const auto prevPos = player.getPosition();

		// Move player with arrow keys
        if (Keyboard::isKeyPressed(Keyboard::A) && !isPlayerFrozen) {
			player.move(-playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && !isPlayerFrozen) {
			player.move(playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && !isPlayerFrozen) {
			player.move(0.f, -playerSpeed);
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && !isPlayerFrozen) {
			player.move(0.f, playerSpeed);
        }

		else{
			idle = 1;
		}

		// Adjust the offset based on your player's height and desired collision position
		float collisionOffsetY = 100.0f; // Adjust this value to move the collision box down
		Vectorx2f adjustedPosition = Vectorx2f::fromSF(player.getPosition());
		adjustedPosition.y += collisionOffsetY; // Move the collision box down
		adjustedPosition.x += collisionOffsetY;
		// Set the collision position to the adjusted position
		collisionx.setPosition(adjustedPosition);
		
		#include </home/z3ta/c++/SoV/main/collision.h>
		
		// Draw the player polygon
        sf::ConvexShape playerShape;
        playerShape.setPointCount(collisionx.vertices.size());
        for (size_t i = 0; i < collisionx.vertices.size(); ++i) {
            playerShape.setPoint(i, sf::Vector2f(collisionx.vertices[i].x, collisionx.vertices[i].y));
        }
		
        playerShape.setFillColor(sf::Color::Green);

		// Draw the boundry polygon
        sf::ConvexShape boundry1Shape;
        boundry1Shape.setPointCount(fboundry1.vertices.size());
        for (size_t i = 0; i < fboundry1.vertices.size(); ++i) {
            boundry1Shape.setPoint(i, sf::Vector2f(fboundry1.vertices[i].x, fboundry1.vertices[i].y));
        }

		boundry1Shape.setFillColor(sf::Color::Blue);

		sf::ConvexShape boundry2Shape;
        boundry2Shape.setPointCount(fboundry2.vertices.size());
        for (size_t i = 0; i < fboundry2.vertices.size(); ++i) {
            boundry2Shape.setPoint(i, sf::Vector2f(fboundry2.vertices[i].x, fboundry2.vertices[i].y));
        }
		
		boundry2Shape.setFillColor(sf::Color::Blue);

		sf::ConvexShape boundry4Shape;
        boundry4Shape.setPointCount(fboundry4.vertices.size());
        for (size_t i = 0; i < fboundry4.vertices.size(); ++i) {
            boundry4Shape.setPoint(i, sf::Vector2f(fboundry4.vertices[i].x, fboundry4.vertices[i].y));
        }
		
		boundry4Shape.setFillColor(sf::Color::Blue);
		
		// Draw the attack range polygon
		sf::ConvexShape attackrangeShape;
        attackrangeShape.setPointCount(attackx.vertices.size());
        for (size_t i = 0; i < attackx.vertices.size(); ++i) {
            attackrangeShape.setPoint(i, sf::Vector2f(attackx.vertices[i].x, attackx.vertices[i].y));
        }
		
        attackrangeShape.setFillColor(sf::Color::Blue);

		//Draw
        sf::View view;
		
		view.setCenter(player.getPosition());
		view.setSize(1700, 1700);
		window.setView(view);
		
		// Draw the building polygon
        
		 // Get time elapsed since last frame
        float deltaTime = clockx.getElapsedTime().asSeconds();
		
		// Draw the barrelx polygon
		sf::ConvexShape barrelxShape;
		barrelxShape.setPointCount(barrelx.vertices.size());
		for (size_t i = 0; i < barrelx.vertices.size(); ++i) {
			barrelxShape.setPoint(i, sf::Vector2f(barrelx.vertices[i].x, barrelx.vertices[i].y));
		}
		barrelxShape.setFillColor(sf::Color::Red);
		
		window.clear();
		
		mousePos = sf::Mouse::getPosition(window);

		if (area == 1){
        	window.draw(background);

			window.draw(boundry1Shape);
		
			window.draw(barrel);

			window.draw(player);
			window.draw(fhouse1x);
			window.draw(overhang);
			window.draw(boundry4Shape);
			minimap1.draw(window);
		
		}

		if (area == 2){
        	window.draw(fjelenvar);
			fjeltaverndoor.render(window);
			fjeltaverndoor.update(mousePos, window, playerPos);
			halfgiantsdoor.render(window);
			halfgiantsdoor.update(mousePos, window, playerPos);
			smithdoor.render(window);
			smithdoor.update(mousePos, window, playerPos);
			herbalistdoor.render(window);
			herbalistdoor.update(mousePos, window, playerPos);
			
			window.draw(player);
			window.draw(fjeltree);
			minimap2.draw(window);
		}

		if (area == 3){
			window.draw(wilderness1);
            window.draw(enemy.getSprite());
			window.draw(enemy1.getSprite());
			window.draw(player);
			minimap3.draw(window);
			enemy.update(player.getPosition().x, player.getPosition().y, deltaTime);
			enemy1.update(player.getPosition().x, player.getPosition().y, deltaTime);
			
		}

		if (area == 4){
			window.draw(fjeltaverninn);
			window.draw(fjelfireplace);
			window.draw(fjelfirebox);
			window.draw(player);
			npc.update(playerPosition, mousePos);  // Update NPC based on player position and mouse position
			npc.handleInput(event, mousePos, window);  // Handle mouse input for the conversation
			npc.draw(window);
		}
		
		minimap1.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
		minimap2.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
		minimap3.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
		inventoryUI.draw(window);
		window.display();
		

	}

	return 0;
}
