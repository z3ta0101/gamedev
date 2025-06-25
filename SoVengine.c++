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
#include "./vectorx2f.h"
#include "./WorldTime.h"
#include "./feedback.h"
#include "./portrait.h"

using namespace sf;

// Player Coordinates
sf::Vector2f playerPos(1100, -200);

sf::Sprite player;

int area = 5;

bool hasPlayed = false;

#include "./intromovie.h"

int generateRandomNumber() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 20);
    return dis(gen);
}

#include "./minimap.h"
#include "./npc.h"

int playerHealth = 10;
const int maxPlayerHealth = 10; // Maximum player health
int playerArmor = 10;
bool isPlatemailEquipped = false;

#include "./enemy.h"
#include "./polygon.h"
#include "./inv.h"
#include "./region_map.h"
#include "./doors.h"

int main()
{
    sf::Font font;
    if (!font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    Enemy enemy(50, 50, playerHealth, playerArmor);
    Enemy enemy1(5000, 5000, playerHealth, playerArmor);

    Minimap minimap1("/home/z3ta/c++/SoV/images/backgrounds/town1xmm.jpg", 0.1f, sf::Vector2f(100.f, 100.f));
    Minimap minimap2("/home/z3ta/c++/SoV/images/backgrounds/town2mm.jpg", 0.1f, sf::Vector2f(100.f, 100.f));
    Minimap minimap3("/home/z3ta/c++/SoV/images/backgrounds/wilderness1mm.jpg", 0.1f, sf::Vector2f(100.f, 100.f));
    PolyDoor fjeltavernout;
    Door1 fjeltaverndoor(sf::Vector2f(40, -2390), "/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
    Door2 halfgiantsdoor(sf::Vector2f(249, 1733), "/home/z3ta/c++/SoV/images/sprites/hgdoorspritesheet.png");
    Door3 smithdoor(sf::Vector2f(7835, -3364), "/home/z3ta/c++/SoV/images/sprites/smithdoor.png");
    Door4 herbalistdoor(sf::Vector2f(9664, 967), "/home/z3ta/c++/SoV/images/sprites/herbalistdoorspritesheet.png");
    std::string textureFile = "/home/z3ta/c++/SoV/images/sprites/buttons/button1.png";

    sf::Texture& inventoryBackgroundTexture = IconManager::getIcon("/home/z3ta/c++/SoV/images/ui/invpanel.png");
    sf::Texture& emptySlotTexture = IconManager::getIcon("/home/z3ta/c++/SoV/images/ui/invslot.png");
    sf::Texture& groundSlotTexture = IconManager::getIcon("/home/z3ta/c++/SoV/images/ui/invslot.png");

    int playerAttackRoll = generateRandomNumber();

    #include "./vertices.h"

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

    // Debug default view size
    sf::View defaultView = window.getDefaultView();
    std::cout << "Default view size: " << defaultView.getSize().x << "x" << defaultView.getSize().y << std::endl;

    sf::Texture& backgroundtxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/town1x.jpg");
    sf::Sprite background(backgroundtxtr);
    background.setPosition(0, -4200);

    Inventory inventory(16);

    std::vector<Item> items = {
        Item("Potion of Healing", 1, 1, "/home/z3ta/c++/SoV/images/ui/hpotion.png", ItemType::Consumable, 10, 100, Effect(), true, UseLimitations(), "Potion of Healing"),
        Item("Dagger", 1, 1, "/home/z3ta/c++/SoV/images/ui/daggericon.png", ItemType::Weapon, 10, 100, Effect(), false, UseLimitations(), "Dagger"),
        Item("Platemail", 1, 1, "/home/z3ta/c++/SoV/images/ui/icons/plateicon00001.png", ItemType::Armor, 10, 100, Effect(), false, UseLimitations(), "Platemail")
    };

    InventoryUI inventoryUI(&inventory, inventoryBackgroundTexture, emptySlotTexture, sf::Vector2f(50, 50), 4, 4, groundSlotTexture);
    inventoryUI.isVisible = false;

    for (int i = 0; i < 1; ++i) {
        Item dagger("dagger " + std::to_string(i + 1), i + 1, 1, "/home/z3ta/c++/SoV/images/ui/daggericon.png", ItemType::Weapon, 10, 100, Effect(), false, UseLimitations(), "Dagger");
        inventory.addItem(dagger);
    }

    for (int i = 0; i < 1; ++i) {
        Item hpotion("hpotion " + std::to_string(i + 1), i + 2, 1, "/home/z3ta/c++/SoV/images/ui/healthpotion.png", ItemType::Consumable, 10, 100, Effect(), true, UseLimitations(), "Potion of Healing");
        inventory.addItem(hpotion);
        inventory.addItem(hpotion);
    }

    for (int i = 0; i < 1; ++i) {
        Item platemail("platemail " + std::to_string(i + 1), i + 3, 1, "/home/z3ta/c++/SoV/images/ui/icons/plateicon00001.png", ItemType::Armor, 10, 100, Effect(), false, UseLimitations(), "Platemail");
        inventory.addItem(platemail);
    }

    sf::Texture& cavetxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/layers/fcaveoverlayer.png");
    sf::Sprite overhang(cavetxtr);
    overhang.setPosition(9228, -560);

    sf::Texture& fjelenvartxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/fjel1.png");
    sf::Sprite fjelenvar(fjelenvartxtr);
    fjelenvar.setPosition(0, -4200);

    sf::Texture& fjeltaverntxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/fjeltaverninn.png");
    sf::Sprite fjeltaverninn(fjeltaverntxtr);
    fjeltaverninn.setPosition(2700, 1700);
    fjeltaverninn.setRotation(61);

    sf::IntRect fjelhearthSourceSprite(0, 0, 220, 220);
    sf::Texture& fjelfireplacetxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/buildings/interior/fireplace1.png");
    sf::Sprite fjelfireplace(fjelfireplacetxtr);
    fjelfireplace.setPosition(2840, 2340);
    fjelfireplace.setRotation(18);

    sf::Texture& fjelfireboxtxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/fireplacefirebox.png");
    sf::Sprite fjelfirebox(fjelfireboxtxtr);
    fjelfirebox.setPosition(2940, 2400);
    fjelfirebox.setRotation(71);

    sf::Texture& fhouse1txtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/layers/fhouse1top.png");
    sf::Sprite fhouse1x(fhouse1txtr);
    fhouse1x.setPosition(2439, 1367);

    sf::Texture& wilderness1txtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/wilderness1.jpg");
    sf::Sprite wilderness1(wilderness1txtr);
    wilderness1.setPosition(0, -4200);

    sf::Vector2f minimapPosition(1100, 1100);
    sf::Vector2f xPosition(1500, 1500);
    sf::Vector2f xAdjust(104, 140);

    sf::Vector2f barrelPosition(1720, 990);
    sf::Vector2f barreltopPosition(720, 1936);

    sf::Vector2f tavernPosition(1000, -1000);
    sf::Vector2f tavernTopPosition(1000, -1000);
    sf::Vector2f whouseTopPosition(6500, 4184);
    sf::Vector2f wtowerTopPosition(8453, 1015);
    sf::Vector2f bhouseTopPosition(6732, -3775);
    sf::Vector2f fjeltreePosition(4995, 3455);
    sf::Vector2f fjeldoor1Position(905, -3473);

    sf::IntRect rectSourceSprite(0, 0, 0, 0);
    sf::Texture& playertxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/spritesheetfemale/femalebase.png");
    player.setTexture(playertxtr);
    player.setPosition(playerPos);

    sf::IntRect clothesSourceSprite(0, 0, 0, 0);
    sf::IntRect armorSourceSprite(0, 0, 0, 0);

    sf::Sprite playerclothes;
    sf::Texture& playerclothestxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/spritesheetfemale/femaleclothes.png");
    playerclothes.setTexture(playerclothestxtr);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerclothes.setPosition(playerPos);

    sf::Sprite playerarmor;
    sf::Texture& playerarmortxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/spritesheetfemale/femalearmor.png");
    playerarmor.setTexture(playerarmortxtr);
    playerarmor.setTextureRect(armorSourceSprite);
    playerarmor.setPosition(playerPos);

    sf::IntRect barrelSourceSprite(0, 0, 71, 98);
    sf::IntRect xSprite(0, 0, 24, 19);

    sf::Sprite xcollision(playertxtr, xSprite);

    sf::Vector2f mapPosition(500, -200);
    sf::Texture& collisionxtxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");

    sf::Clock worldClock; // Clock for WorldTime
    sf::Clock clock;      // Clock for animations
    sf::Clock clockx;
    sf::Clock fireclock;

    xcollision.setTexture(playertxtr);
    xcollision.setPosition(xPosition);
    sf::Vector2f sizeIncrease(1.0f, 1.0f);
    sf::Vector2f sizeIncrease2(0.7f, 0.7f);
    sf::Vector2f sizeDecrease(0.1f, 0.1f);
    player.setScale(sf::Vector2f(player.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
    playerclothes.setScale(sf::Vector2f(playerclothes.getScale().x + sizeIncrease.x, playerclothes.getScale().y + sizeIncrease.y));
    playerarmor.setScale(sf::Vector2f(playerarmor.getScale().x + sizeIncrease.x, playerarmor.getScale().y + sizeIncrease.y));
    fjelfireplace.setScale(sf::Vector2f(fjelfireplace.getScale().x + sizeIncrease2.x, fjelfireplace.getScale().y + sizeIncrease2.y));
    xcollision.setScale(sf::Vector2f(xcollision.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));

    sf::Texture& barreltxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/barreldestructsequence1.png");
    sf::IntRect barrelxSourceSprite(0, 0, 100, 100);
    sf::Sprite barrel(barreltxtr, barrelxSourceSprite);
    barrel.setPosition(barrelPosition);

    sf::Texture& barreltoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/barrel1top.png");
    sf::Sprite barreltop(barreltoptxtr);
    barreltop.setPosition(barreltopPosition);

    sf::Texture& taverntxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/longhousetavernsprite.png");
    sf::Sprite tavern(taverntxtr);
    tavern.setPosition(tavernPosition);

    sf::Texture& taverntoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/taverntop.png");
    sf::Sprite taverntop(taverntoptxtr);
    taverntop.setPosition(tavernTopPosition);

    sf::Texture& whousetoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/whousetop.png");
    sf::Sprite whousetop(whousetoptxtr);
    whousetop.setPosition(whouseTopPosition);

    sf::Texture& wtowertoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/wtowertop.png");
    sf::Sprite wtowertop(wtowertoptxtr);
    wtowertop.setPosition(wtowerTopPosition);

    sf::Texture& bhousetoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/bhousetop.png");
    sf::Sprite bhousetop(bhousetoptxtr);
    bhousetop.setPosition(bhouseTopPosition);

    sf::Texture& fjeltreetxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/fjeltreex.png");
    sf::Sprite fjeltree(fjeltreetxtr);
    fjeltree.setPosition(fjeltreePosition);

    sf::IntRect door1SourceSprite(0, 0, 338, 281);
    sf::Texture& door1txtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
    sf::Sprite taverndoor(door1txtr, door1SourceSprite);
    taverndoor.setPosition(fjeldoor1Position);

    NPC npc(2120.f, 2405.f, font, window);
    RegionMapTrigger mapTrigger(4700.f, 500.f, window); // Fjelenvar to Wilderness
    RegionMapTrigger mapTrigger2(10702.f, -2370.f, window); // Wilderness to Fjelenvar
    bool isPlayerFrozen = false;

    // Create Feedback
    Feedback feedback(font);

    // Create WorldTime with Feedback
    WorldTime worldTime(feedback);

    // Create Rest button
    sf::RectangleShape restButton(sf::Vector2f(100, 50));
    restButton.setPosition(50, 1300); // Bottom left with padding
    restButton.setFillColor(sf::Color(100, 100, 100)); // Gray background
    restButton.setOutlineColor(sf::Color::White);
    restButton.setOutlineThickness(2);

    // Load button texture
    sf::Texture buttonTexture;
    if (!buttonTexture.loadFromFile("/home/z3ta/c++/SoV/images/sprites/buttons/button1.png")) {
        std::cerr << "Error loading button texture!" << std::endl;
        // Fallback to gray background
    } else {
        restButton.setTexture(&buttonTexture);
    }

    // Create Rest button text
    sf::Text restButtonText;
    restButtonText.setFont(font);
    restButtonText.setString("Hvil");
    restButtonText.setCharacterSize(20);
    restButtonText.setFillColor(sf::Color::White);
    // Center text in button
    sf::FloatRect textBounds = restButtonText.getLocalBounds();
    restButtonText.setOrigin(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2);
    restButtonText.setPosition(restButton.getPosition().x + restButton.getSize().x / 2, 
                              restButton.getPosition().y + restButton.getSize().y / 2);

    // Create Portrait
    Portrait portrait("/home/z3ta/c++/SoV/images/portraits/portrait3.png", sf::Vector2f(20, 20), playerHealth, maxPlayerHealth);
    sf::FloatRect portraitBounds = portrait.getPortrait().getGlobalBounds();
    std::cout << "Portrait initialized at position (20, 20) with health: " << playerHealth << "/" << maxPlayerHealth 
              << ", bounds: " << portraitBounds.width << "x" << portraitBounds.height << std::endl;

    float deltaTimex = 0.0f;
    static bool wasLeftMousePressed = false;
    static int previousArea = area;

    while (window.isOpen())
    {
        deltaTimex = worldClock.restart().asSeconds();

        Event event;
        Polygon playerOriginal = collisionx;
        Polygon attackxOriginal = attackx;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            npc.handleInput(event, mousePos, window);
            mapTrigger.handleInput(event, mousePos);
            mapTrigger2.handleInput(event, mousePos);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
                inventoryUI.isVisible = !inventoryUI.isVisible;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePosView = window.mapPixelToCoords(mousePos, window.getDefaultView());
                if (restButton.getGlobalBounds().contains(mousePosView)) {
                    worldTime.triggerRest(playerHealth, maxPlayerHealth);
                }
            }
        }

        // Check for area changes
        if (area != previousArea) {
            std::cout << "Area changed to: " << area << ", PlayerPos: (" << playerPos.x << ", " << playerPos.y << ")\n";
            player.setPosition(playerPos);
            playerclothes.setPosition(playerPos);
            playerarmor.setPosition(playerPos);
            previousArea = area;
        }

        // Combat trigger (left-click near enemy in area 3)
        bool isLeftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (isLeftMousePressed && !wasLeftMousePressed && area == 3 && !isPlayerFrozen) {
            sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePos, window.getView());
            float distanceToEnemy = std::sqrt(std::pow(enemy.getSprite().getPosition().x - mouseWorldPos.x, 2) +
                                              std::pow(enemy.getSprite().getPosition().y - mouseWorldPos.y, 2));
            float distanceToEnemy1 = std::sqrt(std::pow(enemy1.getSprite().getPosition().x - mouseWorldPos.x, 2) +
                                               std::pow(enemy1.getSprite().getPosition().y - mouseWorldPos.y, 2));
            Enemy* targetEnemy = nullptr;
            if (distanceToEnemy <= 100.0f) {
                targetEnemy = &enemy;
            } else if (distanceToEnemy1 <= 100.0f) {
                targetEnemy = &enemy1;
            }
            if (targetEnemy) {
                int playerRoll = generateRandomNumber();
                int enemyRoll = generateRandomNumber();
                feedback.addMessage("Spiller angrep: " + std::to_string(playerRoll));
                feedback.addMessage("Fiende angrep: " + std::to_string(enemyRoll));
                if (enemyRoll > playerRoll) {
                    int damage = 1; // Placeholder damage
                    if (isPlatemailEquipped) {
                        damage = std::max(0, damage - (playerArmor / 10)); // Reduce damage based on armor
                    }
                    playerHealth = std::max(0, playerHealth - damage);
                    feedback.addMessage("Spiller tok " + std::to_string(damage) + " skade, Ny HP: " + std::to_string(playerHealth));
                    portrait.setHealth(playerHealth); // Update portrait
                }
            }
        }
        wasLeftMousePressed = isLeftMousePressed;

        if (hasPlayed == false) {
            playIntroMovie(window);
            hasPlayed = true;
        }

        #include </home/z3ta/c++/SoV/main/animations.h>

        // Update WorldTime
        worldTime.setPaused(isPlayerFrozen);
        worldTime.update(deltaTimex, playerHealth);

        // Update Feedback
        feedback.update(deltaTimex);

        // Update Portrait
        portrait.update();

        // Update Region Map
        mapTrigger.update(playerPos, mousePos);
        mapTrigger2.update(playerPos, mousePos);

        playerCenter = Vector2f(player.getPosition());
        playerRight = Vector2f(player.getPosition());
        playerLeft = Vector2f(player.getPosition());

        mousePosWindow = Vector2f(mousePos);

        aimDir = mousePosWindow - playerCenter;
        float aimDirMagnitude = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

        if (aimDirMagnitude != 0) {
            aimDirNorm = aimDir / aimDirMagnitude;
        } else {
            aimDirNorm = aimDir;
        }

        float PI = 3.14159265f;
        float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;

        const auto prevPos = player.getPosition();

        if (Keyboard::isKeyPressed(Keyboard::A) && !isPlayerFrozen) {
            player.move(-playerSpeed, 0.f);
            playerclothes.move(-playerSpeed, 0.f);
            playerarmor.move(-playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && !isPlayerFrozen) {
            player.move(playerSpeed, 0.f);
            playerclothes.move(playerSpeed, 0.f);
            playerarmor.move(playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && !isPlayerFrozen) {
            player.move(0.f, -playerSpeed);
            playerclothes.move(0.f, -playerSpeed);
            playerarmor.move(0.f, -playerSpeed);
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && !isPlayerFrozen) {
            player.move(0.f, playerSpeed);
            playerclothes.move(0.f, playerSpeed);
            playerarmor.move(0.f, playerSpeed);
        } else {
            idle = 1;
        }

        // Sync playerPos with sprite position
        playerPos = player.getPosition();

        float collisionOffsetY = 100.0f;
        Vectorx2f adjustedPosition = Vectorx2f::fromSF(player.getPosition());
        adjustedPosition.y += collisionOffsetY;
        adjustedPosition.x += collisionOffsetY;
        collisionx.setPosition(adjustedPosition);

        #include </home/z3ta/c++/SoV/main/collision.h>

        sf::ConvexShape playerShape;
        playerShape.setPointCount(collisionx.vertices.size());
        for (size_t i = 0; i < collisionx.vertices.size(); ++i) {
            playerShape.setPoint(i, sf::Vector2f(collisionx.vertices[i].x, collisionx.vertices[i].y));
        }
        playerShape.setFillColor(sf::Color::Green);

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

        sf::ConvexShape attackrangeShape;
        attackrangeShape.setPointCount(attackx.vertices.size());
        for (size_t i = 0; i < attackx.vertices.size(); ++i) {
            attackrangeShape.setPoint(i, sf::Vector2f(attackx.vertices[i].x, attackx.vertices[i].y));
        }
        attackrangeShape.setFillColor(sf::Color::Blue);

        sf::View view;
        view.setCenter(player.getPosition());
        view.setSize(1700, 1700);
        window.setView(view);

        float deltaTime = clockx.getElapsedTime().asSeconds();

        sf::ConvexShape barrelxShape;
        barrelxShape.setPointCount(barrelx.vertices.size());
        for (size_t i = 0; i < barrelx.vertices.size(); ++i) {
            barrelxShape.setPoint(i, sf::Vector2f(barrelx.vertices[i].x, barrelx.vertices[i].y));
        }
        barrelxShape.setFillColor(sf::Color::Red);

        window.clear();

        if (area == 1) {
            window.draw(background);
            window.draw(boundry1Shape);
            window.draw(barrel);
            window.draw(player);
            window.draw(fhouse1x);
            window.draw(overhang);
            window.draw(boundry4Shape);
            minimap1.draw(window);
        }
        else if (area == 2) {
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
        else if (area == 3) {
            window.draw(wilderness1);
            window.draw(enemy.getSprite());
            window.draw(enemy1.getSprite());
            window.draw(player);
            window.draw(playerclothes);
            window.draw(playerarmor);
            minimap3.draw(window);
            feedback.draw(window); // Draw feedback
            mapTrigger2.draw(window);
            enemy.update(player.getPosition().x, player.getPosition().y, deltaTime);
            enemy1.update(player.getPosition().x, player.getPosition().y, deltaTime);
        }
        else if (area == 4) {
            window.draw(fjeltaverninn);
            window.draw(fjelfireplace);
            window.draw(player);
            fjeltavernout.render(window);
            fjeltavernout.update(mousePos, window, playerPos);
            npc.update(playerPos, mousePos);
            npc.draw(window);
        }
        else if (area == 5) {
            window.draw(fjelenvar);
            fjeltaverndoor.render(window);
            fjeltaverndoor.update(mousePos, window, playerPos);
            window.draw(player);
            feedback.draw(window); // Draw feedback
            mapTrigger.draw(window);
        }
        else if (area == 6 || area == 7 || area == 8) {
            std::cerr << "Warning: Area " << area << " not fully implemented, using fallback rendering.\n";
            sf::RectangleShape fallback(sf::Vector2f(1700, 1700));
            fallback.setPosition(playerPos.x - 850, playerPos.y - 850);
            fallback.setFillColor(sf::Color::White);
            window.draw(fallback);
            window.draw(player);
            window.draw(playerclothes);
            window.draw(playerarmor);
        }

        minimap1.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
        minimap2.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
        minimap3.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
        inventoryUI.draw(window);

        // Draw UI elements in default view
        sf::View currentView = window.getView();
        window.setView(window.getDefaultView());
        portrait.draw(window); // Draw portrait
        window.draw(restButton);
        window.draw(restButtonText);
        window.setView(currentView);

        worldTime.draw(window);
        window.display();
    }

    return 0;
}
