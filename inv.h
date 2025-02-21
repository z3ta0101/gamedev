#ifndef INV_H
#define INV_H

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
                defaultTexture.create(32, 32); // Fallback blank texture
            }
        }

        if (textures.find(iconPath) == textures.end()) {
            sf::Texture texture;
            if (texture.loadFromFile(iconPath)) {
                textures[iconPath] = texture;
            } else {
                std::cerr << "Error loading texture: " << iconPath << std::endl;
                textures[iconPath] = defaultTexture; // Cache default on failure
            }
        }
        return textures[iconPath];
    }

private:
    static std::unordered_map<std::string, sf::Texture> textures;
};

std::unordered_map<std::string, sf::Texture> IconManager::textures;

class GroundSlot {
public:
    sf::RectangleShape area;
    sf::Sprite itemSprite;
    Item* item;
    const sf::Texture& groundSlotTexture;

    GroundSlot(sf::Vector2f position, const sf::Texture& groundSlotTexture, float slotSize)
        : groundSlotTexture(groundSlotTexture), item(nullptr) {
        area.setSize(sf::Vector2f(slotSize, slotSize));
        area.setPosition(position);
        area.setTexture(&groundSlotTexture);
        area.setOutlineThickness(1.f);
        area.setOutlineColor(sf::Color::Transparent);
    }

    bool contains(const sf::Vector2f& mousePos) const {
        return area.getGlobalBounds().contains(mousePos);
    }

    void placeItem(Item* newItem) {
        if (newItem) {
            item = newItem;
            itemSprite.setTexture(IconManager::getIcon(newItem->iconPath));
            centerSprite();
        }
    }

    void removeItem() {
        item = nullptr;
        area.setTexture(&groundSlotTexture);
    }

    Item* getItem() const { return item; }

    void draw(sf::RenderWindow& window) {
        window.draw(area);
        if (item) {
            window.draw(itemSprite);
        }
    }

    void resetToOriginalState() {
        area.setTexture(&groundSlotTexture);
    }

    void setHighlight(bool highlight) {
        area.setOutlineColor(highlight ? sf::Color::Yellow : sf::Color::Transparent);
    }

private:
    void centerSprite() {
        sf::FloatRect spriteBounds = itemSprite.getGlobalBounds();
        itemSprite.setPosition(
            area.getPosition().x + (area.getSize().x - spriteBounds.width) / 2,
            area.getPosition().y + (area.getSize().y - spriteBounds.height) / 2
        );
    }
};

class EquipmentSlot {
public:
    sf::RectangleShape area;
    Item* item;
    ItemType allowedType;

    EquipmentSlot(sf::Vector2f position, ItemType type, const sf::Texture& texture)
        : allowedType(type), item(nullptr) {
        area.setSize(sf::Vector2f(75.f, 75.f));
        area.setPosition(position);
        area.setTexture(&texture);
        area.setOutlineThickness(1.f);
        area.setOutlineColor(sf::Color::Transparent);
    }

    bool canEquip(const Item& item) const { return item.type == allowedType; }

    void equipItem(Item* newItem) {
        if (newItem && canEquip(*newItem)) item = newItem;
    }

    void unequipItem() { item = nullptr; }

    void draw(sf::RenderWindow& window) {
        window.draw(area);
        if (item) {
            sf::Sprite itemSprite(IconManager::getIcon(item->iconPath));
            itemSprite.setPosition(area.getPosition());
            window.draw(itemSprite);
        }
    }

    void setHighlight(bool highlight) {
        area.setOutlineColor(highlight ? sf::Color::Yellow : sf::Color::Transparent);
    }
};

class Inventory {
public:
    std::vector<Item> items;
    int maxSlots;

    Inventory(int maxSlots) : maxSlots(maxSlots) {
        items.resize(maxSlots, Item());
    }

    bool addItem(const Item& item) {
        for (auto& existingItem : items) {
            if (existingItem.id == -1) {
                existingItem = item;
                return true;
            }
            if (existingItem.id == item.id && existingItem.stackable) {
                int space = existingItem.maxStackSize - existingItem.quantity;
                if (space > 0) {
                    existingItem.quantity += std::min(space, item.quantity);
                    return true;
                }
            }
        }
        return false;
    }

    bool removeItem(int itemId) {
        for (auto& item : items) {
            if (item.id == itemId) {
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
            return true;
        } else if (items[targetSlotIndex].id == item.id && items[targetSlotIndex].stackable) {
            int space = items[targetSlotIndex].maxStackSize - items[targetSlotIndex].quantity;
            if (space >= item.quantity) {
                items[targetSlotIndex].quantity += item.quantity;
                return true;
            }
        }
        Item temp = items[targetSlotIndex];
        items[targetSlotIndex] = item;
        return addItem(temp);
    }

    void saveInventory(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& item : items) {
            if (item.id != -1) file << item.id << " " << item.quantity << "\n";
        }
    }

    void loadInventory(const std::string& filename) {
        std::ifstream file(filename);
        items.clear();
        items.resize(maxSlots, Item());
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

class InventoryUI {
public:
    sf::Sprite background;
    std::vector<sf::RectangleShape> itemSlots;
    std::vector<GroundSlot> groundSlots;
    sf::Texture groundSlotTexture;
    std::vector<EquipmentSlot> equipmentSlots;
    Inventory* inventory;
    sf::Texture emptySlotTexture;
    int draggedItemIndex = -1;
    Item draggedItemStorage; // Pre-allocated storage for dragged item
    Item* draggedItem = nullptr;
    bool isDragging = false;
    sf::Sprite draggedItemSprite;
    bool isVisible = false;
    sf::Font font;
    sf::Text tooltipText;
    sf::Text quantityText;
    bool buttonWasPressed = false;
    sf::Clock clickTimer;
    float clickThreshold = 0.2f;

    InventoryUI(Inventory* inventory, const sf::Texture& backgroundTexture, 
                const sf::Texture& emptySlotTexture, const sf::Vector2f& position, 
                int rows, int cols, const sf::Texture& groundSlotTexture)
        : inventory(inventory), emptySlotTexture(emptySlotTexture), groundSlotTexture(groundSlotTexture),
          draggedItemStorage() { // Initialize with default empty item
        draggedItem = nullptr; // Ensure pointer starts null
        background.setTexture(backgroundTexture);
        background.setPosition(position);
        background.setScale(2.f, 2.f);

        if (!font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
            std::cerr << "Failed to load font\n";
        }
        tooltipText.setFont(font);
        tooltipText.setCharacterSize(14);
        tooltipText.setFillColor(sf::Color::White);

        quantityText.setFont(font);
        quantityText.setCharacterSize(12);
        quantityText.setFillColor(sf::Color::White);

        float slotSize = 75.f;
        float padding = 10.f;
        float startY = position.y + background.getGlobalBounds().height / 2 - (rows * (slotSize + padding)) / 2;

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                sf::RectangleShape slot;
                slot.setSize(sf::Vector2f(slotSize, slotSize));
                slot.setPosition(position.x + col * (slotSize + padding), startY + row * (slotSize + padding));
                slot.setTexture(&emptySlotTexture);
                slot.setOutlineThickness(1.f);
                slot.setOutlineColor(sf::Color::Transparent);
                itemSlots.push_back(slot);
            }
        }

        float groundSlotX = position.x + background.getGlobalBounds().width - 160.f;
        float groundSlotY = position.y + background.getGlobalBounds().height / 2 - 200.f;
        for (int i = 0; i < 5; ++i) {
            groundSlots.emplace_back(sf::Vector2f(groundSlotX, groundSlotY + (i * (slotSize + padding))), groundSlotTexture, slotSize);
        }

        equipmentSlots.emplace_back(sf::Vector2f(groundSlotX - 100.f, groundSlotY), ItemType::Weapon, emptySlotTexture);
        equipmentSlots.emplace_back(sf::Vector2f(groundSlotX - 100.f, groundSlotY + slotSize + padding), ItemType::Armor, emptySlotTexture);
    }

    void startDragging(int index = -1, GroundSlot* groundSlot = nullptr) {
        if (isDragging) return; // Prevent multiple drags

        if (index >= 0 && index < inventory->items.size() && inventory->items[index].id != -1) {
            draggedItemIndex = index;
            draggedItemStorage = inventory->items[index]; // Copy to storage
            draggedItem = &draggedItemStorage;
            isDragging = true;
            draggedItemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
            inventory->removeItem(draggedItem->id);
            std::cout << "Dragging from inventory: " << draggedItem->iconPath << "\n";
        } else if (groundSlot && groundSlot->getItem()) {
            draggedItemIndex = -1;
            draggedItemStorage = *groundSlot->getItem(); // Copy to storage
            draggedItem = &draggedItemStorage;
            isDragging = true;
            draggedItemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
            groundSlot->removeItem();
            std::cout << "Dragging from ground: " << draggedItem->iconPath << "\n";
        }
    }

    void stopDragging(const sf::Vector2f& mousePos) {
        if (!isDragging || !draggedItem) return;

        bool itemPlaced = false;

        for (auto& groundSlot : groundSlots) {
            if (groundSlot.contains(mousePos)) {
                if (!groundSlot.item) {
                    groundSlot.placeItem(new Item(*draggedItem)); // Create new item for placement
                    itemPlaced = true;
                } else {
                    Item* temp = groundSlot.item;
                    groundSlot.placeItem(new Item(*draggedItem));
                    draggedItemStorage = *temp; // Update storage with swapped item
                    delete temp; // Clean up old item
                    draggedItem = &draggedItemStorage;
                    draggedItemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
                    itemPlaced = true;
                }
                break;
            }
        }

        if (!itemPlaced) {
            for (auto& equipmentSlot : equipmentSlots) {
                if (equipmentSlot.area.getGlobalBounds().contains(mousePos)) {
                    if (equipmentSlot.canEquip(*draggedItem)) {
                        if (!equipmentSlot.item) {
                            equipmentSlot.equipItem(new Item(*draggedItem));
                            itemPlaced = true;
                        } else {
                            Item* temp = equipmentSlot.item;
                            equipmentSlot.equipItem(new Item(*draggedItem));
                            draggedItemStorage = *temp;
                            delete temp;
                            draggedItem = &draggedItemStorage;
                            draggedItemSprite.setTexture(IconManager::getIcon(draggedItem->iconPath));
                            itemPlaced = true;
                        }
                    }
                    break;
                }
            }
        }

        if (!itemPlaced) {
            for (size_t i = 0; i < itemSlots.size(); ++i) {
                if (itemSlots[i].getGlobalBounds().contains(mousePos)) {
                    if (inventory->placeItem(*draggedItem, i)) {
                        itemPlaced = true;
                    }
                    break;
                }
            }
        }

        if (!itemPlaced && draggedItemIndex != -1) {
            inventory->placeItem(*draggedItem, draggedItemIndex);
        }

        draggedItemIndex = -1;
        draggedItem = nullptr;
        isDragging = false;
    }

    void draw(sf::RenderWindow& window) {
        if (!isVisible) return;

        sf::View originalView = window.getView();
        sf::View newView(sf::FloatRect(0.f, 0.f, float(window.getSize().x), float(window.getSize().y)));
        window.setView(newView);

        sf::Vector2i windowMousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePos = window.mapPixelToCoords(windowMousePos);

        window.draw(background);

        bool mousePressedThisFrame = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        bool rightClicked = sf::Mouse::isButtonPressed(sf::Mouse::Right) && !buttonWasPressed;

        // Handle drag initiation before rendering
        if (mousePressedThisFrame && !buttonWasPressed) {
            buttonWasPressed = true;
            clickTimer.restart();
        } else if (!mousePressedThisFrame && buttonWasPressed) {
            buttonWasPressed = false;
            if (clickTimer.getElapsedTime().asSeconds() < clickThreshold) {
                if (isDragging) {
                    stopDragging(mousePos);
                } else {
                    for (size_t i = 0; i < itemSlots.size(); ++i) {
                        if (itemSlots[i].getGlobalBounds().contains(mousePos) && inventory->items[i].id != -1) {
                            startDragging(i);
                            break;
                        }
                    }
                    for (auto& groundSlot : groundSlots) {
                        if (groundSlot.contains(mousePos) && groundSlot.item) {
                            startDragging(-1, &groundSlot);
                            break;
                        }
                    }
                    for (auto& equipmentSlot : equipmentSlots) {
                        if (equipmentSlot.area.getGlobalBounds().contains(mousePos) && equipmentSlot.item) {
                            startDragging(-1, nullptr);
                            break;
                        }
                    }
                }
            }
        }

        if (rightClicked) {
            for (size_t i = 0; i < itemSlots.size(); ++i) {
                if (itemSlots[i].getGlobalBounds().contains(mousePos) && inventory->items[i].id != -1) {
                    std::cout << "Right-clicked item: " << inventory->items[i].name << "\n";
                    break;
                }
            }
        }

        // Render inventory slots
        for (size_t i = 0; i < itemSlots.size(); ++i) {
            itemSlots[i].setOutlineColor(itemSlots[i].getGlobalBounds().contains(mousePos) ? sf::Color::Yellow : sf::Color::Transparent);
            window.draw(itemSlots[i]);

            if (i < inventory->items.size() && inventory->items[i].id != -1 && (!isDragging || i != draggedItemIndex)) {
                sf::Sprite slotSprite(IconManager::getIcon(inventory->items[i].iconPath));
                slotSprite.setPosition(itemSlots[i].getPosition());
                window.draw(slotSprite);

                if (inventory->items[i].stackable && inventory->items[i].quantity > 1) {
                    quantityText.setString(std::to_string(inventory->items[i].quantity));
                    quantityText.setPosition(itemSlots[i].getPosition().x + 50, itemSlots[i].getPosition().y + 50);
                    window.draw(quantityText);
                }

                if (itemSlots[i].getGlobalBounds().contains(mousePos)) {
                    std::string tooltip = inventory->items[i].name + "\n" +
                                         inventory->items[i].description + "\n" +
                                         "Value: " + std::to_string(inventory->items[i].value) + "\n" +
                                         "Durability: " + std::to_string(inventory->items[i].durability);
                    tooltipText.setString(tooltip);
                    tooltipText.setPosition(mousePos);
                    window.draw(tooltipText);
                }
            }
        }

        // Render ground slots
        for (auto& groundSlot : groundSlots) {
            groundSlot.setHighlight(groundSlot.contains(mousePos));
            if (!isDragging || (draggedItem && groundSlot.item != draggedItem)) {
                groundSlot.draw(window);
            }
        }

        // Render equipment slots
        for (auto& equipmentSlot : equipmentSlots) {
            equipmentSlot.setHighlight(equipmentSlot.area.getGlobalBounds().contains(mousePos));
            if (!isDragging || (draggedItem && equipmentSlot.item != draggedItem)) {
                equipmentSlot.draw(window);
            }
        }

        // Render dragged item
        if (isDragging && draggedItem) {
            std::cout << "Drawing dragged item, texture size: " 
                      << draggedItemSprite.getTexture()->getSize().x << "x" 
                      << draggedItemSprite.getTexture()->getSize().y << "\n";
            draggedItemSprite.setPosition(mousePos.x - 32, mousePos.y - 32);
            window.draw(draggedItemSprite);
        }

        window.setView(originalView);
    }
};

#endif // INV_H
