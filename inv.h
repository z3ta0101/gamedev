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

    // Draw ground slot and the item if present
    void draw(sf::RenderWindow& window) {
        window.draw(area);  // Draw the ground slot background
        if (item) {         // Draw the item sprite if it exists
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
};

// The UI for displaying and interacting with the inventory
class InventoryUI {
public:
    sf::Sprite background;
    std::vector<sf::RectangleShape> itemSlots;
    std::vector<GroundSlot> groundSlots;
    Inventory* inventory;
    sf::Texture emptySlotTexture;
    int draggedItemIndex = -1;
    bool isDragging = false;
    sf::Sprite draggedItemSprite;
    bool isVisible = false;
    sf::Font font;
    sf::Text tooltipText;

    InventoryUI(Inventory* inventory, const sf::Texture& backgroundTexture, const sf::Texture& emptySlotTexture, const sf::Vector2f& position, int rows, int cols, const sf::Texture& groundSlotTexture)
        : inventory(inventory), emptySlotTexture(emptySlotTexture) {

        background.setTexture(backgroundTexture);
        background.setPosition(position);
        background.setScale(2.f, 2.f);

        font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf");
        tooltipText.setFont(font);
        tooltipText.setCharacterSize(14);
        tooltipText.setFillColor(sf::Color::White);

        // Set up item slots
        float slotSize = 75.f;  // Inventory slot size
        float padding = 10.f;
        float startY = position.y + background.getGlobalBounds().height / 2 - (rows * (slotSize + padding)) / 2;

        // Create inventory item slots
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(position.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                itemSlots.push_back(slot);
            }
        }

        // Set up ground slots (use slotSize to make them the same size as inventory slots)
        float groundSlotX = position.x + background.getGlobalBounds().width - 160.f;  // Adjust X position
        float groundSlotY = position.y + background.getGlobalBounds().height / 2 - 200.f;  // Adjust Y position

        // Create ground slots with same size as inventory slots
        for (int i = 0; i < 5; ++i) {
            groundSlots.emplace_back(sf::Vector2f(groundSlotX, groundSlotY + (i * (slotSize + padding))), groundSlotTexture, slotSize);
        }
    }


    void startDragging(int index) {
        if (index >= 0 && index < inventory->items.size()) {
            draggedItemIndex = index;
            isDragging = true;

            // Get the texture using the IconManager based on the iconPath stored in the item
            const std::string& iconPath = inventory->items[index].iconPath;
            draggedItemSprite.setTexture(IconManager::getIcon(iconPath));  // Load texture dynamically

        }
    }

    void stopDragging(const sf::Vector2f& mousePos) {
        if (draggedItemIndex == -1) return;

        bool itemPlaced = false;

        // Try placing on a ground slot
        for (auto& groundSlot : groundSlots) {
            if (groundSlot.contains(mousePos)) {
                if (groundSlot.item == nullptr) {
                    groundSlot.placeItem(&inventory->items[draggedItemIndex]);
                    inventory->removeItem(inventory->items[draggedItemIndex].id);
                    itemPlaced = true;
                    break;
                }
            }
        }

        // Try placing in inventory slots
        if (!itemPlaced) {
            for (size_t i = 0; i < itemSlots.size(); ++i) {
                if (itemSlots[i].getGlobalBounds().contains(mousePos)) {
                    Item* draggedItem = &inventory->items[draggedItemIndex];
                    if (i >= inventory->items.size()) {
                        inventory->items.push_back(*draggedItem);
                        inventory->removeItem(draggedItem->id);
                    } else {
                        std::swap(inventory->items[draggedItemIndex], inventory->items[i]);
                    }
                    itemPlaced = true;
                    break;
                }
            }
        }

        

        isDragging = false;
        draggedItemIndex = -1;
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        sf::View originalView = window.getView();
        sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        window.setView(newView);

        sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        window.draw(background);

        // Draw Item Slots and Tooltips
        for (size_t i = 0; i < itemSlots.size(); ++i) {
            sf::Sprite itemSprite;
            if (i < inventory->items.size()) {
                // Use IconManager to get the texture for the item icon
                itemSprite.setTexture(IconManager::getIcon(inventory->items[i].iconPath));
                itemSprite.setPosition(itemSlots[i].getPosition());

                if (itemSprite.getGlobalBounds().contains(mousePos)) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        startDragging(i);
                    }

                    // Display Tooltip
                    tooltipText.setString(inventory->items[i].description);
                    tooltipText.setPosition(mousePos);
                    window.draw(tooltipText);
                }

                window.draw(itemSprite);
            } else {
                sf::Sprite slotSprite;
                slotSprite.setTexture(emptySlotTexture);
                slotSprite.setPosition(itemSlots[i].getPosition());
                window.draw(slotSprite);
            }
        }

        // Draw ground slots
        for (auto& groundSlot : groundSlots) {
            groundSlot.draw(window);
        }

        // Draw Dragging Item
        if (isDragging) {
            draggedItemSprite.setPosition(sf::Mouse::getPosition(window).x - 32, sf::Mouse::getPosition(window).y - 32);
            window.draw(draggedItemSprite);
        }

        window.setView(originalView);
    }
};

#endif // INV_H
