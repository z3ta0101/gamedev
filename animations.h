#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

// Static variables for attack animation state
static bool isAttacking = false;
static int attackFrame = 0;

// Directional rotation
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerHealth > 0)
{
    direction = 5;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerHealth > 0)
{
    direction = 7;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerHealth > 0)
{
    direction = 1;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerHealth > 0)
{
    direction = 3;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerHealth > 0)
{
    direction = 4;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerHealth > 0)
{
    direction = 6;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerHealth > 0)
{
    direction = 8;
    idle = 0;
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerHealth > 0)
{
    direction = 2;
    idle = 0;
}

// Death animations
if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 1) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3048;
    clothesSourceSprite.top = 3048;
    armorSourceSprite.top = isPlatemailEquipped ? 6792 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 2) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3175;
    clothesSourceSprite.top = 3175;
    armorSourceSprite.top = isPlatemailEquipped ? 6919 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 3) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3302;
    clothesSourceSprite.top = 3302;
    armorSourceSprite.top = isPlatemailEquipped ? 7046 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 4) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3429;
    clothesSourceSprite.top = 3429;
    armorSourceSprite.top = isPlatemailEquipped ? 7173 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 5) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3556;
    clothesSourceSprite.top = 3556;
    armorSourceSprite.top = isPlatemailEquipped ? 7300 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 6) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3683;
    clothesSourceSprite.top = 3683;
    armorSourceSprite.top = isPlatemailEquipped ? 7427 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 7) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3810;
    clothesSourceSprite.top = 3810;
    armorSourceSprite.top = isPlatemailEquipped ? 7554 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 8) {
    idle = 1;
    playerSpeed = 0;
    rectSourceSprite.height = 127;
    rectSourceSprite.width = 127;
    clothesSourceSprite.height = 127;
    clothesSourceSprite.width = 127;
    armorSourceSprite.height = 127;
    armorSourceSprite.width = 127;
    rectSourceSprite.top = 3937;
    clothesSourceSprite.top = 3937;
    armorSourceSprite.top = isPlatemailEquipped ? 7681 : 0;
    if (rectSourceSprite.left >= 2413) {
        rectSourceSprite.left = 2413;
        clothesSourceSprite.left = 2413;
        armorSourceSprite.left = isPlatemailEquipped ? 2413 : 0;
    } else {
        rectSourceSprite.left += 127;
        clothesSourceSprite.left += 127;
        armorSourceSprite.left += isPlatemailEquipped ? 127 : 0;
    }
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

// Walking animations
if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 1 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 3744;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 3744;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 3744;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 0;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 0;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 0;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 2 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 3900;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 3900;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 3900;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 156;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 156;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 156;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 3 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 4056;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 4056;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 4056;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 312;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 312;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 312;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 4 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 4212;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 4212;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 4212;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 468;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 468;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 468;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 5 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 4368;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 4368;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 4368;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 624;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 624;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 624;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 6 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 4524;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 4524;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 4524;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 780;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 780;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 780;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 7 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 4680;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 4680;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 4680;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 936;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 936;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 936;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 8 && idle == 0)
{
    if (isPlatemailEquipped)
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 4836;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 4836;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 4836;
    }
    else
    {
        armorSourceSprite.height = 156;
        armorSourceSprite.width = 156;
        armorSourceSprite.top = 1092;
        clothesSourceSprite.height = 156;
        clothesSourceSprite.width = 156;
        clothesSourceSprite.top = 1092;
        rectSourceSprite.height = 156;
        rectSourceSprite.width = 156;
        rectSourceSprite.top = 1092;
    }
    isMoving = true;
    if (armorSourceSprite.left >= 1404)
        armorSourceSprite.left = 0;
    else
        armorSourceSprite.left += 156;

    if (rectSourceSprite.left >= 1404)
        rectSourceSprite.left = 0;
    else
        rectSourceSprite.left += 156;

    if (clothesSourceSprite.left >= 1404)
        clothesSourceSprite.left = 0;
    else
        clothesSourceSprite.left += 156;

    player.setTextureRect(rectSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    clock.restart();
}

// Idle animations and input handling
if (event.type == sf::Event::KeyReleased)
{
    if (event.key.code == sf::Keyboard::W ||
        event.key.code == sf::Keyboard::A ||
        event.key.code == sf::Keyboard::S ||
        event.key.code == sf::Keyboard::D)
    {
        isMoving = false;
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            idle = 1;
        }
    }
}

if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && !inventoryUI.isVisible && playerHealth > 0)
{
    isAttacking = true;
    attackFrame = 0;
    attacking = true;
}

if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
{
    isAttacking = false;
    attackFrame = 0;
    attacking = false;
    round = false;
    // Transition to idle
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    rectSourceSprite.top = 2496 + (direction - 1) * 156;
    clothesSourceSprite.top = isPlatemailEquipped ? 3744 : rectSourceSprite.top;
    armorSourceSprite.top = isPlatemailEquipped ? rectSourceSprite.top + 3744 : 0;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    idle = 1;
}

if (idle == 1 && direction == 1 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 2496;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 2496;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 2 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 2652;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 2652;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 3 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 2808;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 2808;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 4 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 2964;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 2964;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 5 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 3120;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3120;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 176;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 6 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 3276;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3276;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 7 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 3432;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3432;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 8 && playerHealth > 0 && !isPlatemailEquipped && !isAttacking) {
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 3588;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3588;
    clothesSourceSprite.left = 0;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 0;
    rectSourceSprite.left = 0;
    isMoving = false;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

// Armor idle
if (idle == 1 && direction == 1 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 6240;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 6240;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 2 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 6396;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 6396;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 3 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 6552;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 6552;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 4 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 6708;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 6708;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 5 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 6864;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 6864;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 6 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 7020;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 7020;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 7 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 7176;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 7176;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

if (idle == 1 && direction == 8 && playerHealth > 0 && isPlatemailEquipped && !isAttacking) {
    isMoving = false;
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    rectSourceSprite.top = 7332;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    clothesSourceSprite.top = 3744;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    armorSourceSprite.top = 7332;
    rectSourceSprite.left = 0;
    clothesSourceSprite.left = 0;
    armorSourceSprite.left = 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    clock.restart();
}

// Attack animations
if (isAttacking && clock.getElapsedTime().asSeconds() > 0.06 && !inventoryUI.isVisible && playerHealth > 0)
{
    rectSourceSprite.height = 156;
    rectSourceSprite.width = 156;
    clothesSourceSprite.height = 156;
    clothesSourceSprite.width = 156;
    armorSourceSprite.height = 156;
    armorSourceSprite.width = 156;
    int topBase = isPlatemailEquipped ? 4992 + (direction - 1) * 156 : 1248 + (direction - 1) * 156;
    int topArmor = isPlatemailEquipped ? topBase : 0;
    rectSourceSprite.top = topBase;
    clothesSourceSprite.top = topBase;
    armorSourceSprite.top = topArmor;
    rectSourceSprite.left = attackFrame * 156;
    clothesSourceSprite.left = attackFrame * 156;
    armorSourceSprite.left = isPlatemailEquipped ? attackFrame * 156 : 0;
    player.setTextureRect(rectSourceSprite);
    playerclothes.setTextureRect(clothesSourceSprite);
    playerarmor.setTextureRect(armorSourceSprite);
    attackFrame++;
    if (attackFrame >= 14) { // 14 frames (0 to 2184)
        attackFrame = 0;
        round = false;
        playerAttackRoll = generateRandomNumber();
        
        std::cout << "Player Attack Roll: " << playerAttackRoll << "\n";
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        std::cout << "Mouse World Position: (" << worldPos.x << ", " << worldPos.y << ")\n";
        
        if (playerAttackRoll >= 17 && area == 3) {
            enemy.updatehealth(playerCenter.x, playerCenter.y);
            enemy1.updatehealth(playerCenter.x, playerCenter.y);
        }
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isAttacking = false;
            attacking = false;
            // Transition to idle
            rectSourceSprite.top = 2496 + (direction - 1) * 156;
            clothesSourceSprite.top = isPlatemailEquipped ? 3744 : rectSourceSprite.top;
            armorSourceSprite.top = isPlatemailEquipped ? rectSourceSprite.top + 3744 : 0;
            rectSourceSprite.left = 0;
            clothesSourceSprite.left = 0;
            armorSourceSprite.left = 0;
            player.setTextureRect(rectSourceSprite);
            playerclothes.setTextureRect(clothesSourceSprite);
            playerarmor.setTextureRect(armorSourceSprite);
            idle = 1;
        }
    }
    clock.restart();
}

// Fireplace animation
if (fireplace && fireclock.getElapsedTime().asSeconds() > 0.2) {
    if (fjelhearthSourceSprite.left >= 1760) {
        fjelhearthSourceSprite.left = 0;
    } else {
        fjelhearthSourceSprite.left += 220;
    }
    fjelfireplace.setTextureRect(fjelhearthSourceSprite);
    fireclock.restart();
}

// Barrel destruction animation
if (barrelhealth <= 0 && clock.getElapsedTime().asSeconds() > 0.1) {
    barrelxSourceSprite.left += 100;
    if (barrelxSourceSprite.left >= 400) {
        barrelxSourceSprite.left = 400;
    }
    barrel.setTextureRect(barrelxSourceSprite);
    clock.restart();
}

#endif // ANIMATIONS_H
