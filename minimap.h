#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class Minimap {
public:
    Minimap(const std::string& areaImagePath, float minimapScale, const sf::Vector2f& fogRadius)
        : scale(minimapScale), fogRadius(fogRadius) {

        // Load the area texture
        if (!backgroundTexture.loadFromFile(areaImagePath)) {
            std::cerr << "Error loading minimap area texture!" << std::endl;
        }

        // Create the background sprite (scaled-down area image)
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(scale, scale); // Scale the image down for the minimap
		
        // Set up the player indicator
        playerIndicator.setRadius(5);
        playerIndicator.setFillColor(sf::Color::White);

        // Create the fog of war overlay
        fogOverlay.setSize(sf::Vector2f(backgroundSprite.getGlobalBounds().width, backgroundSprite.getGlobalBounds().height));
        fogOverlay.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black

        // Create the render texture for fog of war
        fogRenderTexture.create(backgroundSprite.getGlobalBounds().width, backgroundSprite.getGlobalBounds().height);
        fogRenderTexture.clear(sf::Color::Transparent); // Clear with transparent color
    }

    // Updates the minimap and player indicator position
    void update(const sf::Vector2f& playerPosition, const sf::Vector2f& windowSize) {
        // Adjust the minimap's position based on the player's location
        // Here, the minimap is placed at the bottom-left corner
        minimapPosition = playerPosition;
		int indicatorSpeed = 200;
        // Update the player indicator on the minimap (based on player position)
        // Adjust the player's minimap position based on the scale of the minimap
        playerIndicator.setPosition(minimapPosition.x + 20 + (playerPosition.x * scale / 3.77 - playerIndicator.getRadius()),
                                    minimapPosition.y + 130 + (playerPosition.y * scale / 3.77 - playerIndicator.getRadius()));

        // Update the fog of war (hidden area) based on the player's position
        updateFogOfWar(playerPosition);
    }

    // Draw the minimap (background, fog of war, and player indicator)
    void draw(sf::RenderWindow& window) {
        // Draw the minimap background (scaled-down area)
        backgroundSprite.setPosition(minimapPosition);
        window.draw(backgroundSprite);

        // Draw fog of war
        window.draw(fogOverlay);

        // Draw the player indicator
        window.draw(playerIndicator);
    }

private:
    sf::Texture backgroundTexture;  // Area texture for minimap
    sf::Sprite backgroundSprite;    // Scaled background sprite
    sf::CircleShape playerIndicator; // Indicator for player position
    sf::RectangleShape fogOverlay;  // Fog of war overlay
    sf::RenderTexture fogRenderTexture; // Render texture for dynamic fog of war
    sf::Vector2f minimapPosition;  // Position of minimap on screen (fixed in bottom-left corner)
    float scale;  // Scale factor for minimap
    sf::Vector2f fogRadius; // Fog of war radius around the player

    // Updates the fog of war overlay
    void updateFogOfWar(const sf::Vector2f& playerPosition) {
        // Set up a circular "visibility" area around the player
        sf::CircleShape visibilityCircle(fogRadius.x * scale);  // Scale the fog radius
        visibilityCircle.setFillColor(sf::Color::Transparent);
        visibilityCircle.setOutlineColor(sf::Color::Transparent);
        visibilityCircle.setPosition(minimapPosition.x + (playerPosition.x * scale - visibilityCircle.getRadius()),
                                     minimapPosition.y + (playerPosition.y * scale - visibilityCircle.getRadius()));

        // Clear the render texture and draw fog of war elements
        fogRenderTexture.clear(sf::Color::Transparent);
        fogRenderTexture.draw(fogOverlay); // Draw static fog overlay (if any)
        fogRenderTexture.draw(visibilityCircle); // Draw dynamic visibility circle around player
        fogRenderTexture.display(); // Update the render texture

        // Update the fog of war overlay sprite with the new render texture
        fogOverlay.setTexture(&fogRenderTexture.getTexture()); // Set updated texture
    }
};

#endif // MINIMAP_H