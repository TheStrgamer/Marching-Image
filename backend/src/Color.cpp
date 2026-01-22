#include "../header/Color.hpp"
#include <iostream>
#include <string>
#include <cmath>

/**
 * @brief Convert a number to a hexadecimal string
 * Takes a number between 0 and 255 and returns a string representing the hexadecimal value of the number.
 * Numbers bellow 0 are converted to 0 and numbers above 255 are converted to 255.
 * @param num The number to convert
 * @return The hexadecimal string
 */
std::string toHex(int num)
{
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
int toNum(const std::string &hex) {
    int num = 0;
    for (char c : hex) {
        char cUpper = std::toupper(static_cast<unsigned char>(c));
        int val = -1;

        if (cUpper >= '0' && cUpper <= '9') {
            val = cUpper - '0';
        } else if (cUpper >= 'A' && cUpper <= 'F') {
            val = cUpper - 'A' + 10;
        } else {
            throw std::invalid_argument("Invalid hex character");
        }

        num = num * 16 + val;
    }
    return num;
}

/**
 * @brief Set a number within a range
 * @param num The number to set
 * @param min The minimum value
 * @param max The maximum value
 * @return The number within the range
 */
int setInLimits(int num, int min, int max) {
    if (num < min) {
      return min;
    }
    if (num > max) {
      return max;
    }
    return num;
  }

  
/**
 * @brief Default constructor
 * Initializes the color with black.
 */
Color::Color() {
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
  Color::Color(const int red, const int green, const int blue) {
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
  Color::Color(const std::string& hex) {
    std::cout << "Creating color from hex: " << hex << std::endl;
    if (hex.length() != 7 || hex[0] != '#') {
      throw std::invalid_argument("Invalid hex color format. Expected format: #RRGGBB");
    }
    this->hex = hex;
    red = toNum(hex.substr(1, 2));
    green = toNum(hex.substr(3, 2));
    blue = toNum(hex.substr(5, 2));
    std::cout << "Color created with RGB: (" << red << ", " << green << ", " << blue << ") and hex: " << this->hex << std::endl;
  }
  /**
   * @brief Get the red value
   * @return The red value
   */
  int Color::getRed() const {
    return red;
  }
  /**
   * @brief Get the green value
   * @return The green value
   */
  int Color::getGreen() const {
    return green;
  }
  /**
   * @brief Get the blue value
   * @return The blue value
   */
  int Color::getBlue() const {
    return blue;
  }
  /**
   * @brief Get the hexadecimal string
   * @return The hexadecimal string
   */
  std::string Color::getHex() const {
    return hex;
  }
  /**
   * @brief Set the red value
   * @param red The red value
   */
  void Color::setRed(const int red) {
    this->red = setInLimits(red, 0, 255);
    hex = "#" + toHex(red) + toHex(green) + toHex(blue);
  }
  /**
   * @brief Set the green value
   * @param green The green value
   */
  void Color::setGreen(const int green) {
    this->green = setInLimits(green, 0, 255);
    hex = "#" + toHex(red) + toHex(green) + toHex(blue);
  }
  /**
   * @brief Set the blue value
   * @param blue The blue value
   */
  void Color::setBlue(const int blue) {
    this->blue = setInLimits(blue, 0, 255);
    hex = "#" + toHex(red) + toHex(green) + toHex(blue);
  }
  
  /**
   * @brief Add a color to the current color
   * Adds the given color to the current color.
   * @param color The color to add
   */
  void Color::addColor(const Color &color) {
    setRed(red + color.red);
    setGreen(green + color.green);
    setBlue(blue + color.blue);
  }
  
  /**
   * @brief Add a color to the current color
   * Adds the given color to the current color.
   * @param color The color to add
   */
  void Color::addColor(const std::string &color) {
    const Color c(color);
    addColor(c);
  }
  
  /**
   * @brief Subtract a color from the current color
   * Subtracts the given color from the current color.
   * @param color The color to subtract
   */
  void Color::subtractColor(const Color &color) {
    setRed(red - color.red);
    setGreen(green - color.green);
    setBlue(blue - color.blue);
  }
  
  /**
   * @brief Subtract a color from the current color
   * Subtracts the given color from the current color.
   * @param color The color to subtract
   */
  void Color::subtractColor(const std::string &color) {
    const Color c(color);
    subtractColor(c);
  }
  
  /**
   * @brief Get the distance to another color
   * Calculates the distance to another color.
   * @param color The other color
   * @return The distance
   */
  int Color::getDistance(const Color &color) const {
    return std::sqrt(std::pow(red - color.red, 2) + std::pow(green - color.green, 2) + std::pow(blue - color.blue, 2));
    //return std::sqrt(std::pow(red - color.red, 2) + std::pow(green - color.green, 2) + std::pow(blue - color.blue, 2));
  }

  /**
   * @brief Get the distance to another color
   * Calculates the distance to another color.
   * @param color The other color
   * @return The distance
   */
  int Color::getDistance(const std::string &color) const {
    Color c(color);
    return getDistance(c);
  }
      
  /**
   * @brief Returns the Hsl representation of the rgb color
   * @return the HSL color
   */
  HSL Color::rgbToHsl() const {
      double rd = red / 255.0;
      double gd = green / 255.0;
      double bd = blue / 255.0;

      double max = std::max({rd, gd, bd});
      double min = std::min({rd, gd, bd});
      double delta = max - min;

      HSL hsl;
      hsl.l = (max + min) / 2.0;

      if (delta == 0.0) {
          hsl.h = 0.0;
          hsl.s = 0.0;
      } else {
          hsl.s = delta / (1.0 - std::abs(2.0 * hsl.l - 1.0));

          if (max == rd)
              hsl.h = 60.0 * std::fmod(((gd - bd) / delta), 6.0);
          else if (max == gd)
              hsl.h = 60.0 * (((bd - rd) / delta) + 2.0);
          else
              hsl.h = 60.0 * (((rd - gd) / delta) + 4.0);

          if (hsl.h < 0.0)
              hsl.h += 360.0;
      }

      return hsl;
  }

  /**
   * @brief Function that calculates color closeness in hsl to be more accurate to how it looks
   * @param color the color to compare to this color
   * @return the distance
   */
/**
 * @brief Function that calculates color closeness in hsl to be more accurate to how it looks
 * @param color the color to compare to this color
 * @return the distance (scaled to be comparable with RGB distance)
 */
int Color::getHslDistance(const Color &color) const {
    const HSL& a = rgbToHsl();
    const HSL& b = color.rgbToHsl();
    
    double dh = std::abs(a.h - b.h);
    dh = std::min(dh, 360.0 - dh) / 180.0;

    double ds = std::abs(a.s - b.s);
    double dl = std::abs(a.l - b.l);

    double distance = std::sqrt(
        4.0 * dh * dh +
        1.5 * ds * ds +
        1.0 * dl * dl
    );
    return static_cast<int>(distance * 100);
}

    /**
   * @brief Function that calculates color closeness in hsl to be more accurate to how it looks
   * @param color the string color to compare to this color
   * @return the distance
   */
  int Color::getHslDistance(const std::string &color) const{
    Color c(color);
    return getHslDistance(c);
  }
