#ifndef INV_H
#define INV_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

// Define the Item structure
enum class ItemType {
    Weapon,
    Armor,
    Consumable,
    QuestItem,
    Miscellaneous
};

struct Effect {
    std::string description;
    int value;
};

struct UseLimitations {
    int required_stat;  // Example: strength required for using a sword
    std::string required_stat_name;  // Example: "strength", "dexterity"
};

struct Item {
    std::string name;          // Name of the item
    int id;                    // Unique ID for the item
    int quantity;              // The number of items (useful for stackable items)
    sf::Texture icon;          // Icon representing the item
    ItemType type;             // Type of the item (e.g., consumable, weapon, etc.)
    int value;                 // Value of the item (e.g., sell price)
    int durability;            // Durability of the item (if applicable)
    Effect effect;             // Effect of the item (e.g., healing, damage)
    bool stackable;            // Whether the item is stackable
    UseLimitations use_limitations; // Use limitations (e.g., can be used once, on specific objects, etc.)
    std::string description;   // Description of the item
    int maxStackSize;          // Maximum stack size for stackable items

    // Constructor
    Item(const std::string& name, int id, int quantity, const sf::Texture& icon, ItemType type,
         int value, int durability, const Effect& effect, bool stackable, const UseLimitations& use_limitations,
         const std::string& description, int maxStackSize = 99) // Default max stack size
        : name(name), id(id), quantity(quantity), icon(icon), type(type), value(value),
          durability(durability), effect(effect), stackable(stackable), use_limitations(use_limitations),
          description(description), maxStackSize(maxStackSize) {}
};

class GroundSlot {
public:
    sf::RectangleShape area;  // Area of the ground slot
    sf::Sprite itemSprite;    // Sprite representing the item
    Item* item;              // The item in the ground slot, or nullptr if empty
    const sf::Texture& emptySlotTexture;  // Texture for an empty ground slot (or default state)

    // Constructor initializes the slot and sets the empty texture
    GroundSlot(sf::Vector2f position, const sf::Texture& emptySlotTexture)
        : emptySlotTexture(emptySlotTexture), item(nullptr) {
        area.setSize(sf::Vector2f(64, 64));  // Size of the ground slot (adjustable)
        area.setPosition(position);
        area.setTexture(&emptySlotTexture);  // Set the empty slot texture by default
    }

    // Check if the mouse is over the ground slot
    bool contains(const sf::Vector2f& mousePos) const {
        return area.getGlobalBounds().contains(mousePos);  // Check if mouse is inside the slot
    }

    // Place an item in the ground slot
    void placeItem(Item* newItem) {
        if (!item) {  // Only place an item if the slot is empty
            item = newItem;

            // Set the item sprite texture
            itemSprite.setTexture(newItem->icon);
        
            // Position the item inside the ground slot area, aligning it with the area
            itemSprite.setPosition(area.getPosition());

            // You can choose not to reset the background texture if you want the item to be visible
            area.setTexture(nullptr);  // No background texture when an item is placed
        } else {
            // If the slot is already occupied, you can choose to:
            // 1. Swap the item, or
            // 2. Prevent new items from being placed if the slot is full.

            // Optional: Swap logic to replace the item (this depends on how you want to manage inventory)
            item = newItem;  // Replace the old item with the new one
            itemSprite.setTexture(newItem->icon);  // Update the item sprite with the new item’s icon
            itemSprite.setPosition(area.getPosition());  // Position it correctly
        }
    }

    // Remove the item from the ground slot
    void removeItem() {
        item = nullptr;  // Set item to nullptr (indicating empty slot)
        itemSprite.setTexture(emptySlotTexture);  // Set the slot texture back to the empty slot
        area.setTexture(&emptySlotTexture);  // Make sure the slot visually resets to the empty texture
    }

    // Update the visuals of the ground slot when an item is placed
    void updateVisualsWithItem(Item* newItem) {
        if (item == nullptr) {  // Only update if there isn't already an item
            item = newItem;
            itemSprite.setTexture(newItem->icon);  // Update the sprite texture to the item's icon
            itemSprite.setPosition(area.getPosition());  // Position the item sprite inside the slot
            area.setTexture(nullptr);  // Remove the empty texture (it'll be replaced by the item)
        }
    }

    // Draw the ground slot and the item if there's one inside
    void draw(sf::RenderWindow& window) {
        window.draw(area);  // Draw the area of the ground slot (empty or with texture)
        if (item) {
            window.draw(itemSprite);  // Draw the item sprite if there's an item in the slot
        }
    }

    // Update hover effect when the mouse is over the slot
    void updateHoverEffect(const sf::Vector2f& mousePos) {
        if (contains(mousePos)) {
            area.setFillColor(sf::Color(255, 255, 255, 150));  // Highlight the slot when hovered
        } else {
            area.setFillColor(sf::Color(255, 255, 255, 255));  // Normal state when mouse is not over
        }
    }

    // Display tooltip for the item in the slot
    void showItemTooltip() const {
        if (item) {
            std::cout << "Item: " << item->name << "\nDescription: " << item->description << std::endl;
        }
    }
};


// Define the Inventory class
class Inventory {
public:
    std::vector<Item> items;
    int maxSlots;

    Inventory(int maxSlots) : maxSlots(maxSlots) {}

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

    bool useItem(int itemId) {
        Item* item = getItem(itemId);
        if (item) {
            if (item->type == ItemType::Consumable) {
                item->quantity--;
                if (item->quantity <= 0) {
                    removeItem(itemId);
                }
                return true;
            }
        }
        return false;
    }

    Item* getItem(int itemId) {
        auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
            return item.id == itemId;
        });
        return (it != items.end()) ? &(*it) : nullptr;
    }

    void saveInventory(const std::string& fileName) {
        std::ofstream outFile(fileName);
        for (const auto& item : items) {
            outFile << item.id << "," << item.name << "," << item.quantity << "\n";
        }
    }

    void loadInventory(const std::string& fileName) {
        std::ifstream inFile(fileName);
        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            Item item("Unknown", 0, 0, sf::Texture(), ItemType::Miscellaneous, 0, 0, Effect(), false, UseLimitations(), "");
            std::getline(ss, item.name, ',');
            ss >> item.id;
            ss.ignore(1, ',');
            ss >> item.quantity;
            addItem(item);
        }
    }
};

// Define the InventoryUI class
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

    InventoryUI(Inventory* inventory, const sf::Texture& backgroundTexture, const sf::Texture& emptySlotTexture, const sf::Vector2f& position, int rows, int cols, const sf::Texture& groundSlotTexture)
        : inventory(inventory), emptySlotTexture(emptySlotTexture) {

        // Increase the size of the inventory background (making it larger)
        background.setTexture(backgroundTexture);
        background.setPosition(position);
        background.setScale(2.f, 2.f);  // Scale the background to make it twice as large

        float slotSize = 100.f;  // Size of inventory slots
        float padding = 10.f;
        float startY = position.y + background.getGlobalBounds().height / 2 - (rows * (slotSize + padding)) / 2;

        // Add item slots to the left side of the inventory (same as before)
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(position.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                itemSlots.push_back(slot);
            }
        }

        // Add ground slots to the right side of the inventory (new positioning)
        float groundSlotX = position.x + background.getGlobalBounds().width - 160.f; // Make ground slots appear on the right side
        float groundSlotY = position.y + background.getGlobalBounds().height / 2 - 200.f; // Vertically center the ground slots

        for (int i = 0; i < 5; ++i) {
            groundSlots.emplace_back(sf::Vector2f(groundSlotX, groundSlotY + (i * 70)), groundSlotTexture);
        }
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;
    
        // Adjust the view for the inventory UI
        sf::View originalView = window.getView();
        sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        window.setView(newView);

        sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        window.draw(background); // Draw the background of the inventory

        // Draw the item slots
        for (size_t i = 0; i < itemSlots.size(); ++i) {
            sf::Sprite slotSprite;
        
            // If we are dragging an item, don't draw the item in the original slot
            if (i < inventory->items.size() && i != draggedItemIndex) {
                slotSprite.setTexture(inventory->items[i].icon);
            } else {
                // Empty slot texture
                slotSprite.setTexture(emptySlotTexture);
            }

            slotSprite.setPosition(itemSlots[i].getPosition());
            window.draw(slotSprite);
        }

        // Draw the ground slots on the right
        for (auto& groundSlot : groundSlots) {
            groundSlot.draw(window);
        }

        // Draw the item being dragged, if any
        if (isDragging) {
            draggedItemSprite.setPosition(sf::Mouse::getPosition(window).x - 32, sf::Mouse::getPosition(window).y - 32);
            window.draw(draggedItemSprite);
        }

        window.setView(originalView);  // Restore the original view
    }

    void startDragging(int index) {
        if (index >= 0 && index < inventory->items.size()) {
            draggedItemIndex = index;
            isDragging = true;
    
            // Set the dragged item sprite's texture
            draggedItemSprite.setTexture(inventory->items[index].icon);
    
            // "Remove" the item from the slot visually
            itemSlots[index].setTexture(&emptySlotTexture);  // Replace with the empty slot texture
            itemSlots[index].setFillColor(sf::Color(255, 255, 255, 0));  // Make the slot fully transparent

            // Update the item slot sprite to ensure it's cleared visually
            itemSlots[index].setOutlineColor(sf::Color::Transparent);  // Optionally remove the outline
        }
    }

    void stopDragging(const sf::Vector2f& mousePos) {
        if (draggedItemIndex == -1) return;  // No item is being dragged

        bool itemPlaced = false;

        // Handle item drop to ground slot
        for (auto& groundSlot : groundSlots) {
            if (groundSlot.contains(mousePos)) {
                // Place the item in the ground slot
                if (groundSlot.item == nullptr) {
                    groundSlot.placeItem(&inventory->items[draggedItemIndex]);
                    inventory->removeItem(inventory->items[draggedItemIndex].id);
                    itemPlaced = true;
                    break;
                }
            }
        }

        // Handle item drop back into inventory slot
        if (!itemPlaced) {
            for (size_t i = 0; i < itemSlots.size(); ++i) {
                if (itemSlots[i].getGlobalBounds().contains(mousePos)) {
                    // If item is dropped in the same slot, just restore the icon
                    if (i == draggedItemIndex) {
                        itemPlaced = true;
                        break;
                    }

                    // Step 1: Get the item being dragged
                    Item* draggedItem = &inventory->items[draggedItemIndex];

                    // Step 2: If the target slot is empty, place the item there
                    if (i >= inventory->items.size()) {
                        // Slot is empty, place the dragged item in it
                        inventory->items.push_back(*draggedItem);  // Add to inventory
                        inventory->removeItem(draggedItem->id);  // Remove from original position
                    } else {
                        // Slot is already occupied, swap the items
                        std::swap(inventory->items[draggedItemIndex], inventory->items[i]);
                    }

                    // Step 3: Update the original slot UI (make it empty visually)
                    itemSlots[draggedItemIndex].setTexture(&emptySlotTexture);
                    itemSlots[draggedItemIndex].setFillColor(sf::Color(0, 0, 0, 0));  // Empty slot visually

                    // Step 4: Update the new slot UI with the item’s icon
                    itemSlots[i].setTexture(&inventory->items[i].icon);
                    itemSlots[i].setFillColor(sf::Color(255, 255, 255, 255));  // Restore color

                    itemPlaced = true;
                    break;
                }
            }
        }

        // If not placed anywhere, restore it to the original slot
        if (!itemPlaced) {
            // Restore the original item icon in the original slot
            itemSlots[draggedItemIndex].setTexture(&inventory->items[draggedItemIndex].icon);
            itemSlots[draggedItemIndex].setFillColor(sf::Color(255, 255, 255, 255));  // Restore color
        }

        // Reset dragging state
        isDragging = false;
        draggedItemIndex = -1;
    }

};

#endif // INV_H