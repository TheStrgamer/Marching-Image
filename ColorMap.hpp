#include <String>
#include <vector>

std::string toHex(int num) {
  if (num < 0) {
    return "00";
  }
  if (num > 255) {
    return "FF";
  }
  std::string hex = "0123456789ABCDEF";
  return std::string(1, hex[num/16]) + std::string(1, hex[num%16]);
}

int toNum(const std::string &hex) {
  std::string hexChars = "0123456789ABCDEF";
  int num = 0;
  for (int i = 0; i < hex.length(); i++) {
    num = num * 16 + hexChars.find(hex[i]);
  }
  return num;
}

class Color {
  public:
    Color();
    Color(const int red, const int green, const int blue);
    Color(const std::string hex);
    int getRed() const;
    int getGreen() const;
    int getBlue() const;
    std::string getHex() const;
    void setRed(const int red);
    void setGreen(const int green);
    void setBlue(const int blue);

  private:
    int red;
    int green;
    int blue;
    std::string hex;
};

inline Color::Color() {
  red = 0;
  green = 0;
  blue = 0;
  hex = "#000000";
}
inline Color::Color(const int red, const int green, const int blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}
inline Color::Color(const std::string hex) {
  this->hex = hex;
  red = toNum(hex.substr(1, 2));
  green = toNum(hex.substr(3, 2));
  blue = toNum(hex.substr(5, 2));
}
inline int Color::getRed() const {
  return red;
}
inline int Color::getGreen() const {
  return green;
}
inline int Color::getBlue() const {
  return blue;
}
inline std::string Color::getHex() const {
  return hex;
}
inline void Color::setRed(const int red) {
  this->red = red;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}
inline void Color::setGreen(const int green) {
  this->green = green;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}
inline void Color::setBlue(const int blue) {
  this->blue = blue;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}



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





