#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace FW
{
    namespace Tools
    {
        namespace Math
        {
            int angleBetweenPoints(const int x1, const int y1, const int x2, const int y2);
            int angleBetweenPoints(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
            double deg2rad(float deg);
            float distance(const sf::Vector2f& v1, const sf::Vector2f& v2);
        }

        std::vector<std::string> splitString(const std::string& string, const char delimeter);
    }
}