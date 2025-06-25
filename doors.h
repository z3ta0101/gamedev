#ifndef DOOR_H
#define DOOR_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

extern sf::Sprite player; // Keep for backward compatibility
extern int area; // Keep for backward compatibility
extern sf::Vector2f playerPos; // Declare extern for playerPos

class Door1 {
public:
    Door1(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 828, 674));

        doorPolygon.setPointCount(9);
        doorPolygon.setPoint(0, sf::Vector2f(429, -1977));
        doorPolygon.setPoint(1, sf::Vector2f(429, -2176));
        doorPolygon.setPoint(2, sf::Vector2f(444, -2218));
        doorPolygon.setPoint(3, sf::Vector2f(468, -2231));
        doorPolygon.setPoint(4, sf::Vector2f(493, -2226));
        doorPolygon.setPoint(5, sf::Vector2f(519, -2199));
        doorPolygon.setPoint(6, sf::Vector2f(526, -2170));
        doorPolygon.setPoint(7, sf::Vector2f(532, -2149));
        doorPolygon.setPoint(8, sf::Vector2f(530, -2019));
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
        doorPolygon.setOutlineColor(sf::Color::Transparent);

        closeDoorPolygon.setPointCount(7);
        closeDoorPolygon.setPoint(0, sf::Vector2f(434, -1979));
        closeDoorPolygon.setPoint(1, sf::Vector2f(427, -2181));
        closeDoorPolygon.setPoint(2, sf::Vector2f(419, -2197));
        closeDoorPolygon.setPoint(3, sf::Vector2f(410, -2208));
        closeDoorPolygon.setPoint(4, sf::Vector2f(393, -2192));
        closeDoorPolygon.setPoint(5, sf::Vector2f(389, -2158));
        closeDoorPolygon.setPoint(6, sf::Vector2f(386, -1933));
        closeDoorPolygon.setFillColor(sf::Color::Transparent);
        closeDoorPolygon.setOutlineThickness(2);
        closeDoorPolygon.setOutlineColor(sf::Color::Transparent);
    }

    bool update(const sf::Vector2i& mousePos, sf::RenderWindow& window, sf::Vector2f& playerPosRef) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        float distanceToDoor = std::sqrt(std::pow(playerPosRef.x - doorSprite.getPosition().x, 2) +
                                         std::pow(playerPosRef.y - doorSprite.getPosition().y, 2));
        float interactionThreshold = 500.0f;
        bool transitioned = false;

        if (distanceToDoor <= interactionThreshold) {
            if (doorOpen) {
                bool isHoveringClose = pointInPolygon(closeDoorPolygon, worldMousePos);
                closeDoorPolygon.setOutlineColor(isHoveringClose ? sf::Color::Blue : sf::Color::Transparent);

                bool isHoveringEntry = pointInPolygon(doorPolygon, worldMousePos);
                doorPolygon.setOutlineColor(sf::Color::Transparent);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    if (isHoveringClose) {
                        toggleDoorState(false, playerPosRef);
                        isClicked = true;
                    } else if (isHoveringEntry) {
                        toggleDoorState(true, playerPosRef);
                        isClicked = true;
                        transitioned = true;
                    }
                }
            } else {
                bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
                doorPolygon.setOutlineColor(isHovering ? sf::Color::Blue : sf::Color::Transparent);
                closeDoorPolygon.setOutlineColor(sf::Color::Transparent);

                if (isHovering && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    toggleDoorState(false, playerPosRef);
                    isClicked = true;
                }
            }
        } else {
            doorPolygon.setOutlineColor(sf::Color::Transparent);
            closeDoorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

        return transitioned;
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);
        window.draw(doorPolygon);
        if (doorOpen) {
            window.draw(closeDoorPolygon);
        }
    }

    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;

        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    void toggleDoorState(bool enterArea, sf::Vector2f& playerPosRef) {
        if (enterArea && doorOpen) {
            area = 4;
            playerPosRef = sf::Vector2f(2255.0f, 4110.0f);
            player.setPosition(playerPosRef);
            std::cout << "Transitioning to area 4, playerPos: (" << playerPosRef.x << ", " << playerPosRef.y << ")\n";
        } else {
            doorOpen = !doorOpen;
            doorSprite.setTextureRect(doorOpen ? sf::IntRect(828, 0, 828, 674) : sf::IntRect(0, 0, 828, 674));
        }
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    sf::ConvexShape closeDoorPolygon;
    bool doorOpen;
    bool isClicked;
};

class Door2 {
public:
    Door2(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 570, 1732));

        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(782, 1863));
        doorPolygon.setPoint(1, sf::Vector2f(613, 2154));
        doorPolygon.setPoint(2, sf::Vector2f(613, 2672));
        doorPolygon.setPoint(3, sf::Vector2f(782, 2377));
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    bool update(const sf::Vector2i& mousePos, sf::RenderWindow& window, sf::Vector2f& playerPosRef) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        float distanceToDoor = std::sqrt(std::pow(playerPosRef.x - doorSprite.getPosition().x - 170, 1.8) +
                                         std::pow(playerPosRef.y - doorSprite.getPosition().y - 10, 1.8));
        float interactionThreshold = 400.0f;
        bool transitioned = false;

        if (distanceToDoor <= interactionThreshold) {
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
            doorPolygon.setOutlineColor(isHovering ? sf::Color::Blue : sf::Color::Transparent);

            if (isHovering && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                transitioned = toggleDoorState(playerPosRef);
                isClicked = true;
            }
        } else {
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

        return transitioned;
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);
        window.draw(doorPolygon);
    }

    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;

        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    bool toggleDoorState(sf::Vector2f& playerPosRef) {
        doorOpen = !doorOpen;
        if (doorOpen) {
            doorSprite.setTextureRect(sf::IntRect(570, 0, 570, 1732));
            area = 6;
            playerPosRef = sf::Vector2f(1000.0f, 1000.0f);
            player.setPosition(playerPosRef);
            std::cout << "Transitioning to area 6, playerPos: (" << playerPosRef.x << ", " << playerPosRef.y << ")\n";
            return true;
        } else {
            doorSprite.setTextureRect(sf::IntRect(0, 0, 570, 1732));
            return false;
        }
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;
    bool isClicked;
};

class Door3 {
public:
    Door3(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 184, 537));

        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(8010, -3034));
        doorPolygon.setPoint(1, sf::Vector2f(8010, -2905));
        doorPolygon.setPoint(2, sf::Vector2f(7939, -3029));
        doorPolygon.setPoint(3, sf::Vector2f(7939, -3157));
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    bool update(const sf::Vector2i& mousePos, sf::RenderWindow& window, sf::Vector2f& playerPosRef) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        float distanceToDoor = std::sqrt(std::pow(playerPosRef.x - doorSprite.getPosition().x, 2) +
                                         std::pow(playerPosRef.y - doorSprite.getPosition().y, 2));
        float interactionThreshold = 400.0f;
        bool transitioned = false;

        if (distanceToDoor <= interactionThreshold) {
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
            doorPolygon.setOutlineColor(isHovering ? sf::Color::Blue : sf::Color::Transparent);

            if (isHovering && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                transitioned = toggleDoorState(playerPosRef);
                isClicked = true;
            }
        } else {
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "World Mouse Position: (" << worldMousePos.x << ", " << worldMousePos.y << ")\n";
        }

        return transitioned;
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);
        window.draw(doorPolygon);
    }

    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;

        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    bool toggleDoorState(sf::Vector2f& playerPosRef) {
        doorOpen = !doorOpen;
        if (doorOpen) {
            doorSprite.setTextureRect(sf::IntRect(184, 0, 184, 537));
            area = 7;
            playerPosRef = sf::Vector2f(2000.0f, 2000.0f);
            player.setPosition(playerPosRef);
            std::cout << "Transitioning to area 7, playerPos: (" << playerPosRef.x << ", " << playerPosRef.y << ")\n";
            return true;
        } else {
            doorSprite.setTextureRect(sf::IntRect(0, 0, 184, 537));
            return false;
        }
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;
    bool isClicked;
};

class Door4 {
public:
    Door4(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 410, 794));

        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(9898, 1217));
        doorPolygon.setPoint(1, sf::Vector2f(9898, 1467));
        doorPolygon.setPoint(2, sf::Vector2f(10052, 1622));
        doorPolygon.setPoint(3, sf::Vector2f(10052, 1371));
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    bool update(const sf::Vector2i& mousePos, sf::RenderWindow& window, sf::Vector2f& playerPosRef) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        float distanceToDoor = std::sqrt(std::pow(playerPosRef.x - doorSprite.getPosition().x, 2) +
                                         std::pow(playerPosRef.y - doorSprite.getPosition().y, 2));
        float interactionThreshold = 700.0f;
        bool transitioned = false;

        if (distanceToDoor <= interactionThreshold) {
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
            doorPolygon.setOutlineColor(isHovering ? sf::Color::Blue : sf::Color::Transparent);

            if (isHovering && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                transitioned = toggleDoorState(playerPosRef);
                isClicked = true;
            }
        } else {
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "World Mouse Position: (" << worldMousePos.x << ", " << worldMousePos.y << ")\n";
        }

        return transitioned;
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);
        window.draw(doorPolygon);
    }

    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;

        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    bool toggleDoorState(sf::Vector2f& playerPosRef) {
        doorOpen = !doorOpen;
        if (doorOpen) {
            doorSprite.setTextureRect(sf::IntRect(410, 0, 410, 794));
            area = 8;
            playerPosRef = sf::Vector2f(3000.0f, 3000.0f);
            player.setPosition(playerPosRef);
            std::cout << "Transitioning to area 8, playerPos: (" << playerPosRef.x << ", " << playerPosRef.y << ")\n";
            return true;
        } else {
            doorSprite.setTextureRect(sf::IntRect(0, 0, 410, 794));
            return false;
        }
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;
    bool isClicked;
};

class PolyDoor {
public:
    PolyDoor()
        : doorOpen(false), isClicked(false) {
        doorPolygon.setPointCount(8);
        doorPolygon.setPoint(0, sf::Vector2f(2531, 3898));
        doorPolygon.setPoint(1, sf::Vector2f(2531, 3621));
        doorPolygon.setPoint(2, sf::Vector2f(2540, 3578));
        doorPolygon.setPoint(3, sf::Vector2f(2585, 3561));
        doorPolygon.setPoint(4, sf::Vector2f(2604, 3564));
        doorPolygon.setPoint(5, sf::Vector2f(2639, 3571));
        doorPolygon.setPoint(6, sf::Vector2f(2655, 3608));
        doorPolygon.setPoint(7, sf::Vector2f(2667, 3898));
        doorPolygon.setFillColor(sf::Color::Blue);
        doorPolygon.setOutlineThickness(2);
        doorPolygon.setOutlineColor(sf::Color::Transparent);

        closeDoorPolygon.setPointCount(8);
        closeDoorPolygon.setPoint(0, sf::Vector2f(2431, 3898));
        closeDoorPolygon.setPoint(1, sf::Vector2f(2431, 3621));
        closeDoorPolygon.setPoint(2, sf::Vector2f(2440, 3578));
        closeDoorPolygon.setPoint(3, sf::Vector2f(2485, 3561));
        closeDoorPolygon.setPoint(4, sf::Vector2f(2504, 3564));
        closeDoorPolygon.setPoint(5, sf::Vector2f(2539, 3571));
        closeDoorPolygon.setPoint(6, sf::Vector2f(2555, 3608));
        closeDoorPolygon.setPoint(7, sf::Vector2f(2567, 3898));
        closeDoorPolygon.setFillColor(sf::Color::Transparent);
        closeDoorPolygon.setOutlineThickness(2);
        closeDoorPolygon.setOutlineColor(sf::Color::Transparent);
    }

    bool update(const sf::Vector2i& mousePos, sf::RenderWindow& window, sf::Vector2f& playerPosRef) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
        sf::Vector2f doorCenter(2600, 3700);
        float distanceToDoor = std::sqrt(std::pow(playerPosRef.x - doorCenter.x, 2) +
                                         std::pow(playerPosRef.y - doorCenter.y, 2));
        float interactionThreshold = 500.0f;
        bool transitioned = false;

        if (distanceToDoor <= interactionThreshold) {
            if (doorOpen) {
                bool isHoveringClose = pointInPolygon(closeDoorPolygon, worldMousePos);
                closeDoorPolygon.setFillColor(isHoveringClose ? sf::Color::Blue : sf::Color::Transparent);
                closeDoorPolygon.setOutlineColor(isHoveringClose ? sf::Color::Blue : sf::Color::Transparent);

                bool isHoveringEntry = pointInPolygon(doorPolygon, worldMousePos);
                doorPolygon.setFillColor(sf::Color::Transparent);
                doorPolygon.setOutlineColor(sf::Color::Transparent);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    if (isHoveringClose) {
                        toggleDoorState(false, playerPosRef);
                        isClicked = true;
                    } else if (isHoveringEntry) {
                        toggleDoorState(true, playerPosRef);
                        isClicked = true;
                        transitioned = true;
                    }
                }
            } else {
                bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
                doorPolygon.setFillColor(isHovering ? sf::Color::Blue : sf::Color(0, 0, 255, 128));
                doorPolygon.setOutlineColor(isHovering ? sf::Color::Blue : sf::Color::Transparent);
                closeDoorPolygon.setFillColor(sf::Color::Transparent);
                closeDoorPolygon.setOutlineColor(sf::Color::Transparent);

                if (isHovering && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    toggleDoorState(false, playerPosRef);
                    isClicked = true;
                }
            }
        } else {
            doorPolygon.setFillColor(sf::Color(0, 0, 255, 128));
            doorPolygon.setOutlineColor(sf::Color::Transparent);
            closeDoorPolygon.setFillColor(sf::Color::Transparent);
            closeDoorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

        return transitioned;
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorPolygon);
        if (doorOpen) {
            window.draw(closeDoorPolygon);
        }
    }

    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;

        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    void toggleDoorState(bool enterArea, sf::Vector2f& playerPosRef) {
        if (enterArea && doorOpen) {
            area = 5;
            playerPosRef = sf::Vector2f(498.0f, -1808.0f);
            player.setPosition(playerPosRef);
            std::cout << "Transitioning to area 5, playerPos: (" << playerPosRef.x << ", " << playerPosRef.y << ")\n";
        } else {
            doorOpen = !doorOpen;
            doorPolygon.setFillColor(doorOpen ? sf::Color::Transparent : sf::Color::Blue);
            closeDoorPolygon.setFillColor(doorOpen ? sf::Color::Blue : sf::Color::Transparent);
        }
    }

private:
    sf::ConvexShape doorPolygon;
    sf::ConvexShape closeDoorPolygon;
    bool doorOpen;
    bool isClicked;
};

#endif // DOOR_H
