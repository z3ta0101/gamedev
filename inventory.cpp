#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

// Define Item and related structures
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
    std::string name;
    int id;
    int quantity;
    sf::Texture icon;
    ItemType type;
    int value;
    int durability;
    Effect effect;
    bool stackable;
    UseLimitations use_limitations;
    std::string description;
    int maxStackSize;

    Item(const std::string& name, int id, int quantity, const sf::Texture& icon, ItemType type,
         int value, int durability, const Effect& effect, bool stackable, const UseLimitations& use_limitations,
         const std::string& description, int maxStackSize = 99)
        : name(name), id(id), quantity(quantity), icon(icon), type(type), value(value),
          durability(durability), effect(effect), stackable(stackable), use_limitations(use_limitations),
          description(description), maxStackSize(maxStackSize) {}
};

class GroundSlot {
public:
    sf::RectangleShape area;
    sf::Sprite itemSprite;
    Item* item;              
    const sf::Texture& emptySlotTexture;

    GroundSlot(sf::Vector2f position, const sf::Texture& emptySlotTexture)
        : emptySlotTexture(emptySlotTexture), item(nullptr) {
        area.setSize(sf::Vector2f(64, 64));
        area.setPosition(position);
        area.setTexture(&emptySlotTexture);
    }

    bool contains(const sf::Vector2f& mousePos) const {
        return area.getGlobalBounds().contains(mousePos);
    }

    void placeItem(Item* newItem) {
        if (newItem && !item) {
            item = newItem;
            itemSprite.setTexture(item->icon);
            itemSprite.setPosition(area.getPosition().x + (area.getSize().x - itemSprite.getGlobalBounds().width) / 2,
                                   area.getPosition().y + (area.getSize().y - itemSprite.getGlobalBounds().height) / 2);
            area.setTexture(&emptySlotTexture);
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(area);
        if (item) {
            window.draw(itemSprite);
        }
    }
};

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

    Item* getItem(int itemId) {
        auto it = std::find_if(items.begin(), items.end(), [itemId](const Item& item) {
            return item.id == itemId;
        });
        return (it != items.end()) ? &(*it) : nullptr;
    }
};

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

        background.setTexture(backgroundTexture);
        background.setPosition(position);
        background.setScale(2.f, 2.f);

        float slotSize = 100.f;
        float padding = 10.f;
        float startY = position.y + background.getGlobalBounds().height / 2 - (rows * (slotSize + padding)) / 2;

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(position.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                itemSlots.push_back(slot);
            }
        }

        float groundSlotX = position.x + background.getGlobalBounds().width - 160.f;
        float groundSlotY = position.y + background.getGlobalBounds().height / 2 - 200.f;

        for (int i = 0; i < 5; ++i) {
            groundSlots.emplace_back(sf::Vector2f(groundSlotX, groundSlotY + (i * 70)), groundSlotTexture);
        }
    }

    void startDragging(int index) {
        if (index >= 0 && index < inventory->items.size()) {
            draggedItemIndex = index;
            isDragging = true;
            draggedItemSprite.setTexture(inventory->items[index].icon);
            itemSlots[index].setFillColor(sf::Color(255, 255, 255, 0));
        }
    }

    void stopDragging(const sf::Vector2f& mousePos) {
        if (draggedItemIndex == -1) return;

        bool itemPlaced = false;
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

        if (!itemPlaced) {
            itemSlots[draggedItemIndex].setFillColor(sf::Color(255, 255, 255, 255));
        }

        isDragging = false;
        draggedItemIndex = -1;
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        // Adjust the view for the inventory UI
        sf::View originalView = window.getView();
        sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        window.setView(newView);

        sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        window.draw(background);

        for (size_t i = 0; i < itemSlots.size(); ++i) {
            sf::Sprite itemSprite;
            if (i < inventory->items.size()) {
                itemSprite.setTexture(inventory->items[i].icon);
                itemSprite.setPosition(itemSlots[i].getPosition());

                if (itemSprite.getGlobalBounds().contains(mousePos)) {
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                        startDragging(i);
                    }
                }

                window.draw(itemSprite);
            } else {
                sf::Sprite slotSprite;
                slotSprite.setTexture(emptySlotTexture);
                slotSprite.setPosition(itemSlots[i].getPosition());
                window.draw(slotSprite);
            }
        }

        for (auto& groundSlot : groundSlots) {
            groundSlot.draw(window);
        }

        if (isDragging) {
            draggedItemSprite.setPosition(sf::Mouse::getPosition(window).x - 32, sf::Mouse::getPosition(window).y - 32);
            window.draw(draggedItemSprite);
        }
        window.setView(originalView);  // Restore the original view
    }
};

int main() {
    // Create SFML window and load textures
    sf::RenderWindow window(sf::VideoMode(800, 600), "Inventory System");
    sf::Texture emptySlotTexture, backgroundTexture, itemTexture, groundSlotTexture;
    emptySlotTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/invslot.png");
    backgroundTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/invpanel.png");
    itemTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/healthpotion.png");
    groundSlotTexture.loadFromFile("/home/z3ta/c++/SoV/images/ui/invslot.png");

    // Create an inventory and populate it
    Inventory inventory(10);
    Item exampleItem("Potion of Healing", 1, 5, itemTexture, ItemType::Consumable, 10, 100, Effect(), true, UseLimitations(), "Heals a small amount of HP");
    inventory.addItem(exampleItem);

    // Create InventoryUI
    InventoryUI inventoryUI(&inventory, backgroundTexture, emptySlotTexture, sf::Vector2f(50, 50), 4, 4, groundSlotTexture);
    inventoryUI.isVisible = true;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        inventoryUI.draw(window);
        window.display();
    }

    return 0;
}