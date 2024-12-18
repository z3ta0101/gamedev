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

	// Create a player polygon (initial position)
    Polygon collisionx;
    collisionx.vertices = {
        Vectorx2f(2790, 1840), // Initial position of player
        Vectorx2f(2790, 1910),
        Vectorx2f(2880, 1910),
        Vectorx2f(2880, 1840)
    };

	Polygon attackx;
	attackx.vertices = {
		Vectorx2f(2749, 1945), //attack range polygon
		Vectorx2f(2749, 1805),
		Vectorx2f(2919, 1805),
		Vectorx2f(2919, 1945)
	};

	Polygon barrelx;
	barrelx.vertices = {
		Vectorx2f(1705, 1090), //attack range polygon
		Vectorx2f(1805, 1090),
		Vectorx2f(1805, 1010),
		Vectorx2f(1705, 1010)
	};

	//Felstrond collision barriers

	Polygon fboundry1;
	fboundry1.vertices = {
		
		Vectorx2f(9050, -1414),
		Vectorx2f(8808, -1041),
		Vectorx2f(6695, -2250),
		Vectorx2f(6880, -2988)
		
	};

	Polygon fboundry2;
	fboundry2.vertices = {
		
		Vectorx2f(8955, -935),
		Vectorx2f(9095, -320),
		Vectorx2f(9625, -240),
		Vectorx2f(9625, -1350),
		Vectorx2f(9145, -1305)
	};

	Polygon fboundry3;
	fboundry3.vertices = {
		
		Vectorx2f(9640, -465),
		Vectorx2f(9640, 40),
		Vectorx2f(9550, 40),
		Vectorx2f(9550, -465)
	};

	Polygon fboundry4;
	fboundry4.vertices = {
		
		Vectorx2f(9620, 75),
		Vectorx2f(9340, 75),
		Vectorx2f(9340, 200),
		Vectorx2f(9620, 200)
	};

	Polygon fboundry5;
	fboundry5.vertices = {
		
		Vectorx2f(9225, 190),
		Vectorx2f(9225, 610),
		Vectorx2f(9640, 1375),
		Vectorx2f(9945, 1375),
		Vectorx2f(9945, 485)
		
	};

	Polygon fboundry6;
	fboundry6.vertices = {
		
		Vectorx2f(9720, 1425),
		Vectorx2f(3235, 4535)
		
	};

	Polygon fboundry7;
	fboundry7.vertices = {
		
		Vectorx2f(3235, 4480),
		Vectorx2f(510, 3115)
		
	};

	Polygon fboundry8;
	fboundry8.vertices = {
		
		Vectorx2f(610, 3070),
		Vectorx2f(1055, 2245)
		
	};

	Polygon fboundry9;
	fboundry9.vertices = {
		
		Vectorx2f(935, 2030),
		Vectorx2f(765, 1500)
		
	};

	Polygon fboundry10;
	fboundry10.vertices = {
		
		Vectorx2f(1410, -500),
		Vectorx2f(1405, -1095)
		
	};

	Polygon fboundry11;
	fboundry11.vertices = {
		
		Vectorx2f(1405, -1065),
		Vectorx2f(940, -1550)
		
	};

	Polygon fboundry12;
	fboundry12.vertices = {
		
		Vectorx2f(940, -1510),
		Vectorx2f(5690, -2790
)
		
	};

	Polygon fboundry13;
	fboundry13.vertices = {
		
		Vectorx2f(5690, -2830),
		Vectorx2f(6655, -2515)
		
	};

	Polygon fboundry14;
	fboundry14.vertices = {
		
		Vectorx2f(1035, 2180),
		Vectorx2f(865, 1500)
		
	};

	Polygon fboundry15;
	fboundry15.vertices = {
		
		Vectorx2f(860, 1490),
		Vectorx2f(1390, -440)
		
	};

	Polygon fhouseb1;
	fhouseb1.vertices = {
		Vectorx2f(3100, 3985),
		Vectorx2f(2440, 3585)
		
	};

	Polygon fhouseb2;
	fhouseb2.vertices = {
		Vectorx2f(2440, 3585),
		Vectorx2f(4205, 1670)
	};

	Polygon fhouseb3;
	fhouseb3.vertices = {
		Vectorx2f(4205, 1670),
		Vectorx2f(4575, 2505)
		
	};

	Polygon fhouseb4;
	fhouseb4.vertices = {
		Vectorx2f(4575, 2540),
		Vectorx2f(3100, 4025)
		
	};

	//Fjelenvar collision barriers

	//Top

	Polygon fjelbarrier1;
	fjelbarrier1.vertices = {
		Vectorx2f(1640, -3455),
		Vectorx2f(2100, -3545)
	};

	Polygon fjelbarrier2;
	fjelbarrier2.vertices = {
		Vectorx2f(2100, -3545),
		Vectorx2f(7655, -3550)
	};

	Polygon fjelbarrier3;
	fjelbarrier3.vertices = {
		Vectorx2f(7700, -3505),
		Vectorx2f(8150, -2710)
	};

	Polygon fjelbarrier4;
	fjelbarrier4.vertices = {
		Vectorx2f(8130, -2680),
		Vectorx2f(8400, -2755)
	};

	Polygon fjelbarrier5;
	fjelbarrier5.vertices = {
		Vectorx2f(8400, -2755),
		Vectorx2f(7975, -3625)
	};

	Polygon fjelbarrier6;
	fjelbarrier6.vertices = {
		Vectorx2f(7975, -3540),
		Vectorx2f(8505, -3540)
	};

	Polygon fjelbarrier7;
	fjelbarrier7.vertices = {
		Vectorx2f(8585, -3625),
		Vectorx2f(8555, -2640)
	};

	Polygon fjelbarrier8;
	fjelbarrier8.vertices = {
		Vectorx2f(8630, -2770),
		Vectorx2f(8570, -2515)
	};

	Polygon fjelbarrier9;
	fjelbarrier9.vertices = {
		Vectorx2f(8510, -2545),
		Vectorx2f(7590, -1730)
	};

	Polygon fjelbarrier10;
	fjelbarrier10.vertices = {
		Vectorx2f(7590, -1730),
		Vectorx2f(6870, -1155)
	};

	Polygon fjelbarrier11;
	fjelbarrier11.vertices = {
		Vectorx2f(6870, -1195),
		Vectorx2f(5695, 370)
	};

	Polygon fjelbarrier12;
	fjelbarrier12.vertices = {
		Vectorx2f(5180, 1250),
		Vectorx2f(4865, 1590)
	};

	Polygon fjelbarrier13;
	fjelbarrier13.vertices = {
		Vectorx2f(4865, 1590),
		Vectorx2f(4675, 1555)
	};

	Polygon fjelbarrier14;
	fjelbarrier14.vertices = {
		Vectorx2f(4675, 1555),
		Vectorx2f(2015, 3680)
	};

	Polygon fjelbarrier15;
	fjelbarrier15.vertices = {
		Vectorx2f(2015, 3680),
		Vectorx2f(1800, 3720)
	};

	Polygon fjelbarrier16;
	fjelbarrier16.vertices = {
		Vectorx2f(1800, 3720),
		Vectorx2f(1100, 4275)
	};

	Polygon fjelbarrier17;
	fjelbarrier17.vertices = {
		Vectorx2f(1100, 4275),
		Vectorx2f(675, 3860)
	};

	Polygon fjelbarrier18;
	fjelbarrier18.vertices = {
		Vectorx2f(675, 3860),
		Vectorx2f(565, 2920)
	};

	Polygon fjelbarrier19;
	fjelbarrier19.vertices = {
		Vectorx2f(565, 2920),
		Vectorx2f(1360, 1305)
	};

	Polygon fjelbarrier20;
	fjelbarrier20.vertices = {
		Vectorx2f(1360, 1305),
		Vectorx2f(1360, 1075)
	};

	Polygon fjelbarrier21;
	fjelbarrier21.vertices = {
		Vectorx2f(1360, 1075),
		Vectorx2f(645, 290)
	};

	Polygon fjelbarrier22;
	fjelbarrier22.vertices = {
		Vectorx2f(645, 290),
		Vectorx2f(645, -3090)
	};

	Polygon fjelbarrier24;
	fjelbarrier24.vertices = {
		Vectorx2f(115, -3100),
		Vectorx2f(140, 3270)
	};

	Polygon fjelbarrier25;
	fjelbarrier25.vertices = {
		Vectorx2f(535, -3120),
		Vectorx2f(1635, -3415)
	};

	//Bottom

	Polygon fjelbarrier26;
	fjelbarrier26.vertices = {
		Vectorx2f(7050, 2855),
		Vectorx2f(6915, 2980)
	};
	
	Polygon fjelbarrier27;
	fjelbarrier27.vertices = {
		Vectorx2f(6915, 2980),
		Vectorx2f(6905, 3050)
	};

	Polygon fjelbarrier28;
	fjelbarrier28.vertices = {
		Vectorx2f(6905, 3050),
		Vectorx2f(6630, 3400)
	};

	Polygon fjelbarrier29;
	fjelbarrier29.vertices = {
		Vectorx2f(6630, 3400),
		Vectorx2f(6535, 3380)
	};

	Polygon fjelbarrier30;
	fjelbarrier30.vertices = {
		Vectorx2f(6535, 3380),
		Vectorx2f(6380, 3887)
	};

	Polygon fjelbarrier31;
	fjelbarrier31.vertices = {
		Vectorx2f(6380, 3887),
		Vectorx2f(6445, 3420)
	};

	Polygon fjelbarrier32;
	fjelbarrier32.vertices = {
		Vectorx2f(6445, 3420),
		Vectorx2f(6380, 4060)
	};

	Polygon fjelbarrier33;
	fjelbarrier33.vertices = {
		Vectorx2f(6380, 4060),
		Vectorx2f(6395, 4135)
	};

	Polygon fjelbarrier34;
	fjelbarrier34.vertices = {
		Vectorx2f(6395, 4135),
		Vectorx2f(6230, 4220)
	};

	Polygon fjelbarrier35;
	fjelbarrier35.vertices = {
		Vectorx2f(6230, 4220),
		Vectorx2f(6230, 4310)
	};

	Polygon fjelbarrier36;
	fjelbarrier36.vertices = {
		Vectorx2f(6230, 4310),
		Vectorx2f(5580, 5280)
	};

	Polygon fjelbarrier37;
	fjelbarrier37.vertices = {
		Vectorx2f(5580, 5280),
		Vectorx2f(5675, 5725)
	};

	Polygon fjelbarrier38;
	fjelbarrier38.vertices = {
		Vectorx2f(5675, 5715),
		Vectorx2f(6100, 5600)
	};

	Polygon fjelbarrier39;
	fjelbarrier39.vertices = {
		Vectorx2f(6100, 5900),
		Vectorx2f(8970, 5705)
	};

	Polygon fjelbarrier40;
	fjelbarrier40.vertices = {
		Vectorx2f(8970, 5705),
		Vectorx2f(9175, 5420)
	};

	Polygon fjelbarrier41;
	fjelbarrier41.vertices = {
		Vectorx2f(9175, 5420),
		Vectorx2f(9405, 5345)
	};

	Polygon fjelbarrier42;
	fjelbarrier42.vertices = {
		Vectorx2f(9405, 5345),
		Vectorx2f(9405, 5205)
	};

	Polygon fjelbarrier43;
	fjelbarrier43.vertices = {
		Vectorx2f(9405, 5205),
		Vectorx2f(9660, 4845)
	};

	Polygon fjelbarrier44;
	fjelbarrier44.vertices = {
		Vectorx2f(9660, 4845),
		Vectorx2f(9755, 4630)
	};

	Polygon fjelbarrier45;
	fjelbarrier45.vertices = {
		Vectorx2f(9755, 4630),
		Vectorx2f(9725, 4390)
	};

	Polygon fjelbarrier46;
	fjelbarrier46.vertices = {
		Vectorx2f(9725, 4390),
		Vectorx2f(9820, 3895)
	};

	Polygon fjelbarrier47;
	fjelbarrier47.vertices = {
		Vectorx2f(9820, 3895),
		Vectorx2f(9740, 3060)
	};

	Polygon fjelbarrier48;
	fjelbarrier48.vertices = {
		Vectorx2f(9740, 3060),
		Vectorx2f(9795, 2610)
	};

	Polygon fjelbarrier49;
	fjelbarrier49.vertices = {
		Vectorx2f(9795, 2610),
		Vectorx2f(9935, 2490)
	};

	Polygon fjelbarrier50;
	fjelbarrier50.vertices = {
		Vectorx2f(9935, 2490),
		Vectorx2f(9935, 2215)
	};

	Polygon fjelbarrier51;
	fjelbarrier51.vertices = {
		Vectorx2f(9935, 2215),
		Vectorx2f(10270, 1870)
	};

	Polygon fjelbarrier52;
	fjelbarrier52.vertices = {
		Vectorx2f(10270, 1870),
		Vectorx2f(9460, 1005)
	};

	Polygon fjelbarrier53;
	fjelbarrier53.vertices = {
		Vectorx2f(9460, 1005),
		Vectorx2f(9870, 615)
	};

	Polygon fjelbarrier54;
	fjelbarrier54.vertices = {
		Vectorx2f(9870, 615),
		Vectorx2f(10245, 615)
	};

	Polygon fjelbarrier55;
	fjelbarrier55.vertices = {
		Vectorx2f(10245, 615),
		Vectorx2f(10245, -2085)
	};

	Polygon fjelbarrier56;
	fjelbarrier56.vertices = {
		Vectorx2f(10245, -2085),
		Vectorx2f(8555, -2250)
	};

	Polygon fjelbarrier57;
	fjelbarrier57.vertices = {
		Vectorx2f(10045, 75),
		Vectorx2f(9985, 75)
	};

	Polygon fjelbarrier58;
	fjelbarrier58.vertices = {
		Vectorx2f(9985, 75),
		Vectorx2f(9910, 165)
	};

	Polygon fjelbarrier59;
	fjelbarrier59.vertices = {
		Vectorx2f(9910, 165),
		Vectorx2f(9910, 270)
	};

	Polygon fjelbarrier60;
	fjelbarrier60.vertices = {
		Vectorx2f(9910, 270),
		Vectorx2f(9325, 975)
	};

	Polygon fjelbarrier61;
	fjelbarrier61.vertices = {
		Vectorx2f(9325, 995),
		Vectorx2f(9280, 980)
	};

	Polygon fjelbarrier62;
	fjelbarrier62.vertices = {
		Vectorx2f(9280, 980),
		Vectorx2f(9140, 1105)
	};

	Polygon fjelbarrier63;
	fjelbarrier63.vertices = {
		Vectorx2f(9140, 1105),
		Vectorx2f(9105, 1160)
	};

	Polygon fjelbarrier64;
	fjelbarrier64.vertices = {
		Vectorx2f(9105, 1160),
		Vectorx2f(9000, 1210)
	};

	Polygon fjelbarrier65;
	fjelbarrier65.vertices = {
		Vectorx2f(9000, 1210),
		Vectorx2f(8880, 1295)
	};

	Polygon fjelbarrier66;
	fjelbarrier66.vertices = {
		Vectorx2f(8880, 1295),
		Vectorx2f(8835, 1270)
	};

	Polygon fjelbarrier67;
	fjelbarrier67.vertices = {
		Vectorx2f(8835, 1270),
		Vectorx2f(8690, 1385)
	};

	Polygon fjelbarrier68;
	fjelbarrier68.vertices = {
		Vectorx2f(8690, 1385),
		Vectorx2f(8415, 1620)
	};

	Polygon fjelbarrier69;
	fjelbarrier69.vertices = {
		Vectorx2f(8415, 1620),
		Vectorx2f(8415, 1720)
	};

	Polygon fjelbarrier70;
	fjelbarrier70.vertices = {
		Vectorx2f(8415, 1720),
		Vectorx2f(8285, 1850)
	};

	Polygon fjelbarrier71;
	fjelbarrier71.vertices = {
		Vectorx2f(8285, 1850),
		Vectorx2f(8135, 1955)
	};

	Polygon fjelbarrier72;
	fjelbarrier72.vertices = {
		Vectorx2f(8135, 1955),
		Vectorx2f(8020, 1990)
	};

	Polygon fjelbarrier73;
	fjelbarrier73.vertices = {
		Vectorx2f(8020, 1970),
		Vectorx2f(7930, 2060)
	};

	//Bridge

	Polygon fjelbarrier74;
	fjelbarrier74.vertices = {
		Vectorx2f(5730, 370),
		Vectorx2f(5860, 345)
	};

	Polygon fjelbarrier75;
	fjelbarrier75.vertices = {
		Vectorx2f(5860, 345),
		Vectorx2f(5970, 355)
	};

	Polygon fjelbarrier76;
	fjelbarrier76.vertices = {
		Vectorx2f(5970, 325),
		Vectorx2f(6345, 460)
	};

	Polygon fjelbarrier77;
	fjelbarrier77.vertices = {
		Vectorx2f(6345, 440),
		Vectorx2f(6790, 755)
	};

	Polygon fjelbarrier78;
	fjelbarrier78.vertices = {
		Vectorx2f(6790, 755),
		Vectorx2f(7280, 1235)
	};

	Polygon fjelbarrier79;
	fjelbarrier79.vertices = {
		Vectorx2f(7280, 1235),
		Vectorx2f(7665, 1680)
	};

	Polygon fjelbarrier80;
	fjelbarrier80.vertices = {
		Vectorx2f(7665, 1680),
		Vectorx2f(7920, 2060)
	};

	Polygon fjelbarrier81;
	fjelbarrier81.vertices = {
		Vectorx2f(7105, 2845),
		Vectorx2f(6705, 2200)
	};

	Polygon fjelbarrier82;
	fjelbarrier82.vertices = {
		Vectorx2f(6705, 2200),
		Vectorx2f(5785, 1445)
	};

	Polygon fjelbarrier83;
	fjelbarrier83.vertices = {
		Vectorx2f(5785, 1445),
		Vectorx2f(5225, 1250)
	};

	//wilderness1 barriers

	//tree1

	Polygon w1barrier1;
	w1barrier1.vertices = {
		Vectorx2f(1700, -1500),
		Vectorx2f(2065, -1300
)
	};

	Polygon w1barrier2;
	w1barrier2.vertices = {
		Vectorx2f(2065, -1300),
		Vectorx2f(2465, -1300)
	};

	Polygon w1barrier3;
	w1barrier3.vertices = {
		Vectorx2f(2465, -1300),
		Vectorx2f(2920, -1500)
	};

	Polygon w1barrier4;
	w1barrier4.vertices = {
		Vectorx2f(2920, -1500),
		Vectorx2f(1700, -1500)
	};

	//tree2

	Polygon w1barrier5;
	w1barrier5.vertices = {
		Vectorx2f(6885, -3385),
		Vectorx2f(7715, -3320)
	};

	Polygon w1barrier6;
	w1barrier6.vertices = {
		Vectorx2f(7715, -3320),
		Vectorx2f(7870, -3420)
	};

	Polygon w1barrier7;
	w1barrier7.vertices = {
		Vectorx2f(7870, -3420),
		Vectorx2f(7735, -3635)
	};

	Polygon w1barrier8;
	w1barrier8.vertices = {
		Vectorx2f(7735, -3635),
		Vectorx2f(6885, -3385)
	};

	//tree3

	Polygon w1barrier9;
	w1barrier9.vertices = {
		Vectorx2f(9775, -1595),
		Vectorx2f(9920, -1330)
	};

	Polygon w1barrier10;
	w1barrier10.vertices = {
		Vectorx2f(9920, -1330),
		Vectorx2f(10455, -1090)
	};

	Polygon w1barrier11;
	w1barrier11.vertices = {
		Vectorx2f(10455, -1090),
		Vectorx2f(10965, -1185)
	};

	Polygon w1barrier12;
	w1barrier12.vertices = {
		Vectorx2f(10965, -1185),
		Vectorx2f(10965, -1580)
	};

	Polygon w1barrier13;
	w1barrier13.vertices = {
		Vectorx2f(10965, 1580),
		Vectorx2f(9775, -1595)
	};

	//tree4

	Polygon w1barrier14;
	w1barrier14.vertices = {
		Vectorx2f(10315, 2685),
		Vectorx2f(10685, 2830)
	};

	Polygon w1barrier15;
	w1barrier15.vertices = {
		Vectorx2f(10685, 2830),
		Vectorx2f(10970, 2755)
	};

	Polygon w1barrier16;
	w1barrier16.vertices = {
		Vectorx2f(10970, 2755),
		Vectorx2f(10315, 2685)
	};

	//cliffside

	Polygon w1barrier17;
	w1barrier17.vertices = {
		Vectorx2f(2685, 665),
		Vectorx2f(3500, 890)
	};

	Polygon w1barrier18;
	w1barrier18.vertices = {
		Vectorx2f(3500, 890),
		Vectorx2f(3755, 845)
	};

	Polygon w1barrier19;
	w1barrier19.vertices = {
		Vectorx2f(3755, 845),
		Vectorx2f(4020, 645)
	};

	Polygon w1barrier20;
	w1barrier20.vertices = {
		Vectorx2f(4020, 645),
		Vectorx2f(4185, 420)
	};

	Polygon w1barrier21;
	w1barrier21.vertices = {
		Vectorx2f(4185, 420),
		Vectorx2f(5155, -335
)
	};

	Polygon w1barrier22;
	w1barrier22.vertices = {
		Vectorx2f(5605, -335),
		Vectorx2f(5345, -705)
	};

	Polygon w1barrier23;
	w1barrier23.vertices = {
		Vectorx2f(5345, -705),
		Vectorx2f(6230, -1030)
	};

	Polygon w1barrier24;
	w1barrier24.vertices = {
		Vectorx2f(6230, -1030),
		Vectorx2f(7180, -1275)
	};

	Polygon w1barrier25;
	w1barrier25.vertices = {
		Vectorx2f(7180, -1275),
		Vectorx2f(8825, -2475)
	};

	Polygon w1barrier26;
	w1barrier26.vertices = {
		Vectorx2f(8825, -2475),
		Vectorx2f(8410, -2665)
	};

	Polygon w1barrier27;
	w1barrier27.vertices = {
		Vectorx2f(8410, -2665),
		Vectorx2f(7380, -2100)
	};

	Polygon w1barrier28;
	w1barrier28.vertices = {
		Vectorx2f(7380, -2100),
		Vectorx2f(5320, -1715)
	};

	Polygon w1barrier29;
	w1barrier29.vertices = {
		Vectorx2f(5320, -1715),
		Vectorx2f(4810, -1255)
	};

	Polygon w1barrier30;
	w1barrier30.vertices = {
		Vectorx2f(4810, -1255),
		Vectorx2f(2765, 465)
	};

	Polygon w1barrier31;
	w1barrier31.vertices = {
		Vectorx2f(2765, 465),
		Vectorx2f(2685, 665)
	};

	//map bounds

	Polygon w1barrier32;
	w1barrier32.vertices = {
		Vectorx2f(-30, -4270),
		Vectorx2f(-30, 6730)
	};

	Polygon w1barrier33;
	w1barrier33.vertices = {
		Vectorx2f(-30, 6730),
		Vectorx2f(10965, 6730)
	};

	Polygon w1barrier34;
	w1barrier34.vertices = {
		Vectorx2f(10965, 6730),
		Vectorx2f(10965, -4270)
	};

	Polygon w1barrier35;
	w1barrier35.vertices = {
		Vectorx2f(10965, -4270),
		Vectorx2f(-30, -4270)
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
	bool round = false;
	int repose = 0;
	float playerSpeed = 5.0f;
	int barrelhealth = 2;
	int area = 3;
	
	
	RenderWindow window(VideoMode(1400, 1400), "Shadows of Vaalundroth", Style::Default);
	window.setFramerateLimit(60);
	
    //Felstrond backdrop
    sf::Texture backgroundtxtr;
    backgroundtxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town1x.jpg");
    
    sf::Sprite background;
    background.setTexture(backgroundtxtr);
    background.setPosition(0, -4200);

	//Felstrond cave
	sf::Texture cavetxtr;
	cavetxtr.loadFromFile("/home/z3ta/c++/SoV/images/layers/fcaveoverlayer.png");

	sf::Sprite overhang;
	overhang.setTexture(cavetxtr);
	overhang.setPosition(9228, -560);

	//fjelenvår backdrop
    sf::Texture fjelenvartxtr;
    fjelenvartxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town2.jpg");
    
    sf::Sprite fjelenvar;
    fjelenvar.setTexture(fjelenvartxtr);
    fjelenvar.setPosition(0, -4200);
	
	//fhouse1
	sf::Texture fhouse1txtr;
	fhouse1txtr.loadFromFile("/home/z3ta/c++/SoV/images/layers/fhouse1top.png");

	sf::Sprite fhouse1x;
	fhouse1x.setTexture(fhouse1txtr);
	fhouse1x.setPosition(2439, 1367);

	//wilderness1 backdrop
	sf::Texture wilderness1txtr;
	wilderness1txtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/wilderness1.jpg");

	sf::Sprite wilderness1;
	wilderness1.setTexture(wilderness1txtr);
	wilderness1.setPosition(0, -4200);

	//Player Coordinates
	sf::Vector2f playerPosition(2700, 1700);
	sf::Vector2f xPosition(1500, 1500);
	sf::Vector2f xAdjust(104, 140);
	//Object Coordinates
	sf::Vector2f barrelPosition(1720, 990);
	sf::Vector2f barreltopPosition(720, 1936);
	//Building Coordinates
	sf::Vector2f tavernPosition(1000, -1000);
	sf::Vector2f tavernTopPosition(1000, -1000);
	sf::Vector2f whouseTopPosition(6500, 4184);
	sf::Vector2f wtowerTopPosition(8453, 1015);
	sf::Vector2f bhouseTopPosition(6732, -3775);
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
	barreltxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barreldestructsequence1.png");
	sf::IntRect barrelxSourceSprite(0, 0, 100, 100);
	sf::Sprite barrel(barreltxtr, barrelxSourceSprite);
	barrel.setPosition(barrelPosition);
	


	sf::Texture barreltoptxtr;
	barreltoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barrel1top.png");
	sf::Sprite barreltop(barreltoptxtr);
	barreltop.setPosition(barreltopPosition);

	//Buildings
	sf::Texture taverntxtr;
	taverntxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/longhousetavernsprite.png");
	sf::Sprite tavern(taverntxtr);
	tavern.setPosition(tavernPosition);

	//TavernTop
	sf::Texture taverntoptxtr;
	taverntoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/taverntop.png");
	sf::Sprite taverntop(taverntoptxtr);
	taverntop.setPosition(tavernTopPosition);

	//whousetop
	sf::Texture whousetoptxtr;
	whousetoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/whousetop.png");
	sf::Sprite whousetop(whousetoptxtr);
	whousetop.setPosition(whouseTopPosition);

	//wtowertop
	sf::Texture wtowertoptxtr;
	wtowertoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/wtowertop.png");
	sf::Sprite wtowertop(wtowertoptxtr);
	wtowertop.setPosition(wtowerTopPosition);

	//bhousetop
	sf::Texture bhousetoptxtr;
	bhousetoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/bhousetop.png");
	sf::Sprite bhousetop(bhousetoptxtr);
	bhousetop.setPosition(bhouseTopPosition);

	//Vectors
	Vector2f playerCenter;
	Vector2f playerRight;
	Vector2f playerLeft;
	Vector2f enemyxCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	Vector2f loc;

	while (window.isOpen())
	{
		Event event;
		Polygon playerOriginal = collisionx;
		Polygon attackxOriginal = attackx;
		
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
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;

				if (rectSourceSprite.left == 0 && round == true){
					attacking = true;
					
				}
					
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 2)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 127;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 3)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 254;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 4)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 381;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 5)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 508;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 6)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 635;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 7)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 762;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 8)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 889;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
	    
        }

		if (barrelhealth <= 0 && clock.getElapsedTime().asSeconds() > 0.1)
		{
			
			barrelxSourceSprite.left += 100;
			if (barrelxSourceSprite.left >= 400){
				barrelxSourceSprite.left = 400;
			}
			barrel.setTextureRect(barrelxSourceSprite);
			clock.restart();
		}

		//Vectors
		playerCenter = Vector2f(player.getPosition());
		playerRight = Vector2f(player.getPosition());
		playerLeft = Vector2f(player.getPosition());
		
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			
			std::cout << "Position: (" << playerCenter.x << ", " << playerCenter.y << ")" << std::endl;
		}

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
		const auto prevPos = player.getPosition();

		// Move player with arrow keys
        if (Keyboard::isKeyPressed(Keyboard::A)) {
			player.move(-playerSpeed, 0.f);
            for (auto& vertex : collisionx.vertices) {
                vertex.x -= 5; // Move left
            }
			for (auto& vertex : attackx.vertices) {
                vertex.x -= 5; // Move left
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
			player.move(playerSpeed, 0.f);
            for (auto& vertex : collisionx.vertices) {
                vertex.x += 5; // Move right
            }
			for (auto& vertex : attackx.vertices) {
                vertex.x += 5; // Move right
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::W)) {
			player.move(0.f, -playerSpeed);
            for (auto& vertex : collisionx.vertices) {
                vertex.y -= 5; // Move up
            }
			for (auto& vertex : attackx.vertices) {
                vertex.y -= 5; // Move up
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
			player.move(0.f, playerSpeed);
            for (auto& vertex : collisionx.vertices) {
                vertex.y += 5; // Move down
            }
			for (auto& vertex : attackx.vertices) {
                vertex.y += 5; // Move down
            }
        }
		else{
			idle = 1;
		}

		
		
		// Check for collision
        Vectorx2f collisionNormal;

		if (checkPolygonCollision(fboundry1, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry2, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry3, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry4, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry5, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry6, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry7, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry8, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry9, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry10, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry11, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry12, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry13, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry14, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry15, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb1, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb2, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb3, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb4, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier5, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier6, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier7, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier8, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier9, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier10, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier11, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier12, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier13, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier14, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier15, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier16, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier17, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier18, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier19, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier20, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier21, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier22, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		

		if (checkPolygonCollision(fjelbarrier24, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier25, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier26, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier27, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier28, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier29, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier30, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier31, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier32, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier33, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier34, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier35, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier36, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier37, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier38, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier39, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier40, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier41, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier42, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier43, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier44, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier45, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier46, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier47, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier48, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier49, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier50, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier51, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier52, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier53, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier54, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier55, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier56, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier57, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier58, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier59, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier60, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier61, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier62, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier63, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier64, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier65, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier66, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier67, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier68, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier69, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier70, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier71, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier72, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier73, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier74, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier75, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier76, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier77, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier78, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier79, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier80, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier81, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier82, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelbarrier83, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		//wilderness1 collision

		if (checkPolygonCollision(w1barrier1, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier2, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier3, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier4, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier5, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier6, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier7, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier8, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier9, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier10, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier11, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier12, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier13, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier14, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier15, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier16, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier17, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier18, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier19, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier20, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier21, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier22, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier23, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier24, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier25, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier26, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier27, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier28, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier29, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier30, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier31, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier32, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier33, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier34, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier35, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }



		

		

		

		if (checkPolygonCollision(barrelx, attackx, collisionNormal) && attacking == true && round == true && area == 1) {
            // If an attack is detected, deal damage to the barrel
			area = 2;
            barrelhealth -= 1;
			std::cout << "The value of x is: " << barrelhealth << std::endl;
			std::cout << "The value of z is: " << barrelxSourceSprite.left << std::endl;
			round = false;
        }
		
		// Draw the player polygon
        sf::ConvexShape playerShape;
        playerShape.setPointCount(collisionx.vertices.size());
        for (size_t i = 0; i < collisionx.vertices.size(); ++i) {
            playerShape.setPoint(i, sf::Vector2f(collisionx.vertices[i].x, collisionx.vertices[i].y));
        }
		
        playerShape.setFillColor(sf::Color::Green);

		// Draw the boundry polygon
        sf::ConvexShape boundry1Shape;
        boundry1Shape.setPointCount(fboundry1.vertices.size());
        for (size_t i = 0; i < fboundry1.vertices.size(); ++i) {
            boundry1Shape.setPoint(i, sf::Vector2f(fboundry1.vertices[i].x, fboundry1.vertices[i].y));
        }

		boundry1Shape.setFillColor(sf::Color::Blue);

		sf::ConvexShape boundry2Shape;
        boundry2Shape.setPointCount(fboundry2.vertices.size());
        for (size_t i = 0; i < fboundry2.vertices.size(); ++i) {
            boundry2Shape.setPoint(i, sf::Vector2f(fboundry2.vertices[i].x, fboundry2.vertices[i].y));
        }
		
		boundry2Shape.setFillColor(sf::Color::Blue);

		sf::ConvexShape boundry4Shape;
        boundry4Shape.setPointCount(fboundry4.vertices.size());
        for (size_t i = 0; i < fboundry4.vertices.size(); ++i) {
            boundry4Shape.setPoint(i, sf::Vector2f(fboundry4.vertices[i].x, fboundry4.vertices[i].y));
        }
		
		boundry4Shape.setFillColor(sf::Color::Blue);
		
		// Draw the attack range polygon
		sf::ConvexShape attackrangeShape;
        attackrangeShape.setPointCount(attackx.vertices.size());
        for (size_t i = 0; i < attackx.vertices.size(); ++i) {
            attackrangeShape.setPoint(i, sf::Vector2f(attackx.vertices[i].x, attackx.vertices[i].y));
        }
		
        attackrangeShape.setFillColor(sf::Color::Blue);

		//Draw
        sf::View view;
		view.setCenter(player.getPosition());
		
		// Draw the building polygon
        

		// Draw the barrelx polygon
		sf::ConvexShape barrelxShape;
		barrelxShape.setPointCount(barrelx.vertices.size());
		for (size_t i = 0; i < barrelx.vertices.size(); ++i) {
			barrelxShape.setPoint(i, sf::Vector2f(barrelx.vertices[i].x, barrelx.vertices[i].y));
		}
		barrelxShape.setFillColor(sf::Color::Red);
		
		window.clear();
		
		window.setView(view);
		
		if (area == 1){
        window.draw(background);

		window.draw(boundry1Shape);
		

		window.draw(barrel);

		window.draw(player);
		window.draw(fhouse1x);
		window.draw(overhang);
		window.draw(boundry4Shape);
		

		}

		if (area == 2){
        window.draw(fjelenvar);

		window.draw(player);
		}

		if (area == 3){
			window.draw(wilderness1);
			window.draw(player);
		}

		window.display();
		

	}

	return 0;
}
