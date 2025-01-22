#pragma once
#include <string>

/**
 * @brief Convert a number to a hexadecimal string
 * Takes a number between 0 and 255 and returns a string representing the hexadecimal value of the number.
 * Numbers bellow 0 are converted to 0 and numbers above 255 are converted to 255.
 * @param num The number to convert
 * @return The hexadecimal string
 */
inline std::string toHex(int num) {
  if (num < 0) {
    return "00";
  }
  if (num > 255) {
    return "FF";
  }
  std::string hex = "0123456789ABCDEF";
  return std::string(1, hex[num/16]) + std::string(1, hex[num%16]);
}

/**
 * @brief Convert a hexadecimal string to a number
 * @param hex The hexadecimal string to convert
 * @return The number
 */
inline int toNum(const std::string &hex) {
  std::string hexChars = "0123456789ABCDEF";
  int num = 0;
  for (int i = 0; i < hex.length(); i++) {
    num = num * 16 + hexChars.find(hex[i]);
  }
  return num;
}

/**
 * @brief A class to represent a color
 * A class to represent a color with red, green, and blue values and a hexadecimal string.
 */
class Color {
  public:
  Color();
  Color(int red, int green, int blue);
  Color(std::string hex);
  int getRed() const;
  int getGreen() const;
  int getBlue() const;
  std::string getHex() const;
  void setRed(int red);
  void setGreen(int green);
  void setBlue(int blue);

  private:
  int red;
  int green;
  int blue;
  std::string hex;
};

/**
 * @brief Default constructor
 * Initializes the color with black.
 */
inline Color::Color() {
  red = 0;
  green = 0;
  blue = 0;
  hex = "#000000";
}
/**
 * @brief Constructor with red, green, and blue values
 * Initializes the color with the given red, green, and blue values.
 * Automatically sets the hexadecimal string.
 * @param red The red value
 * @param green The green value
 * @param blue The blue value
 */
inline Color::Color(const int red, const int green, const int blue) {
  this->red = red;
  this->green = green;
  this->blue = blue;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}

/**
 * @brief Constructor with a hexadecimal string
 * Initializes the color with the given hexadecimal string.
 * Automatically sets the red, green, and blue values.
 * @param hex The hexadecimal string
 */
inline Color::Color(const std::string hex) {
  this->hex = hex;
  red = toNum(hex.substr(1, 2));
  green = toNum(hex.substr(3, 2));
  blue = toNum(hex.substr(5, 2));
}
/**
 * @brief Get the red value
 * @return The red value
 */
inline int Color::getRed() const {
  return red;
}
/**
 * @brief Get the green value
 * @return The green value
 */
inline int Color::getGreen() const {
  return green;
}
/**
 * @brief Get the blue value
 * @return The blue value
 */
inline int Color::getBlue() const {
  return blue;
}
/**
 * @brief Get the hexadecimal string
 * @return The hexadecimal string
 */
inline std::string Color::getHex() const {
  return hex;
}
/**
 * @brief Set the red value
 * @param red The red value
 */
inline void Color::setRed(const int red) {
  this->red = red;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}
/**
 * @brief Set the green value
 * @param green The green value
 */
inline void Color::setGreen(const int green) {
  this->green = green;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}
/**
 * @brief Set the blue value
 * @param blue The blue value
 */
inline void Color::setBlue(const int blue) {
  this->blue = blue;
  hex = "#" + toHex(red) + toHex(green) + toHex(blue);
}

