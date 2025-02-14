#include <iostream>
#include <string>
#include <vector>
#include "ColorMap.hpp"
#include "Color.hpp"
#include "ImageHandler.hpp"

using namespace std;
int main() {
  Color red = Color(255, 0, 0);
  Color green = Color(0, 255, 0);
  Color blue = Color(0, 0, 255);
  Color black = Color(0, 0, 0);
  Color white = Color(255, 255, 255);
  Color yellow = Color(255, 255, 0);
  Color cyan = Color(0, 255, 255);
  Color magenta = Color(255, 0, 255);

  vector<Color> colors = {red, green, blue, black, white, yellow, cyan, magenta};
  vector<string> names = {"red", "green", "blue", "black", "white", "yellow", "cyan", "magenta"};
  ColorMap colorMap1 = ColorMap(colors);
  vector<Color> colors2 = {Color(255, 55, 13), Color(124, 255, 0), Color(12, 90, 55)};
  for (const auto &color : colors2) {
    cout << color.getHex() << " is closest to " << colorMap1.getClosestColor(color).getHex() << endl;
  }

  string inputFolder = "test_images/";
  string outputFolder = "/app/output/";

  return 0;

}