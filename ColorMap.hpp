#include <String>
#include <vector>
#include "Color.hpp"

/**
 * @brief A class to map colors to the closest available color
 */
class ColorMap {
  public:
    ColorMap();
    ColorMap(std::vector<Color> colors);
    ColorMap(std::vector<std::string> colors);
    void addColor(Color);
    void addColor(const std::string& color);
    std::vector<Color> getColors() const;
    Color getColor(int index) const;
    void removeColor(const Color& color);
    void removeColor(const std::string& color);
    void removeColor(int index);
    void clear();
    Color getClosestColor(const Color &color) const;
    Color getClosestColor(const std::string &color) const;
  private:
    std::vector<Color> colors;
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
inline ColorMap::ColorMap(const std::vector<Color> colors) {
  this -> colors = colors;
}

/**
 * @brief Get the list of colors
 * @return The list of colors
 */
inline std::vector<Color> ColorMap::getColors() const {
  return colors;
}

/**
 * @brief Get a color by index
 * @param index The index of the color
 * @return The color
 */
inline  Color ColorMap::getColor(const int index) const {
  return colors.at(index);
}

/**
 * @brief Add a color to the list
 * @param color The color to add
 */
inline void ColorMap::addColor(Color color) {
  colors.emplace_back(color);
}
/**
 * @brief Add a color to the list
 * @param color The color to add
 */
inline void ColorMap::addColor(const std::string& color) {
  const Color c(color);
  addColor(c);
}

/**
 * @brief Remove a color from the list
 * @param index The index of the color to remove
 * @throws invalid_argument If the index is out of bounds
 */
inline void ColorMap::removeColor(const int index) {
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
inline void ColorMap::removeColor(const Color &color) {
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
inline void ColorMap::removeColor(const std::string &color) {
  Color c(color);
  removeColor(c);
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
inline Color ColorMap::getClosestColor(const Color& color) const {
  return {};
}

/**
 * @brief Get the closest color to the given color
 * @param color The color to compare
 * @return The closest color
 */
inline Color ColorMap::getClosestColor(const std::string &color) const {
  Color c(color);
  return getClosestColor(c);
}





