#ifndef NPC_H
#define NPC_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class NPC {
public:
    NPC(float startX, float startY, sf::Font& font, sf::RenderWindow& window)
        : x(startX), y(startY), window(window), customCursorActive(false), isActive(false), selectedOption(-1) {

        std::cout << "Loading NPC texture..." << std::endl;
        
        if (!texture.loadFromFile("/home/z3ta/c++/SoV/images/sprites/dwarfidlespritesheet.png")) {
            std::cerr << "Error loading NPC texture!" << std::endl;
        } else {
            std::cout << "NPC texture loaded successfully." << std::endl;
        }

        sf::IntRect rectSourceSprite;
        sprite.setTexture(texture);
        rectSourceSprite = sf::IntRect(0, 0, 27, 45);  // Assuming each sprite frame is 27x45 (adjust as needed)
        sprite.setTextureRect(rectSourceSprite);
        sf::Vector2f sizeIncrease(1.4f, 1.4f);
        sprite.setScale(sf::Vector2f(sprite.getScale().x + sizeIncrease.x, sprite.getScale().y + sizeIncrease.y));
        sprite.setPosition(x, y);

        // Initialize the conversation box at fixed window position
        box.setSize(sf::Vector2f(1100, 200));  // Adjust box size if needed
        box.setFillColor(sf::Color(0, 0, 0, 180));  // Semi-transparent black
        box.setPosition(400.f, 800.f);  // This should always appear at the same position on screen

        npcText.setFont(font);
        npcText.setCharacterSize(20);
        npcText.setFillColor(sf::Color::White);
        npcText.setPosition(410.f, 810.f);  // Text should appear just above the box

        // Initialize options with the correct size (maxOptions)
        options.resize(maxOptions);  // Ensure we have maxOptions number of elements
        for (int i = 0; i < maxOptions; ++i) {
            options[i].setFont(font);
            options[i].setCharacterSize(18);
            options[i].setFillColor(sf::Color::Red);
            options[i].setPosition(410.f, 850.f + (i * 30));  // Position options below the text
        }

        std::cout << "NPC initialized with conversation box." << std::endl;
    }

    void update(sf::Vector2f playerPos, sf::Vector2i mousePos) {
		sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
		
		
        // Change cursor if the mouse is over the NPC
        if (sprite.getGlobalBounds().contains(worldMousePos)) {
            if (!customCursorActive) {
                changeMouseCursor("/home/z3ta/c++/SoV/images/sprites/mouse/mousesocial.png");
                customCursorActive = true;
            }
        } else if (customCursorActive) {
            resetToDefaultCursor();
            customCursorActive = false;
        }

        // Update mouse hover only if the conversation box is active
        if (isActive) {
            updateMouseHover(mousePos, window);
        }
    }

    void handleInput(const sf::Event& event, sf::Vector2i mousePos, sf::RenderWindow& window) {
    	// Handle mouse button press
    	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {

        	// Get mouse position in window coordinates
        	sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        	sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        	// Check if the player clicked on the NPC sprite to start the conversation
        	if (sprite.getGlobalBounds().contains(mousePos)) {
            	if (!isActive) {
                	// Start the conversation and display the first set of options
                	isActive = true;
                	setText("Hello, traveler! How can I help you?");
                	setOptions({"Ask about the quest", "Tell me about yourself", "Goodbye"});
                	std::cout << "Conversation box activated.\n";
            	}
        	} 
        	// Check if the player clicked inside the conversation box
        	else if (isActive && box.getGlobalBounds().contains(static_cast<float>(windowMousePos.x), static_cast<float>(windowMousePos.y))) {
            	// Get the selected option based on the hover logic
            	int clickedOption = getSelectedOption();

            	// Only proceed if an option was actually clicked
            	if (clickedOption == -1) return;

            	std::cout << "Option clicked: " << clickedOption << "\n";

            	// Based on the selected option, update the dialogue text and options
            	if (clickedOption == 0) {
                	setText("You asked about the quest! Here's more information.");
                	setOptions({"Continue the quest", "Goodbye"});
            	} 
            	else if (clickedOption == 1) {
                	setText("You asked about me! I'm an NPC with much knowledge.");
                	setOptions({"Ask about the quest", "Goodbye"});
            	} 
            	else if (clickedOption == 2) {
                	setText("Goodbye, traveler!");
                	isActive = false; // End the conversation when 'Goodbye' is selected
                	std::cout << "Conversation ended.\n";
            	}
        	}
    	}
	}

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);  // Always draw NPC sprite

        if (isActive) {
            drawConversationBox(window);  // Draw conversation box if active
        }
    }

    bool getIsActive() const {
        return isActive;
    }

    void setText(const std::string& npcDialogue) {
        npcText.setString(npcDialogue);
        std::cout << "NPC text set to: " << npcDialogue << std::endl;
    }

    void setOptions(const std::vector<std::string>& playerOptions) {
        for (size_t i = 0; i < playerOptions.size() && i < maxOptions; ++i) {
            options[i].setString(playerOptions[i]);
        }
    }

    int getSelectedOption() const {
        return selectedOption;
    }

private:
    void updateMouseHover(sf::Vector2i mousePos, sf::RenderWindow& window) {
    	selectedOption = -1;  // Reset the selected option

    	// Convert the mouse position to world coordinates
    	sf::Vector2f windowMousePos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    	std::cout << "Mouse Hover Position: (" << windowMousePos.x << ", " << windowMousePos.y << ")\n";

    	// Check each option's bounding box to see if the mouse is over it
    	for (size_t i = 0; i < options.size(); ++i) {
        	sf::FloatRect optionBounds = options[i].getGlobalBounds();
        	if (optionBounds.contains(windowMousePos)) {
            	selectedOption = i;
            	std::cout << "Mouse hovering over option " << selectedOption << "\n";
            	break; // Stop once we find the hovered option
        	}
    	}
	}

    void drawConversationBox(sf::RenderWindow& window) {
    	if (isActive) {
        	// Save the current view (in case you have camera transformations)
        	sf::View originalView = window.getView();

        	// Dynamically update the view to match the window size
        	sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        	window.setView(newView);

        	// Get the mouse position in window coordinates
        	sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        	sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);
        
        	// Print out the window and mouse position for debugging
        	std::cout << "Window Size: " << window.getSize().x << "x" << window.getSize().y << std::endl;
        	std::cout << "Mouse Position (world coordinates): (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;

        	// Now draw UI elements in screen space
        	window.draw(box);  // Draw the conversation box
        	window.draw(npcText);  // Draw NPC dialogue

        	// Draw the options
        	for (size_t i = 0; i < options.size(); ++i) {
            	// Set color based on hover state
            	options[i].setFillColor(i == selectedOption ? sf::Color::White : sf::Color::Red);
            	window.draw(options[i]);  // Draw each option
        	}

        	// Restore the original view
        	window.setView(originalView);
    	}
	}

    void changeMouseCursor(const std::string& cursorPath) {
        sf::Image cursorImage;
        if (!cursorImage.loadFromFile(cursorPath)) {
            std::cerr << "Error loading cursor image from path: " << cursorPath << std::endl;
            resetToDefaultCursor();
            return;
        }

        sf::Cursor customCursor;
        if (customCursor.loadFromPixels(cursorImage.getPixelsPtr(),
                                        sf::Vector2u(cursorImage.getSize().x, cursorImage.getSize().y),
                                        sf::Vector2u(0, 0))) {
            window.setMouseCursor(customCursor);
        } else {
            std::cerr << "Failed to create cursor from image!" << std::endl;
            resetToDefaultCursor();
        }
    }

    void resetToDefaultCursor() {
        sf::Cursor defaultCursor;
        if (defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
            window.setMouseCursor(defaultCursor);  // Use the default cursor
        } else {
            std::cerr << "Error loading default cursor!" << std::endl;
        }
    }

    float x, y;
    bool customCursorActive;
    bool isActive;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::RectangleShape box;
    sf::Text npcText;
    std::vector<sf::Text> options;
    int selectedOption;
    static const int maxOptions = 3;
    sf::RenderWindow& window;
};

#endif // NPC_H