#include <String>
#include <vector>
#include "Color.hpp"

/**
 * @brief A class to map colors to the closest available color
 */
class ColorMap {
  public:
    ColorMap();
    ColorMap(std::vector<std::string> colors);
    std::vector<std::string> getColors() const;
    std::string getColor(int index) const;
    void addColor(std::string color);
    void removeColor(int index);
    void clear();
    std::string getClosestColor(const std::string &color) const;
  private:
    std::vector<std::string> colors;
};

/**
 * @brief Default constructor
 * Initializes the color map with no colors.
 */
inline ColorMap::ColorMap() {
  colors = {};
}
/**
 * @brief Constructor with a list of colors
 * Initializes the color map with the given list of colors.
 * @param colors The list of colors
 */
inline ColorMap::ColorMap(const std::vector<std::string> colors) {
  this->colors = colors;
}

/**
 * @brief Get the list of colors
 * @return The list of colors
 */
inline std::vector<std::string> ColorMap::getColors() const {
  return colors;
}

/**
 * @brief Get a color by index
 * @param index The index of the color
 * @return The color
 */
inline  std::string ColorMap::getColor(const int index) const {
  return colors.at(index);
}

/**
 * @brief Add a color to the list
 * @param color The color to add
 */
inline void ColorMap::addColor(const std::string color) {
  colors.emplace_back(color);
}

/**
 * @brief Remove a color from the list
 * @param index The index of the color to remove
 */
inline void ColorMap::removeColor(const int index) {
  colors.erase(colors.begin() + index);
}

/**
 * @brief Clear the list of colors
 */
inline void ColorMap::clear() {
  colors.clear();
}

/**
 * @brief Get the closest color to the given color
 * @param color The color to compare
 * @return The closest color
 */
inline std::string ColorMap::getClosestColor(const std::string &color) const {
  return "";
}





