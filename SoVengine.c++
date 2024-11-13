#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <tinyxml2.h>
#include <limits>

using namespace sf;

bool stopped = false;
float playerset = 0.0f;
// Define a structure for a 2D vector
struct Vectorx2f {
    float x, y;

    Vectorx2f(float x = 0, float y = 0) : x(x), y(y) {}

    Vectorx2f operator+(const Vectorx2f& other) const {
        return Vectorx2f(x + other.x, y + other.y);
    }

    Vectorx2f operator-(const Vectorx2f& other) const {
        return Vectorx2f(x - other.x, y - other.y);
    }

    float dot(const Vectorx2f& other) const {
        return x * other.x + y * other.y;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vectorx2f normalize() const {
        float magnitude = this->magnitude();
        return Vectorx2f(x / magnitude, y / magnitude);
    }
};

// Define a structure for a polygon
struct Polygon {
    std::vector<Vectorx2f> vertices;

    // Load vertices from an XML file
    void loadVerticesFromXML(const std::string& filename) {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Error loading XML file: " << filename << std::endl;
            return;
        }

        tinyxml2::XMLElement* polygonElement = doc.FirstChildElement("bodydef")
            ->FirstChildElement("bodies")
            ->FirstChildElement("body")
            ->FirstChildElement("fixture")
            ->FirstChildElement("polygon");

        if (!polygonElement) {
            std::cerr << "No polygon element found in XML." << std::endl;
            return;
        }

        for (tinyxml2::XMLElement* vertexElement = polygonElement->FirstChildElement("vertex");
             vertexElement != nullptr;
             vertexElement = vertexElement->NextSiblingElement("vertex")) {
            float x = vertexElement->FloatAttribute("x");
            float y = vertexElement->FloatAttribute("y");
            vertices.push_back(Vectorx2f(x, y));
        }
    }

    // Function to get the edges of the polygon
    std::vector<Vectorx2f> getEdges() const {
        std::vector<Vectorx2f> edges;
        for (size_t i = 0; i < vertices.size(); ++i) {
            size_t next = (i + 1) % vertices.size();
            edges.push_back(vertices[next] - vertices[i]);
        }
        return edges;
    }

    // Get the current position of the polygon (using the first vertex as the reference)
    Vectorx2f getPosition() const {
        return vertices.empty() ? Vectorx2f(0, 0) : vertices[0]; // Return the first vertex as the position
    }

    // Set the position of the polygon based on a new position
    void setPosition(const Vectorx2f& newPosition) {
        Vectorx2f currentPosition = getPosition(); // Get the current position
        Vectorx2f offset = newPosition - currentPosition; // Calculate the offset
		
        // Update each vertex position by the offset
        for (auto& vertex : vertices) {
            vertex = vertex + offset;
        }
    }
};

// Function to check for collision between two polygons using SAT
bool checkPolygonCollision(const Polygon& poly1, const Polygon& poly2, Vectorx2f& collisionNormal) {
    std::vector<Vectorx2f> edges1 = poly1.getEdges();
    std::vector<Vectorx2f> edges2 = poly2.getEdges();

    // Check for collision on all edges of both polygons
    for (const auto& edge : edges1) {
        Vectorx2f normal = Vectorx2f(-edge.y, edge.x).normalize(); // Normal of the edge
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::min();
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::min();

        for (const auto& vertex : poly1.vertices) {
            float dotProduct = vertex.dot(normal);
            if (dotProduct < min1) {
                min1 = dotProduct;
            }
            if (dotProduct > max1) {
                max1 = dotProduct;
            }
        }

        for (const auto& vertex : poly2.vertices) {
            float dotProduct = vertex.dot(normal);
            if (dotProduct < min2) {
                min2 = dotProduct;
            }
            if (dotProduct > max2) {
                max2 = dotProduct;
            }
        }

        if (max1 < min2 || max2 < min1) {
			stopped = false;
            return false; // No collision
        }
    }

    for (const auto& edge : edges2) {
        Vectorx2f normal = Vectorx2f(-edge.y, edge.x).normalize(); // Normal of the edge
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::min();
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::min();

        for (const auto& vertex : poly1.vertices) {
            float dotProduct = vertex.dot(normal);
            if (dotProduct < min1) {
                min1 = dotProduct;
            }
            if (dotProduct > max1) {
                max1 = dotProduct;
            }
        }

        for (const auto& vertex : poly2.vertices) {
            float dotProduct = vertex.dot(normal);
            
            if (dotProduct < min2) {
                min2 = dotProduct;
            }
            if (dotProduct > max2) {
                max2 = dotProduct;
            }
        }

        if (max1 < min2 || max2 < min1) {
            return false; // No collision
        }
    }

    // If we reach here, it means there is a collision
	
	stopped = true;
    collisionNormal = Vectorx2f(0, 0); // Placeholder for collision normal
	
    return true; // Collision detected
}

// Function to resolve the collision
void resolveCollision(Polygon& collisionx, const Vectorx2f& collisionNormal) {
    for (auto& vertex : collisionx.vertices) {
        vertex = vertex + collisionNormal; // Move player out of collision
		
    }
}

int main()
{

	Polygon xtavern;
    xtavern.loadVerticesFromXML("/home/z3ta/c++/SoV/images/sprites/lht.xml");

	// Create a player polygon (initial position)
    Polygon collisionx;
    collisionx.vertices = {
        Vectorx2f(1700, 1700), // Initial position of player
        Vectorx2f(1700, 1750),
        Vectorx2f(1750, 1750),
        Vectorx2f(1750, 1700)
    };

	
	srand(time(NULL));

	const unsigned WINDOW_WIDTH = 1400;
	const unsigned WINDOW_HEIGHT = 1400;
	const unsigned WINDOW_WIDTHX = 1400;
	const unsigned WINDOW_HEIGHTX = 800;
    bool walkright = 0;
	bool walkleft = 0;
    bool movediag1 = 0;
	bool movediag2 = 0;
    int playervelx = 0;
	int playervely = 0;
	int moveupleftdiagkey1 = 0;
	int moveupleftdiagkey2 = 0;
	int movedownleftdiagkey1 = 0;
	int movedownleftdiagkey2 = 0;
	int moveuprightdiagkey1 = 0;
	int moveuprightdiagkey2 = 0;
	int movedownrightdiagkey1 = 0;
	int movedownrightdiagkey2 = 0;
	int up = 1;
	int right = 2;
	int down = 3;
	int left = 4;
	int upleft = 5;
	int upright = 6;
	int downright = 7;
	int downleft = 8;
	int direction = 1;
    int directionx = 0;
	int idle = 1;
	bool isMoving = false;
	bool isMovingx = true;
	bool attacking = false;
	int repose = 0;
	float playerSpeed = 15.0f;
	
	RenderWindow window(VideoMode(1400, 1400), "Shadows of Vaalundroth", Style::Default);
	window.setFramerateLimit(60);
	
    //Background
    sf::Texture backgroundtxtr;
    backgroundtxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town1x.jpg");
    
    sf::Sprite background;
    background.setTexture(backgroundtxtr);
    background.setPosition(0, -4200);

	//Player Coordinates
	sf::Vector2f playerPosition(1700, 1700);
	sf::Vector2f xPosition(1500, 1500);
	sf::Vector2f xAdjust(104, 140);
	//Object Coordinates
	sf::Vector2f barrelPosition(720, 2400);
	sf::Vector2f barreltopPosition(720, 1936);
	//Building Coordinates
	sf::Vector2f tavernPosition(0, 0);

	//Player
    sf::Texture playertxtr;
    playertxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");
    sf::IntRect rectSourceSprite(0, 0, 127, 127);
	sf::IntRect barrelSourceSprite(0, 0, 71, 98);
	sf::IntRect xSprite(0, 0, 24, 19);
    sf::Sprite player(playertxtr, rectSourceSprite);
	sf::Sprite xcollision(playertxtr, xSprite);
	player.setPosition(playerPosition);
	sf::Texture collisionxtxtr;
	collisionxtxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");
	
    sf::Clock clock;
    player.setTexture(playertxtr);
	
	xcollision.setTexture(playertxtr);
	xcollision.setPosition(xPosition);
	sf::Vector2f sizeIncrease(1.0f, 1.0f);
	sf::Vector2f sizeDecrease(0.1f, 0.1f);
	player.setScale(sf::Vector2f(player.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
	xcollision.setScale(sf::Vector2f(xcollision.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
	
	
	//Barrels
	sf::Texture barreltxtr;
	barreltxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barrel1.png");
	sf::Sprite barrel(barreltxtr);
	barrel.setPosition(barrelPosition);
	sf::Sprite barrelx(barreltxtr);
	barrelx.setPosition(barrelPosition);


	sf::Texture barreltoptxtr;
	barreltoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barrel1top.png");
	sf::Sprite barreltop(barreltoptxtr);
	barreltop.setPosition(barreltopPosition);

	//Buildings
	sf::Texture taverntxtr;
	taverntxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/longhousetavernsprite.png");
	sf::Sprite tavern(taverntxtr);
	tavern.setPosition(tavernPosition);

	//Vectors
	Vector2f playerCenter;
	Vector2f playerRight;
	Vector2f playerLeft;
	Vector2f enemyxCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	while (window.isOpen())
	{
		Event event;
		Polygon playerOriginal = collisionx;
		
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
        
		//Directional rotation
        if (sf::Keyboard::isKeyPressed(Keyboard::W))
		{
			direction = 5;
			idle = 0;
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::D))
		{
			direction = 7;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::S))
		{
			direction = 1;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::A))
		{
			direction = 3;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::W) && (Keyboard::isKeyPressed(sf::Keyboard::A)))
		{
			direction = 4;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::W) && (sf::Keyboard::isKeyPressed(Keyboard::D)))
		{
			direction = 6;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::D) && (sf::Keyboard::isKeyPressed(Keyboard::S)))
		{
			direction = 8;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::S) && (sf::Keyboard::isKeyPressed(Keyboard::A)))
		{
			direction = 2;
			idle = 0;
			
		}

		//Walking animations
		
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 1 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2032;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
        if (clock.getElapsedTime().asSeconds() > 0.1f && direction == 2 && idle == 0)
        {
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2159;
			isMoving = true;
            if (rectSourceSprite.left >= 700)
                rectSourceSprite.left = 0;
            else
                rectSourceSprite.left += 127;
            
            player.setTextureRect(rectSourceSprite);
            clock.restart();
        }
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 3 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2286;
			isMoving = true;
            if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else 
			    rectSourceSprite.left += 127;
		
		    player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1f && direction == 4 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2413;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 5 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2540;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 6 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2667;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 7 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2794;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 8 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2921;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		

		//Idle animations
		if (event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::W)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::A)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::S)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::D)
			{
				
				isMoving = false;
				
			}

		if (idle == 1 && direction == 1 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1016;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 2 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1143;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 3 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1270;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 4 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1397;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 5 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1524;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 6 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1651;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 7 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1778;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 8 && isMoving == false){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1905;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 1){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2032;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 2){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2159;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 3){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2286;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 4){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2413;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 5){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2540;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 6){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2667;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 7){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2794;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (isMoving == true && direction == 8){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2921;
			
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		}
		//Attack animations
		
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asSeconds() > 0.1)
        {
			
			if (direction == 1)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 0;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 2)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 127;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 3)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 254;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 4)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 381;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 5)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 508;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 6)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 635;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 7)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 762;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 8)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 889;
			    if (rectSourceSprite.left >= 700)
			        rectSourceSprite.left = 0;
			    else
			        rectSourceSprite.left += 127;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
	    
        }

		//Vectors
		playerCenter = Vector2f(player.getPosition());
		playerRight = Vector2f(player.getPosition());
		playerLeft = Vector2f(player.getPosition());
		
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		float aimDirMagnitude = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		
        if (aimDirMagnitude != 0) 
		{
            aimDirNorm = aimDir / aimDirMagnitude;
        } 
		
		else 
		
		{
            aimDirNorm = aimDir; // Handle division by zero or very small magnitude case
        }

		float PI = 3.14159265f;
		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
		//player.setRotation(deg + 90);

		//Notes
		
        
		//Player
		if (Keyboard::isKeyPressed(Keyboard::A) && stopped == false)
			player.move(-playerSpeed, 0.f);
			
		else{
			idle = 1;
		}
		if (Keyboard::isKeyPressed(Keyboard::D) && stopped == false)
			player.move(playerSpeed, 0.f);
		else{
			idle = 1;
		}	
		if (Keyboard::isKeyPressed(Keyboard::W) && stopped == false)
			player.move(0.f, -playerSpeed);
		else{
			idle = 1;
		}	
		if (Keyboard::isKeyPressed(Keyboard::S) && stopped == false)
			player.move(0.f, playerSpeed);
		else{
			idle = 1;
		}

		// Move player with arrow keys
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            for (auto& vertex : collisionx.vertices) {
                vertex.x -= 15; // Move left
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            for (auto& vertex : collisionx.vertices) {
                vertex.x += 15; // Move right
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            for (auto& vertex : collisionx.vertices) {
                vertex.y -= 15; // Move up
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            for (auto& vertex : collisionx.vertices) {
                vertex.y += 15; // Move down
            }
        }
		
		// Check for collision
        Vectorx2f collisionNormal;
        if (checkPolygonCollision(xtavern, collisionx, collisionNormal)) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		// Draw the player polygon
        sf::ConvexShape playerShape;
        playerShape.setPointCount(collisionx.vertices.size());
        for (size_t i = 0; i < collisionx.vertices.size(); ++i) {
            playerShape.setPoint(i, sf::Vector2f(collisionx.vertices[i].x, collisionx.vertices[i].y));
        }
		
        playerShape.setFillColor(sf::Color::Green);
		
		//Draw
        sf::View view;
		view.setCenter(player.getPosition());
		
		// Draw the building polygon
        sf::ConvexShape xtavernShape;
        xtavernShape.setPointCount(xtavern.vertices.size());
        for (size_t i = 0; i < xtavern.vertices.size(); ++i) {
            xtavernShape.setPoint(i, sf::Vector2f(xtavern.vertices[i].x, xtavern.vertices[i].y));
        }

        xtavernShape.setFillColor(sf::Color::Blue);

		window.clear();
		
		window.setView(view);
		
        window.draw(background);

		window.draw(xtavernShape);

		window.draw(barrel);
		
		window.draw(tavern);

		window.draw(playerShape);

		window.draw(player);

		window.display();
	}

	return 0;
}