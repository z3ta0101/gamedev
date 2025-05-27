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

using namespace sf;

// Player Coordinates
sf::Vector2f playerPosition(1900, -3000);

sf::Sprite player;

int area = 3;

#include "./doors.h"

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
int playerArmor = 17;

#include "./enemy.h"
#include "./polygon.h"
#include "./inv.h"

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
    Door fjeltaverndoor(sf::Vector2f(900, -3472), "/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
    Door2 halfgiantsdoor(sf::Vector2f(249, 1733), "/home/z3ta/c++/SoV/images/sprites/hgdoorspritesheet.png");
    Door3 smithdoor(sf::Vector2f(7835, -3364), "/home/z3ta/c++/SoV/images/sprites/smithdoor.png");
    Door4 herbalistdoor(sf::Vector2f(9664, 967), "/home/z3ta/c++/SoV/images/sprites/herbalistdoorspritesheet.png");
    std::string textureFile = "/home/z3ta/c++/SoV/images/sprites/buttons/button1.png";

    sf::Texture& inventoryBackgroundTexture = IconManager::getIcon("/home/z3ta/c++/SoV/images/ui/invpanel.png");
    sf::Texture& emptySlotTexture = IconManager::getIcon("/home/z3ta/c++/SoV/images/ui/invslot.png");
    sf::Texture& groundSlotTexture = IconManager::getIcon("/home/z3ta/c++/SoV/images/ui/invslot.png");

    int playerAttackRoll = generateRandomNumber();

    #include "./vertices.h"

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
        Item dagger("dagger " + std::to_string(i + 1), i + 1, 1, "/home/z3ta/c++/SoV/images/ui/daggericon.png", ItemType::Weapon, 1, 100, Effect(), false, UseLimitations(), "Dagger");
        inventory.addItem(dagger);
    }

    for (int i = 0; i < 1; ++i) {
        Item hpotion("hpotion " + std::to_string(i + 1), i + 2, 1, "/home/z3ta/c++/SoV/images/ui/healthpotion.png", ItemType::Consumable, 10, 100, Effect(), true, UseLimitations(), "Potion of Healing");
        inventory.addItem(hpotion);
        inventory.addItem(hpotion);
    }

    for (int i = 0; i < 1; ++i) {
        Item platemail("platemail " + std::to_string(i + 1), i + 3, 1, "/home/z3ta/c++/SoV/images/ui/icons/plateicon00001.png", ItemType::Armor, 10, 100, Effect(), true, UseLimitations(), "Platemail");
        inventory.addItem(platemail);
        
    }

    sf::Texture& cavetxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/layers/fcaveoverlayer.png");
    sf::Sprite overhang(cavetxtr);
    overhang.setPosition(9228, -560);

    sf::Texture& fjelenvartxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/town2.jpg");
    sf::Sprite fjelenvar(fjelenvartxtr);
    fjelenvar.setPosition(0, -4200);

    sf::Texture& fjeltaverntxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/backgrounds/fjeltaverninn.png");
    sf::Sprite fjeltaverninn(fjeltaverntxtr);
    fjeltaverninn.setPosition(2700, 1700);
    fjeltaverninn.setRotation(71);

    sf::IntRect fjelhearthSourceSprite(0, 0, 414, 155);
    sf::Texture& fjelfireplacetxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/fireplacespritesheet.png");
    sf::Sprite fjelfireplace(fjelfireplacetxtr);
    fjelfireplace.setPosition(2940, 2400);
    fjelfireplace.setRotation(71);

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
    sf::Texture& playertxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/spritesheetfemale/basefemale.png");
    player.setTexture(playertxtr);
    player.setPosition(playerPosition);

    sf::IntRect clothesSourceSprite(0, 0, 0, 0);

    sf::Sprite playerclothes;
    sf::Texture& playerclothestxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/layers/clotheslayer1.png");
    playerclothes.setTexture(playerclothestxtr);
    playerclothes.setPosition(playerPosition);

    sf::IntRect barrelSourceSprite(0, 0, 71, 98);
    sf::IntRect xSprite(0, 0, 24, 19);

    sf::Sprite xcollision(playertxtr, xSprite);

    sf::Vector2f mapPosition(500, -200);
    sf::Texture& collisionxtxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");

    sf::Clock clock;
    sf::Clock clockx;
    sf::Clock fireclock;

    xcollision.setTexture(playertxtr);
    xcollision.setPosition(xPosition);
    sf::Vector2f sizeIncrease(1.0f, 1.0f);
    sf::Vector2f sizeDecrease(0.1f, 0.1f);
    player.setScale(sf::Vector2f(player.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
    playerclothes.setScale(sf::Vector2f(playerclothes.getScale().x + sizeIncrease.x, playerclothes.getScale().y + sizeIncrease.y));
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

    sf::Texture& taverntoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/buildings/taverntop.png");
    sf::Sprite taverntop(taverntoptxtr);
    taverntop.setPosition(tavernTopPosition);

    sf::Texture& whousetoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/buildings/whousetop.png");
    sf::Sprite whousetop(whousetoptxtr);
    whousetop.setPosition(whouseTopPosition);

    sf::Texture& wtowertoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/buildings/wtowertop.png");
    sf::Sprite wtowertop(wtowertoptxtr);
    wtowertop.setPosition(wtowerTopPosition);

    sf::Texture& bhousetoptxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/buildings/bhousetop.png");
    sf::Sprite bhousetop(bhousetoptxtr);
    bhousetop.setPosition(bhouseTopPosition);

    sf::Texture& fjeltreetxtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/fjeltreex.png");
    sf::Sprite fjeltree(fjeltreetxtr);
    fjeltree.setPosition(fjeltreePosition);

    sf::IntRect door1SourceSprite(0, 0, 338, 281);
    sf::Texture& door1txtr = IconManager::getIcon("/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
    sf::Sprite taverndoor(door1txtr, door1SourceSprite);
    taverndoor.setPosition(fjeldoor1Position);

    NPC npc(660.f, 2480.f, font, window);
    bool isPlayerFrozen = false;

    while (window.isOpen())
    {
        Event event;
        Polygon playerOriginal = collisionx;
        Polygon attackxOriginal = attackx;
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            npc.handleInput(event, mousePos, window);

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::I) {
                inventoryUI.isVisible = !inventoryUI.isVisible;
            }
        }

        if (hasPlayed == false) {
            playIntroMovie(window);
        }

        #include </home/z3ta/c++/SoV/main/animations.h>

        playerCenter = Vector2f(player.getPosition());
        playerRight = Vector2f(player.getPosition());
        playerLeft = Vector2f(player.getPosition());

        mousePosWindow = Vector2f(Mouse::getPosition(window));

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
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && !isPlayerFrozen) {
            player.move(playerSpeed, 0.f);
            playerclothes.move(playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && !isPlayerFrozen) {
            player.move(0.f, -playerSpeed);
            playerclothes.move(0.f, -playerSpeed);
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && !isPlayerFrozen) {
            player.move(0.f, playerSpeed);
            playerclothes.move(0.f, playerSpeed);
        } else {
            idle = 1;
        }

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

        mousePos = sf::Mouse::getPosition(window);

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

        if (area == 2) {
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

        if (area == 3) {
            window.draw(wilderness1);
            window.draw(enemy.getSprite());
            window.draw(enemy1.getSprite());
            window.draw(player);
            
            minimap3.draw(window);
            enemy.update(player.getPosition().x, player.getPosition().y, deltaTime);
            enemy1.update(player.getPosition().x, player.getPosition().y, deltaTime);
        }

        if (area == 4) {
            window.draw(fjeltaverninn);
            window.draw(fjelfireplace);
            window.draw(fjelfirebox);
            window.draw(player);
            
            npc.update(playerPosition, mousePos);
            npc.handleInput(event, mousePos, window);
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
