#include <string>
#include <vector>
#include "../header/Color.hpp"
#include "../header/ColorMap.hpp"
#include <stdexcept>

/**
 * @brief Default constructor
 * Initializes the color map with no colors.
 */
ColorMap::ColorMap() {
  colors = {};
}
/**
 * @brief Constructor with a list of colors
 * Initializes the color map with the given list of colors.
 * @param colors The list of colors
 */
ColorMap::ColorMap(const std::vector<std::string>& colors) {
  this -> colors = {};
  for (const auto &color : colors) {
    const Color c(color);
    addColor(c);
  }
}

/**
 * @brief Constructor with a list of colors
 * Initializes the color map with the given list of colors.
 * @param colors The list of colors
 */
ColorMap::ColorMap(const std::vector<Color>& colors) {
  this -> colors = colors;
}

/**
 * @brief Get the list of colors
 * @return The list of colors
 */
const std::vector<Color>& ColorMap::getColors() const {
  return colors;
}

/**
 * @brief Get a color by index
 * @param index The index of the color
 * @return The color
 */
 Color ColorMap::getColor(const int index) const {
  return colors.at(index);
}

/**
 * @brief Add a color to the list
 * @param color The color to add
 */
void ColorMap::addColor(const Color& color) {
  colors.emplace_back(color);
}
/**
 * @brief Add a color to the list
 * @param color The color to add
 */
void ColorMap::addColor(const std::string& color) {
  const Color c(color);
  addColor(c);
}

/**
 * @brief Remove a color from the list
 * @param index The index of the color to remove
 * @throws invalid_argument If the index is out of bounds
 */
void ColorMap::removeColor(const int index) {
  if (index < 0 || index >= colors.size()) {
    throw std::invalid_argument("Index out of bounds");
  }
  colors.erase(colors.begin() + index);
}

/**
 * @brief Remove a color from the list
 * @param color The color to remove
 * @throws invalid_argument If the color is not found
 */
void ColorMap::removeColor(const Color &color) {
  for (int i = 0; i < colors.size(); i++) {
    if (colors.at(i).getHex() == color.getHex()) {
      removeColor(i);
      return;
    }
  }
  throw std::invalid_argument("Color not found");
}

/**
 * @brief Remove a color from the list
 * @param color The color to remove
 * @throws invalid_argument If the color is not found
 */
void ColorMap::removeColor(const std::string &color) {
  Color c(color);
  removeColor(c);
}



/**
 * @brief Clear the list of colors
 */
void ColorMap::clear() {
  colors.clear();
}

/**
 * @brief Get the closest color to the given color
 * @param color The color to compare
 * @param hsl if the method should use hsl or rgb distance method
 * @return The closest color
 */
Color ColorMap::getClosestColor(const Color& color, bool hsl) const {
  int minDistance = 1000000;
  Color col = colors.at(0);
  for (const auto &c : colors) {
    if (c.getHex() == color.getHex()) {
      return c;
    }
    int distance = 1000000;
    if (hsl) distance = c.getHslDistance(color);
    else distance = c.getDistance(color);
    if (distance < minDistance) {
      minDistance = distance;
      col = c;
    }
    if (distance == minDistance) {
      if (c.getHex() < col.getHex()) {
        col = c;
      }
    }
  }
  return col;
}

/**
 * @brief Get the closest color to the given color
 * @param color The color to compare
 * @param hsl if the method should use hsl or rgb distance method
 * @return The closest color
 */
Color ColorMap::getClosestColor(const std::string &color, bool hsl) const {
  Color c(color);
  return getClosestColor(c, hsl);
}
/**
 * @brief Get the closest color to the given color
 * @param color The color to compare
 * @return The closest color
 */
Color ColorMap::getClosestColor(const std::string &color) const {
  Color c(color);
  return getClosestColor(c);
}

/**
 * @brief Get the closest color to the given color
 * @param color The color to compare
 * @return The closest color
 */
Color ColorMap::getClosestColor(const Color& color) const {
  return getClosestColor(color, false);
}






