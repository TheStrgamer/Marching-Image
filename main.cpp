#include <iostream>
#include <string>
#include <vector>
#include "header/ColorMap.hpp"
#include "header/Color.hpp"
#include "header/ImageHandler.hpp"


using namespace std;
void mapImageAndSave(const string &inputName, ColorMap &colorMap, ImageHandler &imageHandler) {
  string inputFolder = "test_images/";
  string outputFolder = "/app/output/";

  imageHandler.readImage(inputFolder+ inputName);
  imageHandler.mapImage(colorMap);
  imageHandler.saveImage(outputFolder + inputName);
  for (int i = 1; i < 6; i++) {
    imageHandler.blurImage(i*3+1);
    imageHandler.mapImage(colorMap);
    imageHandler.saveImage(outputFolder + "blurred_" + to_string(i) + "_" + inputName);
    cout << "Saved " << outputFolder + "blurred_" + to_string(i) + "_" + inputName << endl;
  }
}

int main() {
  Color red = Color(255, 0, 0);
  Color green = Color(0, 255, 0);
  Color blue = Color(0, 0, 255);
  Color black = Color(0, 0, 0);
  Color white = Color(255, 255, 255);
  Color yellow = Color(255, 255, 0);
  Color cyan = Color(0, 255, 255);
  Color magenta = Color(255, 0, 255);
  Color gray = Color(128, 128, 128);
  Color orange = Color(255, 165, 0);

  vector<Color> colors = {orange, red, yellow, blue, white, black, green};
  ColorMap colorMap1 = ColorMap(colors);

  ImageHandler imageHandler = ImageHandler();

  mapImageAndSave("rickroll.jpg", colorMap1, imageHandler);


  return 0;

}