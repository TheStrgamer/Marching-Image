#pragma once
#include <string>

/**
 * @brief Convert a number to a hexadecimal string
 * Takes a number between 0 and 255 and returns a string representing the hexadecimal value of the number.
 * Numbers bellow 0 are converted to 0 and numbers above 255 are converted to 255.
 * @param num The number to convert
 * @return The hexadecimal string
 */
std::string toHex(int num);

/**
 * @brief Convert a hexadecimal string to a number
 * @param hex The hexadecimal string to convert
 * @return The number
 */
int toNum(const std::string &hex);

/**
 * @brief Set a number within a range
 * @param num The number to set
 * @param min The minimum value
 * @param max The maximum value
 * @return The number within the range
 */
int setInLimits(int num, int min, int max);

/**
 * @brief A class to represent a color
 * A class to represent a color with red, green, and blue values and a hexadecimal string.
 */
class Color {
  public:
  Color();
  Color(int red, int green, int blue);
  explicit Color(const std::string& hex);
  int getRed() const;
  int getGreen() const;
  int getBlue() const;
  std::string getHex() const;
  void setRed(int red);
  void setGreen(int green);
  void setBlue(int blue);
  void addColor(const Color &color);
  void addColor(const std::string &color);
  void subtractColor(const Color &color);
  void subtractColor(const std::string &color);
  int getDistance(const Color &color) const;
  int getDistance(const std::string &color) const;

  int operator==(const Color &color) const {
    return red == color.red && green == color.green && blue == color.blue;
  }
  int operator!=(const Color &color) const {
    return red != color.red || green != color.green || blue != color.blue;
  }
  std::string operator<<(const Color &color) const {
    return color.getHex();
  }
  void operator+=(const Color &color) {
    addColor(color);
  }
  void operator-=(const Color &color) {
    subtractColor(color);
  }

  private:
  int red;
  int green;
  int blue;
  std::string hex;
};
