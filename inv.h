#ifndef INV_H
#define INV_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

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
    int value;  // can represent various effects, like health change or stat buff
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
    std::string iconPath; // Store icon path

    Item(const std::string& name, int id, int quantity, const std::string& iconPath, ItemType type,
         int value, int durability, const Effect& effect, bool stackable, const UseLimitations& use_limitations,
         const std::string& description, int maxStackSize = 99)
        : name(name), id(id), quantity(quantity), iconPath(iconPath), type(type), value(value),
          durability(durability), effect(effect), stackable(stackable), use_limitations(use_limitations),
          description(description), maxStackSize(maxStackSize) {}
};

class IconManager {
public:
    static sf::Texture& getIcon(const std::string& iconPath) {
        // Check if the texture has already been loaded
        if (textures.find(iconPath) == textures.end()) {
            sf::Texture texture;
            if (texture.loadFromFile(iconPath)) {
                textures[iconPath] = texture;
            } else {
                // Handle loading error (maybe return a default texture)
                std::cerr << "Error loading texture: " << iconPath << std::endl;
            }
        }
        return textures[iconPath];
    }

private:
    static std::unordered_map<std::string, sf::Texture> textures;  // Cache of textures
};

std::unordered_map<std::string, sf::Texture> IconManager::textures;

// Slot where an item can be placed in ground (inventory world)
class GroundSlot {
public:
    sf::RectangleShape area;        // The background area of the slot
    sf::Sprite itemSprite;          // The item sprite to be placed inside the slot
    Item* item;                     // The item placed in the slot (nullptr if empty)
    const sf::Texture& groundSlotTexture; // The default texture for an empty slot

    // Constructor: Initialize ground slot with texture and size
    GroundSlot(sf::Vector2f position, const sf::Texture& groundSlotTexture, float slotSize)
        : groundSlotTexture(groundSlotTexture), item(nullptr) {
        area.setSize(sf::Vector2f(slotSize, slotSize));  // Set the slot size
        area.setPosition(position);                      // Set the position
        area.setTexture(&groundSlotTexture);             // Set default texture (empty slot)
    }

    // Check if the mouse is inside the ground slot
    bool contains(const sf::Vector2f& mousePos) const {
        return area.getGlobalBounds().contains(mousePos);
    }

    // Place item inside the ground slot
    void placeItem(Item* newItem) {
        if (newItem && !item) {
            item = newItem;
            itemSprite.setTexture(IconManager::getIcon(item->iconPath));  // Use IconManager to get texture
            sf::FloatRect spriteBounds = itemSprite.getGlobalBounds();

            // Center the item sprite within the slot area
            itemSprite.setPosition(area.getPosition().x + (area.getSize().x - spriteBounds.width) / 2,
                                   area.getPosition().y + (area.getSize().y - spriteBounds.height) / 2);
        }
    }

    // Remove item from the ground slot
    void removeItem() {
        item = nullptr;
        // Optionally, you can clear the sprite's texture to visually reset the slot:
        itemSprite.setTexture(groundSlotTexture); // Sets an empty texture
    }

    // Draw ground slot and the item if present
    void draw(sf::RenderWindow& window) {
        window.draw(area);  // Draw the ground slot background

        if (item) {
            itemSprite.setTexture(IconManager::getIcon(item->iconPath));
            // You might want to recalculate the position here if the slot size changes
            sf::FloatRect spriteBounds = itemSprite.getGlobalBounds();
            itemSprite.setPosition(area.getPosition().x + (area.getSize().x - spriteBounds.width) / 2,
                                area.getPosition().y + (area.getSize().y - spriteBounds.height) / 2);
            window.draw(itemSprite);
        }
    }
};

// Main Inventory system
class Inventory {
public:
    std::vector<Item> items;  // Items in the inventory
    int maxSlots;             // Max number of items that can be in the inventory

    Inventory(int maxSlots) : maxSlots(maxSlots) {}

    // Add an item to the inventory
    bool addItem(const Item& item) {
        if (item.stackable) {
            for (auto& existingItem : items) {
                if (existingItem.id == item.id && existingItem.stackable) {
                    int availableSpace = existingItem.maxStackSize - existingItem.quantity;
                    if (availableSpace > 0) {
                        if (item.quantity <= availableSpace) {
                            existingItem.quantity += item.quantity;
                            return true;
                        } else {
                            existingItem.quantity = existingItem.maxStackSize;
                            int remainingQuantity = item.quantity - availableSpace;
                            Item newItem = item;
                            newItem.quantity = remainingQuantity;
                            items.push_back(newItem);
                            return true;
                        }
                    }
                }
            }
        }

        if (items.size() < maxSlots) {
            items.push_back(item);
            return true;
        }

        return false;
    }

    // Remove an item from the inventory by its ID
    bool removeItem(int itemId) {
        auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
            return item.id == itemId;
        });
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }

    // Get an item by its ID
    Item* getItem(int itemId) {
        auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
            return item.id == itemId;
        });
        return (it != items.end()) ? &(*it) : nullptr;
    }

    // Place an item back into the inventory
    bool placeItem(const Item& item) {
        // If the item is stackable, attempt to merge with an existing stack
        if (item.stackable) {
            for (auto& existingItem : items) {
                if (existingItem.id == item.id) { // Same item type
                    int availableSpace = existingItem.maxStackSize - existingItem.quantity;
                    if (availableSpace > 0) {
                        if (item.quantity <= availableSpace) {
                            existingItem.quantity += item.quantity;
                            return true; // Successfully merged
                        } else {
                            existingItem.quantity = existingItem.maxStackSize;
                            Item newItem = item;
                            newItem.quantity = item.quantity - availableSpace;
                            return placeItem(newItem); // Recursively place remaining items
                        }
                    }
                }
            }
        }

        // If no stacking is possible, place in an empty slot if available
        if (items.size() < maxSlots) {
            items.push_back(item);
            return true;
        }

        return false; // No space left
    }

    
    
};

// Member variables:
sf::Clock clickTimer;  // Move clock outside draw(), so it persists across frames.
bool buttonWasPressed = false;  // Tracks if the mouse was pressed in the previous frame
bool clickDetected = false;     // Tracks if a full click (press and release) was detected
const float clickThreshold = 0.2f;  // Time in seconds to consider a click (e.g., 200 ms)

class InventoryUI {
public:
    sf::Sprite background;
    std::vector<sf::RectangleShape> itemSlots;
    std::vector<GroundSlot> groundSlots;
    Inventory* inventory;
    sf::Texture emptySlotTexture;
    int draggedItemIndex = -1;
    Item* draggedItem = nullptr;
    bool isDragging = false;
    sf::Sprite draggedItemSprite;
    bool isVisible = false;
    sf::Font font;
    sf::Text tooltipText;
    bool buttonWasPressed = false;
    bool clickDetected = false;
    sf::Clock clickTimer;
    float clickThreshold = 0.2f;

    InventoryUI(Inventory* inventory, const sf::Texture& backgroundTexture, 
                const sf::Texture& emptySlotTexture, const sf::Vector2f& position, 
                int rows, int cols, const sf::Texture& groundSlotTexture)
        : inventory(inventory), emptySlotTexture(emptySlotTexture) {
        
        background.setTexture(backgroundTexture);
        background.setPosition(position);
        background.setScale(2.f, 2.f);

        font.loadFromFile(ASSETS_DIR "texgyrechorus-mediumitalic.otf");
        tooltipText.setFont(font);
        tooltipText.setCharacterSize(14);
        tooltipText.setFillColor(sf::Color::White);

        float slotSize = 75.f;
        float padding = 10.f;
        float startY = position.y + background.getGlobalBounds().height / 2 - (rows * (slotSize + padding)) / 2;

        // Create inventory slots
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(position.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                itemSlots.push_back(slot);
            }
        }

        // Create ground slots
        float groundSlotX = position.x + background.getGlobalBounds().width - 160.f;
        float groundSlotY = position.y + background.getGlobalBounds().height / 2 - 200.f;

        for (int i = 0; i < 5; ++i) {
            groundSlots.emplace_back(sf::Vector2f(groundSlotX, groundSlotY + (i * (slotSize + padding))), groundSlotTexture, slotSize);
        }
    }

    void startDragging(int index) {
        if (index >= 0 && index < inventory->items.size()) {
            draggedItemIndex = index;
            draggedItem = &inventory->items[index];
            isDragging = true;
            draggedItemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
        }
    }

    void stopDragging(const sf::Vector2f& mousePos) {
        if (!isDragging || !draggedItem) return;

        bool itemPlaced = false;

        for (auto& groundSlot : groundSlots) {
            if (groundSlot.area.getGlobalBounds().contains(mousePos)) {
                if (groundSlot.item == nullptr) {
                    // Use IconManager to set the texture of the itemSprite
                    groundSlot.itemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
                    groundSlot.placeItem(draggedItem);
                    inventory->removeItem(draggedItem->id);
                    itemPlaced = true;
                    break;
                }
            }
        }

        if (!itemPlaced) {
            for (size_t i = 0; i < itemSlots.size(); ++i) {
                if (itemSlots[i].getGlobalBounds().contains(mousePos)) {
                    if (i != draggedItemIndex) {
                        inventory->placeItem(*draggedItem);
                        inventory->removeItem(draggedItem->id);
                        itemPlaced = true;
                        break;
                    }
                }
            }
        }

        if (itemPlaced) {
            draggedItemIndex = -1;
            draggedItem = nullptr;
            isDragging = false;
        }
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        sf::View originalView = window.getView();
        sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        window.setView(newView);

        sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        window.draw(background);

        for (size_t i = 0; i < itemSlots.size(); ++i) {
            sf::Sprite slotSprite;
            if (i < inventory->items.size() && (i != draggedItemIndex || !isDragging)) {
                slotSprite.setTexture(IconManager::getIcon(inventory->items[i].iconPath));
            } else {
                slotSprite.setTexture(emptySlotTexture);
            }

            slotSprite.setPosition(itemSlots[i].getPosition());
            window.draw(slotSprite);

            if (slotSprite.getGlobalBounds().contains(mousePos)) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonWasPressed) {
                    buttonWasPressed = true;
                    clickDetected = false;
                    clickTimer.restart();
                }

                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonWasPressed) {
                    if (clickDetected && !isDragging) {
                        startDragging(i);
                    }
                    buttonWasPressed = false;
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clickTimer.getElapsedTime().asSeconds() < clickThreshold) {
                    clickDetected = true;
                }

                if (i < inventory->items.size()) {
                    tooltipText.setString(inventory->items[i].description);
                    tooltipText.setPosition(mousePos);
                    window.draw(tooltipText);
                }
            }

            if (i < inventory->items.size() && i != draggedItemIndex) {
                sf::Sprite itemSprite(IconManager::getIcon(inventory->items[i].iconPath));
                itemSprite.setPosition(itemSlots[i].getPosition());
                window.draw(itemSprite);
            }
        }

        for (auto& groundSlot : groundSlots) {
            groundSlot.draw(window);

            if (groundSlot.area.getGlobalBounds().contains(mousePos)) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !buttonWasPressed) {
                    buttonWasPressed = true;
                    clickDetected = false;
                    clickTimer.restart();
                }

                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && buttonWasPressed) {
                    if (isDragging && draggedItem) {
                        if (groundSlot.item == nullptr) {
                            groundSlot.removeItem();  // Clear existing item from the slot (if any)
                            groundSlot.placeItem(draggedItem);  // Place dragged item in ground slot
        
                            // Set the texture of the dragged item sprite using the IconManager
                            groundSlot.itemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
        
                            // Remove the item from the inventory (assuming removeItem is properly defined)
                            inventory->removeItem(draggedItem->id); 
        
                            // Reset the dragging state
                            draggedItemIndex = -1;
                            isDragging = false;
                        } else {
                            Item* tempItem = groundSlot.item;
                            groundSlot.removeItem();
                            groundSlot.placeItem(draggedItem);
                            inventory->removeItem(draggedItem->id);

                            draggedItem = tempItem;
                            draggedItemIndex = tempItem->id;
                            isDragging = true;
                        }
                    } else {
                        if (groundSlot.item) {
                            draggedItem = groundSlot.item;
                            draggedItemIndex = draggedItem->id;
                            isDragging = true;
                            groundSlot.removeItem();
                        }
                    }
                    buttonWasPressed = false;
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
                    clickTimer.getElapsedTime().asSeconds() < clickThreshold) {
                    clickDetected = true;
                }
            }
        }

        if (isDragging && draggedItem) {
            draggedItemSprite.setPosition(mousePos.x - 32, mousePos.y - 32);
            window.draw(draggedItemSprite);
        }

        window.setView(originalView);
    }
};

#endif // INV_H
