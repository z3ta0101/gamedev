#ifndef REGION_MAP_H
#define REGION_MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "./inv.h" // For IconManager

// Declare external variables from SoVengine.c++
extern int area;
extern sf::Vector2f playerPos;

struct AreaIcon {
    sf::Sprite sprite;
    sf::Vector2f position;
    int targetArea;
    sf::Vector2f targetPos; // Player spawn point in target area
    AreaIcon(const std::string& iconPath, sf::Vector2f pos, int area, sf::Vector2f tPos)
        : position(pos), targetArea(area), targetPos(tPos) {
        sprite.setTexture(IconManager::getIcon(iconPath));
        sprite.setPosition(pos);
        // Scale to 50x50
        sf::Vector2u textureSize = sprite.getTexture()->getSize();
        sprite.setScale(50.f / textureSize.x, 50.f / textureSize.y);
    }
};

class RegionMapTrigger {
public:
    RegionMapTrigger(float posX, float posY, sf::RenderWindow& window)
        : position(posX, posY), window(window), isClickable(false), isMapVisible(false), 
          customCursorActive(false), wasLeftPressed(false) {
        // Initialize invisible polygon (500x700)
        triggerArea.setSize(sf::Vector2f(500.f, 700.f));
        triggerArea.setPosition(posX, posY);
        triggerArea.setFillColor(sf::Color::Transparent);
        triggerArea.setOutlineThickness(0.f);

        // Load region map texture
        if (!mapTexture.loadFromFile("/home/z3ta/c++/SoV/images/maps/region1.png")) {
            std::cerr << "Failed to load region map texture\n";
        }
        mapSprite.setTexture(mapTexture);
        // Center map in window (assumes 1920x1080 window)
        sf::Vector2u textureSize = mapTexture.getSize();
        mapSprite.setPosition((1920.f - textureSize.x) / 2, (1080.f - textureSize.y) / 2);

        // Define area icons with target positions
        areaIcons.emplace_back("/home/z3ta/c++/SoV/images/maps/mountainpassicon.png", sf::Vector2f(600.f, 400.f), 4, sf::Vector2f(2100.f, 2400.f)); // Pass
        areaIcons.emplace_back("/home/z3ta/c++/SoV/images/maps/wildernessicon.png", sf::Vector2f(620.f, 770.f), 3, sf::Vector2f(10702, -2370)); // Wilderness
        areaIcons.emplace_back("/home/z3ta/c++/SoV/images/maps/fjelicon.png", sf::Vector2f(700.f, 570.f), 5, sf::Vector2f(4700.f, 500.f)); // Town

        std::cout << "RegionMapTrigger initialized at (" << posX << ", " << posY << ")\n";
    }

    void update(sf::Vector2f playerPos, sf::Vector2i mousePos) {
        // Check player proximity (within 1000 pixels)
        float distance = std::sqrt(std::pow(playerPos.x - position.x, 2) + std::pow(playerPos.y - position.y, 2));
        isClickable = distance <= 1000.f;

        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Change cursor if over trigger area and clickable
        if (isClickable && triggerArea.getGlobalBounds().contains(worldMousePos)) {
            if (!customCursorActive) {
                changeMouseCursor("/home/z3ta/c++/SoV/images/maps/mapicon.png");
                customCursorActive = true;
            }
        } else if (customCursorActive) {
            resetToDefaultCursor();
            customCursorActive = false;
        }
    }

    void handleInput(const sf::Event& event, sf::Vector2i mousePos) {
        if (!isClickable) return;

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (!wasLeftPressed) {
                wasLeftPressed = true;
                sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
                if (triggerArea.getGlobalBounds().contains(worldMousePos)) {
                    // Toggle map visibility
                    isMapVisible = !isMapVisible;
                    std::cout << "Region map " << (isMapVisible ? "opened" : "closed") << "\n";
                } else if (isMapVisible) {
                    // Check area icon clicks in screen coordinates
                    sf::Vector2f screenMousePos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    for (const auto& icon : areaIcons) {
                        if (icon.sprite.getGlobalBounds().contains(screenMousePos)) {
                            area = icon.targetArea;
                            playerPos = icon.targetPos;
                            isMapVisible = false;
                            std::cout << "Transitioning to area: " << icon.targetArea 
                                      << " at position (" << playerPos.x << ", " << playerPos.y << ")\n";
                            break;
                        }
                    }
                }
            }
        } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            wasLeftPressed = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        // Draw trigger area (invisible, but useful for debugging)
        window.draw(triggerArea);

        if (isMapVisible) {
            sf::View originalView = window.getView();
            sf::View uiView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
            window.setView(uiView);

            // Draw region map
            window.draw(mapSprite);

            // Draw area icons
            for (const auto& icon : areaIcons) {
                window.draw(icon.sprite);
            }

            window.setView(originalView);
        }
    }

    bool isVisible() const { return isMapVisible; }

private:
    void changeMouseCursor(const std::string& cursorPath) {
        sf::Image cursorImage;
        if (!cursorImage.loadFromFile(cursorPath)) {
            std::cerr << "Error loading cursor image: " << cursorPath << "\n";
            resetToDefaultCursor();
            return;
        }

        sf::Cursor customCursor;
        if (customCursor.loadFromPixels(cursorImage.getPixelsPtr(),
                                       sf::Vector2u(cursorImage.getSize().x, cursorImage.getSize().y),
                                       sf::Vector2u(0, 0))) {
            window.setMouseCursor(customCursor);
        } else {
            std::cerr << "Failed to create cursor from image!\n";
            resetToDefaultCursor();
        }
    }

    void resetToDefaultCursor() {
        sf::Cursor defaultCursor;
        if (defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
            window.setMouseCursor(defaultCursor);
        } else {
            std::cerr << "Failed to load default cursor!\n";
        }
    }

    sf::Vector2f position;
    sf::RectangleShape triggerArea;
    sf::Texture mapTexture;
    sf::Sprite mapSprite;
    std::vector<AreaIcon> areaIcons;
    bool isClickable;
    bool isMapVisible;
    bool customCursorActive;
    bool wasLeftPressed;
    sf::RenderWindow& window;
};

#endif // REGION_MAP_H
