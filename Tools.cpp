#include "Tools.h"

namespace FW
{
    namespace Tools
    {
        namespace Math
        {
            static const double PI = 3.14159265359;
            static const double DEGREE_RADIAN_RATIO = 0.0174533;

            int angleBetweenPoints(const int x1, const int y1, const int x2, const int y2)
            {
                return 180 + static_cast<int>(((std::atan2(y1 - y2, x1 - x2) * 180.0000) / PI));
            }

            int angleBetweenPoints(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
            {
                return angleBetweenPoints(vec1.x, vec1.y, vec2.x, vec2.y);
            }

            // convert degrees to radians using the ratio between 1 radian and 1 degree
            double deg2rad(float deg)
            {
                return deg * DEGREE_RADIAN_RATIO;
            }

            float distance(const sf::Vector2f& v1, const sf::Vector2f& v2)
            {
              return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2));
            }
        }

        std::vector<std::string> splitString(const std::string& string, const char delimeter)
        {
            std::vector<std::string> items;
            std::stringstream ss(string);
            std::string item;

            while (std::getline(ss, item, delimeter))
                items.push_back(item);

            return items;
        }
    }
}