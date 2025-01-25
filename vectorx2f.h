#ifndef VECTORX2F_H
#define VECTORX2F_H

#include <SFML/Graphics.hpp>  // For sf::Vector2f
#include <cmath>               // For std::sqrt

struct Vectorx2f {
    float x, y;

    // Constructor
    Vectorx2f(float x = 0, float y = 0) : x(x), y(y) {}

    // Operator overloading for vector addition and subtraction
    Vectorx2f operator+(const Vectorx2f& other) const {
        return Vectorx2f(x + other.x, y + other.y);
    }

    Vectorx2f operator-(const Vectorx2f& other) const {
        return Vectorx2f(x - other.x, y - other.y);
    }

    // Dot product of two vectors
    float dot(const Vectorx2f& other) const {
        return x * other.x + y * other.y;
    }

    // Magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize the vector
    Vectorx2f normalize() const {
        float mag = this->magnitude();
        return Vectorx2f(x / mag, y / mag);
    }

    // Convert from sf::Vector2f to Vectorx2f
    static Vectorx2f fromSF(const sf::Vector2f& sfVec) {
        return Vectorx2f(sfVec.x, sfVec.y);
    }

    // Convert from Vectorx2f to sf::Vector2f
    sf::Vector2f toSF() const {
        return sf::Vector2f(x, y);
    }
};

#endif // VECTORX2F_H