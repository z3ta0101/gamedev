#ifndef FEEDBACK_HH
#define FEEDBACK_HH

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

class Feedback {
private:
    sf::RectangleShape feedbackBox_; // Semi-transparent feedback box
    sf::Text feedbackText_;         // Text for feedback messages
    std::vector<std::pair<std::string, float>> messages_; // Queue of messages with timers
    const float messageDuration_;   // Duration each message is visible (seconds)

public:
    Feedback(sf::Font& font)
        : feedbackBox_(sf::Vector2f(400, 150)), messageDuration_(5.0f)
    {
        feedbackBox_.setPosition(500, 1100); // Centered, 300px from bottom (1400x1400 window)
        feedbackBox_.setFillColor(sf::Color(0, 0, 0, 128)); // 50% opacity black

        feedbackText_.setFont(font);
        feedbackText_.setCharacterSize(14);
        feedbackText_.setFillColor(sf::Color::White);
        feedbackText_.setPosition(510, 1110); // 10px padding inside box
    }

    void addMessage(const std::string& message) {
        messages_.emplace_back(message, 0.0f);
        if (messages_.size() > 5) {
            messages_.erase(messages_.begin());
        }
    }

    void update(float deltaTime) {
        for (auto& [message, timer] : messages_) {
            timer += deltaTime;
        }
        messages_.erase(
            std::remove_if(messages_.begin(), messages_.end(),
                [this](const auto& msg) { return msg.second > messageDuration_; }),
            messages_.end()
        );
    }

    void draw(sf::RenderWindow& window) {
        sf::View currentView = window.getView();
        sf::View defaultView(sf::FloatRect(0, 0, 1400, 1400));
        window.setView(defaultView);

        window.draw(feedbackBox_);

        float yOffset = 1110;
        for (const auto& [message, timer] : messages_) {
            float alpha = 255.0f;
            if (timer > messageDuration_ - 1.0f) {
                alpha = 255.0f * (messageDuration_ - timer);
            }
            feedbackText_.setFillColor(sf::Color(255, 255, 255, static_cast<int>(alpha)));
            feedbackText_.setPosition(510, yOffset);
            feedbackText_.setString(message);
            window.draw(feedbackText_);
            yOffset += 20;
        }

        window.setView(currentView);
    }
};

#endif // FEEDBACK_HH
