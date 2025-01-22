#include <String>
#include <vector>
#include "Color.hpp"

class ColorMap {
  public:
    ColorMap();
    std::vector<std::string> getColors() const;
    std::string getColor(const int index) const;
    void addColor(const std::string color);
    void removeColor(const int index);
    void clear();
    std::string getClosestColor(const std::string &color) const;
  private:
    std::vector<std::string> colors;
};

inline ColorMap::ColorMap() {
  colors = {};
}
inline std::vector<std::string> ColorMap::getColors() const {
  return colors;
}
inline  std::string ColorMap::getColor(const int index) const {
  return colors.at(index);
}
inline void ColorMap::addColor(const std::string color) {
  colors.emplace_back(color);
}
inline void ColorMap::removeColor(const int index) {
  colors.erase(colors.begin() + index);
}
inline void ColorMap::clear() {
  colors.clear();
}
inline std::string ColorMap::getClosestColor(const std::string &color) const {
  return "";
}





