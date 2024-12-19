#include <cmath>
#include <SFML/Graphics.hpp>

const int   numberOfRays = 360;                                                                             // Number of rays from the center
const int   windowWidth  = 1000;                                                                            // Width of the window
const int   windowHeight = 800;                                                                            // Height of the window
const float length       = 10000.0f;
const std::vector<float> wallArray = {  100.0f,  100.0f, 400.0f,  400.0f,                                     // each line consists of starting and ending cordinates
                                        600.0f,  200.0f, 800.0f,  700.0f,                                     // Currently holding 4 lines
                                        400.0f,  50.0f,  800.0f,  50.0f,
                                        300.0f, 500.0f, 800.0f, 500.0f 
                                     };

// To draw the wall (line) between two points
void drawWalls(sf::RenderWindow& window, int x, int y, int a, int b) {
    sf::VertexArray wall(sf::Lines, 2);
    wall[0].position = sf::Vector2f(x, y);
    wall[1].position = sf::Vector2f(a, b);
    wall[0].color = sf::Color::White;
    wall[1].color = sf::Color::White;

    window.draw(wall);
}

// To calculate the intersection of two lines (from coordinates)
sf::Vector2f intersection(sf::Vector2f origin, sf::Vector2f rayLastCor, sf::Vector2f wallFirstCor, sf::Vector2f wallSecondCor) {
    float x1 = origin.x;
    float y1 = origin.y;
    float x2 = rayLastCor.x;
    float y2 = rayLastCor.y;

    float x3 = wallFirstCor.x;
    float y3 = wallFirstCor.y;
    float x4 = wallSecondCor.x;
    float y4 = wallSecondCor.y;

    float denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

    // Check if the lines are parallel (denominator is close to 0)
    if (std::abs(denominator) < 1e-5) {
        // Return the same coordinates of the ray (no intersection)
        return rayLastCor;
    }

    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / denominator;

    // Check if the intersection point lies on both line segments
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        return sf::Vector2f(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
    }

    // Return default coordinates if no intersection
    return rayLastCor;
}

// To draw a ray (line) from source (center) to infinity at some angle
void drawRay(sf::RenderWindow& window, const sf::Vector2f& origin) {
    float radianAngle = 0;
    for (float angleDegrees=1; angleDegrees<=numberOfRays ; angleDegrees += 1){ 
        
        // Set the starting point of the ray
        sf::VertexArray ray(sf::Lines, 2);

        ray[0].position = origin;
        ray[0].color = sf::Color(255, 255, 255);

        // Set the end point of the ray (direction)
        radianAngle = angleDegrees * (3.14159265f / 180.0f);                              // Convert angle to radians

        ray[1].position = sf::Vector2f(origin.x + length * std::cos(radianAngle),
                                    origin.y + length * std::sin(radianAngle));
        ray[1].color = sf::Color(255, 255, 255);
   
        for (int i=0; i <=wallArray.size() - 4; i += 4) {
            sf::Vector2f intersectionCordinates = intersection( origin, ray[1].position,
                                                                {wallArray[i], wallArray[i+1]}, {wallArray[i+2], wallArray[i+3]});
            
            if (intersectionCordinates.x != -1 && intersectionCordinates.y != -1) {
                // we need to only draw the line till intersection cordinates
                ray[1].position = intersectionCordinates;
            } 
        }
        window.draw(ray);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "2D RayCasting SFML C++");
    window.setFramerateLimit(144);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            // if Window is resized
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

        }

        window.clear(sf::Color(14, 14, 14));

        // Get the mouse position
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionFloat(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
    
        // Draw the walls
        for (int i=0; i <=wallArray.size() - 4; i += 4) {
            drawWalls(window, wallArray[i],  wallArray[i+1], wallArray[i+2], wallArray[i+3]);                       // if you want to add new lines just update the global wallArray vector above
        }

        // Call the drawRay function to draw the ray from mouse position to infinity
        drawRay(window, mousePositionFloat); 
        
        window.display();
    }

    return 0;
}
