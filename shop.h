#ifndef SHOP_H
#define SHOP_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

// Item Types
enum class ItemType {
    Weapon,
    Armor,
    Consumable,
    QuestItem,
    Miscellaneous
};

// Effects like buffs or debuffs
struct Effect {
    std::string description;
    int value;
};

// Stat requirements to use certain items
struct UseLimitations {
    int required_stat;
    std::string required_stat_name;
};

// Inventory Item struct
struct Item {
    std::string name;
    int id;
    int quantity;
    ItemType type;
    int value;
    int durability;
    Effect effect;
    bool stackable;
    UseLimitations use_limitations;
    std::string description;
    int maxStackSize;
    std::string iconPath;

    Item(const std::string& name = "", int id = -1, int quantity = 0, const std::string& iconPath = "", ItemType type = ItemType::Miscellaneous,
         int value = 0, int durability = 0, const Effect& effect = {"", 0}, bool stackable = false, const UseLimitations& use_limitations = {0, ""},
         const std::string& description = "", int maxStackSize = 99)
        : name(name), id(id), quantity(quantity), iconPath(iconPath), type(type), value(value),
          durability(durability), effect(effect), stackable(stackable), use_limitations(use_limitations),
          description(description), maxStackSize(maxStackSize) {}
};

class IconManager {
public:
    static sf::Texture& getIcon(const std::string& iconPath) {
        static sf::Texture defaultTexture;
        if (defaultTexture.getSize().x == 0) { // Load default once
            if (!defaultTexture.loadFromFile("default_icon.png")) {
                std::cerr << "Failed to load default texture\n";
                defaultTexture.create(75, 75); // Adjusted to match typical slot size
            }
        }

        if (textures.find(iconPath) == textures.end()) {
            sf::Texture texture;
            if (texture.loadFromFile(iconPath)) {
                textures[iconPath] = texture;
            } else {
                std::cerr << "Error loading texture: " << iconPath << "\n";
                textures[iconPath] = defaultTexture; // Cache default on failure
            }
        }
        return textures[iconPath];
    }

private:
    static std::unordered_map<std::string, sf::Texture> textures;
};

std::unordered_map<std::string, sf::Texture> IconManager::textures;

class Inventory {
public:
    std::vector<Item> items;
    int maxSlots;
    int gold; // Track player gold

    Inventory(int maxSlots) : maxSlots(maxSlots), gold(140) { // Start with 140 gold
        items.resize(maxSlots, Item());
    }

    bool addItem(const Item& item) {
        for (auto& existingItem : items) {
            if (existingItem.id == -1) {
                existingItem = item;
                std::cout << "Added item: " << item.name << "\n";
                return true;
            }
            if (existingItem.id == item.id && existingItem.stackable) {
                int space = existingItem.maxStackSize - existingItem.quantity;
                if (space > 0) {
                    existingItem.quantity += std::min(space, item.quantity);
                    std::cout << "Stacked item: " << item.name << ", Added " << std::min(space, item.quantity) << "\n";
                    return true;
                }
            }
        }
        return false;
    }

    bool removeItem(int itemId) {
        for (auto& item : items) {
            if (item.id == itemId) {
                std::cout << "Removed item: " << item.name << "\n";
                item = Item();
                return true;
            }
        }
        return false;
    }

    Item* getItem(int itemId) {
        for (auto& item : items) {
            if (item.id == itemId) return &item;
        }
        return nullptr;
    }

    bool placeItem(const Item& item, int targetSlotIndex) {
        if (targetSlotIndex < 0 || targetSlotIndex >= maxSlots) return false;

        if (items[targetSlotIndex].id == -1) {
            items[targetSlotIndex] = item;
            std::cout << "Placed item: " << item.name << "\n";
            return true;
        } else if (items[targetSlotIndex].id == item.id && items[targetSlotIndex].stackable) {
            int space = items[targetSlotIndex].maxStackSize - items[targetSlotIndex].quantity;
            if (space >= item.quantity) {
                items[targetSlotIndex].quantity += item.quantity;
                std::cout << "Stacked item in slot: " << item.name << "\n";
                return true;
            }
        }
        Item temp = items[targetSlotIndex];
        items[targetSlotIndex] = item;
        std::cout << "Swapped item: " << item.name << "\n";
        return addItem(temp);
    }

    int getGold() const { return gold; }

    void setGold(int amount) {
        gold = std::max(0, amount);
        std::cout << "Gold set to: " << gold << "\n";
    }

    void saveInventory(const std::string& filename) {
        std::ofstream file(filename);
        file << "Gold: " << gold << "\n"; // Save gold
        for (const auto& item : items) {
            if (item.id != -1) file << item.id << " " << item.quantity << "\n";
        }
    }

    void loadInventory(const std::string& filename) {
        std::ifstream file(filename);
        items.clear();
        items.resize(maxSlots, Item());
        std::string line;
        if (std::getline(file, line) && line.find("Gold: ") == 0) {
            gold = std::stoi(line.substr(6));
            std::cout << "Loaded gold: " << gold << "\n";
        }
        int id, quantity;
        while (file >> id >> quantity) {
            for (auto& item : items) {
                if (item.id == -1) {
                    item.id = id;
                    item.quantity = quantity;
                    break;
                }
            }
        }
    }
};

class ShopUI {
public:
    sf::Sprite shopBackground;
    sf::Sprite playerBackground;
    std::vector<sf::RectangleShape> shopSlots;
    std::vector<sf::RectangleShape> playerSlots;
    Inventory* shopInventory;
    Inventory* playerInventory;
    sf::Texture emptySlotTexture;
    bool isVisible = false;
    sf::Font font;
    sf::Text tooltipText;
    sf::Text quantityText;
    sf::Text goldText; // Text for gold display
    sf::Sprite goldSprite; // Sprite for gold pile
    sf::Texture goldTexture; // Texture for gold pile
    sf::RectangleShape buyButton;
    sf::RectangleShape sellButton;
    sf::Text buyButtonText;
    sf::Text sellButtonText;
    int selectedShopItemIndex = -1;
    int selectedPlayerItemIndex = -1;
    bool buttonWasPressed = false;
    sf::Clock clickTimer;
    float clickThreshold = 0.2f;

    ShopUI(Inventory* shopInv, Inventory* playerInv, const sf::Texture& backgroundTexture, 
           const sf::Texture& emptySlotTexture, const sf::Vector2f& shopPosition, const sf::Vector2f& playerPosition, 
           int rows, int cols)
        : shopInventory(shopInv), playerInventory(playerInv), emptySlotTexture(emptySlotTexture) {
        // Initialize backgrounds
        shopBackground.setTexture(backgroundTexture);
        shopBackground.setPosition(shopPosition);
        shopBackground.setScale(2.f, 2.f);
        playerBackground.setTexture(backgroundTexture);
        playerBackground.setPosition(playerPosition);
        playerBackground.setScale(2.f, 2.f);

        // Load font
        if (!font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
            std::cerr << "Failed to load font\n";
        }

        // Initialize tooltip and quantity text
        tooltipText.setFont(font);
        tooltipText.setCharacterSize(14);
        tooltipText.setFillColor(sf::Color::White);
        quantityText.setFont(font);
        quantityText.setCharacterSize(12);
        quantityText.setFillColor(sf::Color::White);

        // Load gold pile texture
        if (!goldTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/inv/goldpile.png")) {
            std::cerr << "Failed to load gold pile texture\n";
        }
        goldSprite.setTexture(goldTexture);

        // Position gold sprite and text below player's 4x4 item slots, centered horizontally
        float slotSize = 75.f;
        float padding = 10.f;
        float panelWidth = cols * (slotSize + padding) - padding;
        float startY = playerPosition.y + playerBackground.getGlobalBounds().height / 2 - (rows * (slotSize + padding)) / 2;
        float goldSpriteY = startY + rows * (slotSize + padding) + 10.f; // 10px below slots
        float goldTextY = goldSpriteY + 32.f + 5.f; // 32px (sprite height) + 5px below sprite
        float centerX = playerPosition.x + panelWidth / 2;
        goldSprite.setPosition(centerX - 16.f, goldSpriteY); // Center sprite (32/2 = 16)
        goldText.setFont(font);
        goldText.setCharacterSize(16);
        goldText.setFillColor(sf::Color::Yellow);
        goldText.setPosition(centerX, goldTextY);
        goldText.setString(std::to_string(playerInventory->getGold()));
        sf::FloatRect goldBounds = goldText.getLocalBounds();
        goldText.setOrigin(goldBounds.width / 2, 0);

        // Initialize shop slots
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(shopPosition.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                slot.setTexture(&emptySlotTexture);
                slot.setOutlineThickness(1.f);
                slot.setOutlineColor(sf::Color::Transparent);
                shopSlots.push_back(slot);
            }
        }

        // Initialize player slots
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(playerPosition.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                slot.setTexture(&emptySlotTexture);
                slot.setOutlineThickness(1.f);
                slot.setOutlineColor(sf::Color::Transparent);
                playerSlots.push_back(slot);
            }
        }

        // Initialize Buy button
        buyButton.setSize(sf::Vector2f(100.f, 30.f));
        buyButton.setPosition(shopPosition.x + panelWidth / 2 - 50.f, goldSpriteY + 50.f); // Centered below shop slots
        buyButton.setFillColor(sf::Color(50, 50, 50));
        buyButton.setOutlineThickness(1.f);
        buyButton.setOutlineColor(sf::Color::White);
        buyButtonText.setFont(font);
        buyButtonText.setCharacterSize(16);
        buyButtonText.setFillColor(sf::Color::White);
        buyButtonText.setString("Buy");
        sf::FloatRect buyBounds = buyButtonText.getLocalBounds();
        buyButtonText.setPosition(shopPosition.x + panelWidth / 2 - buyBounds.width / 2, goldSpriteY + 50.f + 5.f);

        // Initialize Sell button
        sellButton.setSize(sf::Vector2f(100.f, 30.f));
        sellButton.setPosition(playerPosition.x + panelWidth / 2 - 50.f, goldSpriteY + 50.f); // Centered below player slots
        sellButton.setFillColor(sf::Color(50, 50, 50));
        sellButton.setOutlineThickness(1.f);
        sellButton.setOutlineColor(sf::Color::White);
        sellButtonText.setFont(font);
        sellButtonText.setCharacterSize(16);
        sellButtonText.setFillColor(sf::Color::White);
        sellButtonText.setString("Sell");
        sf::FloatRect sellBounds = sellButtonText.getLocalBounds();
        sellButtonText.setPosition(playerPosition.x + panelWidth / 2 - sellBounds.width / 2, goldSpriteY + 50.f + 5.f);
    }

    void handleInput(const sf::Vector2f& mousePos, bool mousePressed) {
        if (!isVisible) return;

        bool mousePressedThisFrame = mousePressed && !buttonWasPressed;
        if (mousePressedThisFrame) {
            buttonWasPressed = true;
            clickTimer.restart();
        } else if (!mousePressed && buttonWasPressed) {
            buttonWasPressed = false;
            if (clickTimer.getElapsedTime().asSeconds() < clickThreshold) {
                // Check shop slots
                for (size_t i = 0; i < shopSlots.size(); ++i) {
                    if (shopSlots[i].getGlobalBounds().contains(mousePos) && shopInventory->items[i].id != -1) {
                        if (selectedShopItemIndex == i) {
                            selectedShopItemIndex = -1; // Deselect on second click
                        } else {
                            selectedShopItemIndex = i;
                            selectedPlayerItemIndex = -1; // Deselect player item
                        }
                        return;
                    }
                }
                // Check player slots
                for (size_t i = 0; i < playerSlots.size(); ++i) {
                    if (playerSlots[i].getGlobalBounds().contains(mousePos) && playerInventory->items[i].id != -1) {
                        if (selectedPlayerItemIndex == i) {
                            selectedPlayerItemIndex = -1; // Deselect on second click
                        } else {
                            selectedPlayerItemIndex = i;
                            selectedShopItemIndex = -1; // Deselect shop item
                        }
                        return;
                    }
                }
                // Check Buy button
                if (buyButton.getGlobalBounds().contains(mousePos) && selectedShopItemIndex != -1) {
                    Item& item = shopInventory->items[selectedShopItemIndex];
                    if (playerInventory->getGold() >= item.value) {
                        if (playerInventory->addItem(item)) {
                            playerInventory->setGold(playerInventory->getGold() - item.value);
                            shopInventory->removeItem(item.id);
                            std::cout << "Bought item: " << item.name << " for " << item.value << " gold\n";
                            selectedShopItemIndex = -1;
                        } else {
                            std::cout << "Player inventory full, cannot buy: " << item.name << "\n";
                        }
                    } else {
                        std::cout << "Not enough gold to buy: " << item.name << "\n";
                    }
                }
                // Check Sell button
                if (sellButton.getGlobalBounds().contains(mousePos) && selectedPlayerItemIndex != -1) {
                    Item& item = playerInventory->items[selectedPlayerItemIndex];
                    if (item.type == ItemType::Weapon || item.type == ItemType::Armor || item.type == ItemType::Consumable) {
                        playerInventory->setGold(playerInventory->getGold() + item.value);
                        playerInventory->removeItem(item.id);
                        std::cout << "Sold item: " << item.name << " for " << item.value << " gold\n";
                        selectedPlayerItemIndex = -1;
                    } else {
                        std::cout << "Shopkeeper does not buy: " << item.name << "\n";
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        sf::View originalView = window.getView();
        sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        window.setView(newView);

        sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        // Draw backgrounds
        window.draw(shopBackground);
        window.draw(playerBackground);

        // Draw shop slots
        for (size_t i = 0; i < shopSlots.size(); ++i) {
            shopSlots[i].setOutlineColor((i == selectedShopItemIndex || shopSlots[i].getGlobalBounds().contains(mousePos)) ? sf::Color::Yellow : sf::Color::Transparent);
            window.draw(shopSlots[i]);

            if (i < shopInventory->items.size() && shopInventory->items[i].id != -1) {
                sf::Sprite slotSprite(IconManager::getIcon(shopInventory->items[i].iconPath));
                slotSprite.setPosition(shopSlots[i].getPosition());
                window.draw(slotSprite);

                if (shopInventory->items[i].stackable && shopInventory->items[i].quantity > 1) {
                    quantityText.setString(std::to_string(shopInventory->items[i].quantity));
                    quantityText.setPosition(shopSlots[i].getPosition().x + 50, shopSlots[i].getPosition().y + 50);
                    window.draw(quantityText);
                }

                if (shopSlots[i].getGlobalBounds().contains(mousePos)) {
                    std::string tooltip = shopInventory->items[i].name + "\n" +
                                         shopInventory->items[i].description + "\n" +
                                         "Value: " + std::to_string(shopInventory->items[i].value) + "\n" +
                                         "Durability: " + std::to_string(shopInventory->items[i].durability);
                    tooltipText.setString(tooltip);
                    tooltipText.setPosition(mousePos);
                    window.draw(tooltipText);
                }
            }
        }

        // Draw player slots
        for (size_t i = 0; i < playerSlots.size(); ++i) {
            playerSlots[i].setOutlineColor((i == selectedPlayerItemIndex || playerSlots[i].getGlobalBounds().contains(mousePos)) ? sf::Color::Yellow : sf::Color::Transparent);
            window.draw(playerSlots[i]);

            if (i < playerInventory->items.size() && playerInventory->items[i].id != -1) {
                sf::Sprite slotSprite(IconManager::getIcon(playerInventory->items[i].iconPath));
                slotSprite.setPosition(playerSlots[i].getPosition());
                window.draw(slotSprite);

                if (playerInventory->items[i].stackable && playerInventory->items[i].quantity > 1) {
                    quantityText.setString(std::to_string(playerInventory->items[i].quantity));
                    quantityText.setPosition(playerSlots[i].getPosition().x + 50, playerSlots[i].getPosition().y + 50);
                    window.draw(quantityText);
                }

                if (playerSlots[i].getGlobalBounds().contains(mousePos)) {
                    std::string tooltip = playerInventory->items[i].name + "\n" +
                                         playerInventory->items[i].description + "\n" +
                                         "Value: " + std::to_string(playerInventory->items[i].value) + "\n" +
                                         "Durability: " + std::to_string(playerInventory->items[i].durability);
                    tooltipText.setString(tooltip);
                    tooltipText.setPosition(mousePos);
                    window.draw(tooltipText);
                }
            }
        }

        // Draw gold pile and gold text
        window.draw(goldSprite);
        goldText.setString(std::to_string(playerInventory->getGold()));
        sf::FloatRect goldBounds = goldText.getLocalBounds();
        goldText.setOrigin(goldBounds.width / 2, 0); // Re-center if gold changes
        window.draw(goldText);

        // Draw buttons
        window.draw(buyButton);
        window.draw(buyButtonText);
        window.draw(sellButton);
        window.draw(sellButtonText);

        window.setView(originalView);
    }
};

#endif // SHOP_H
