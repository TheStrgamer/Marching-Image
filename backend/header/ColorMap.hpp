#pragma once
#include <string>
#include <vector>
#include "Color.hpp"

/**
 * @brief A class to map colors to the closest available color
 */
class ColorMap {
  public:
    ColorMap();
    explicit ColorMap(const std::vector<Color>& colors);
    explicit ColorMap(const std::vector<std::string>& colors);
    void addColor(const Color&);
    void addColor(const std::string& color);
    const std::vector<Color>& getColors() const;
    Color getColor(int index) const;
    void removeColor(const Color& color);
    void removeColor(const std::string& color);
    void removeColor(int index);
    void clear();
    Color getClosestColor(const Color &color) const;
    Color getClosestColor(const std::string &color) const;
    Color getClosestColor(const Color &color, bool hsl) const;
    Color getClosestColor(const std::string &color, bool hsl) const;


  private:
    std::vector<Color> colors;
};
