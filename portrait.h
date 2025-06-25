#ifndef PORTRAIT_HH
#define PORTRAIT_HH

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

class Portrait {
private:
    sf::Sprite portrait_;
    sf::Texture texture_;
    sf::RectangleShape healthOverlay_;
    sf::Text healthText_;
    sf::Font font_;
    int& health_;
    const int maxHealth_;

public:
    Portrait(const std::string& portraitPath, sf::Vector2f position, int& health, const int maxHealth)
        : health_(health), maxHealth_(maxHealth)
    {
        if (!texture_.loadFromFile(portraitPath)) {
            std::cerr << "Error loading portrait texture: " << portraitPath << std::endl;
        } else {
            std::cout << "Loaded portrait texture: " << portraitPath 
                      << ", size: " << texture_.getSize().x << "x" << texture_.getSize().y << std::endl;
        }
        portrait_.setTexture(texture_);
        portrait_.setPosition(position);

        // Force 120x190 pixels
        sf::Vector2u textureSize = texture_.getSize();
        if (textureSize.x != 0 && textureSize.y != 0) {
            float scaleX = 120.0f / textureSize.x;
            float scaleY = 190.0f / textureSize.y;
            portrait_.setScale(scaleX, scaleY);
            std::cout << "Portrait scaled to 120x190, scale factors: " << scaleX << "x" << scaleY << std::endl;
        } else {
            std::cerr << "Warning: Texture size is 0, cannot scale portrait!" << std::endl;
        }

        if (!font_.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
            std::cerr << "Error loading font for portrait!" << std::endl;
        }

        // Initialize health overlay (grows upward as health decreases)
        float healthRatio = static_cast<float>(maxHealth_ - health_) / maxHealth_;
        healthOverlay_.setSize(sf::Vector2f(120, 190 * healthRatio));
        healthOverlay_.setPosition(position);
        healthOverlay_.setFillColor(sf::Color(255, 0, 0, 128)); // Transparent red
        // Align overlay to bottom of portrait
        healthOverlay_.setPosition(position.x, position.y + (190 * (1.0f - healthRatio)));

        healthText_.setFont(font_);
        healthText_.setCharacterSize(12);
        healthText_.setFillColor(sf::Color::White);
        healthText_.setPosition(position.x + 5, position.y + 190);
        updateHealthText();
    }

    void setHealth(int health) {
        health_ = std::max(0, std::min(health, maxHealth_));
        // Update overlay size and position
        float healthRatio = static_cast<float>(maxHealth_ - health_) / maxHealth_;
        healthOverlay_.setSize(sf::Vector2f(120, 190 * healthRatio));
        healthOverlay_.setPosition(portrait_.getPosition().x, portrait_.getPosition().y + (190 * (1.0f - healthRatio)));
        updateHealthText();
    }

    void updateHealthText() {
        healthText_.setString("HP: " + std::to_string(health_) + "/" + std::to_string(maxHealth_));
    }

    void update() {
        // Update overlay size and position
        float healthRatio = static_cast<float>(maxHealth_ - health_) / maxHealth_;
        healthOverlay_.setSize(sf::Vector2f(120, 190 * healthRatio));
        healthOverlay_.setPosition(portrait_.getPosition().x, portrait_.getPosition().y + (190 * (1.0f - healthRatio)));
        updateHealthText();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(portrait_);
        window.draw(healthOverlay_); // Draw transparent red overlay
        window.draw(healthText_);
    }

    sf::Sprite& getPortrait() {
        return portrait_;
    }
};

#endif // PORTRAIT_HH
