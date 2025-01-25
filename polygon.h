#ifndef POLYGON_H
#define POLYGON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

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

#endif // POLYGON_H