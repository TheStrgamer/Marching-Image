#include <iostream>
#include <string>
#include <vector>
#include "header/ColorMap.hpp"
#include "header/Color.hpp"
#include "header/ImageHandler.hpp"
#include "header/Server.hpp"


using namespace std;
void mapImageAndSave(const string &inputName, ColorMap &colorMap, ImageHandler &imageHandler) {
  string inputFolder = "test_images/";
  string outputFolder = "/app/output/";

  imageHandler.readImage(inputFolder+ inputName);
  imageHandler.mapImage(colorMap);
  imageHandler.saveImage(outputFolder + inputName);
  for (int i = 2; i < 5; i++) {
    imageHandler.blurImage(i * 12 + 1);
    imageHandler.mapImage(colorMap);
    imageHandler.removeIslands(100);

    string outputFileName = outputFolder + "b_" + to_string(i) + inputName;
    imageHandler.saveImage(outputFileName);
    cout << "Saved " << outputFileName << endl;
  }

}

int main() {
  // Color red = Color(255, 0, 0);
  // Color green = Color(0, 255, 0);
  // Color blue = Color(0, 0, 255);
  // Color black = Color(0, 0, 0);
  // Color white = Color(255, 255, 255);
  // Color yellow = Color(255, 255, 0);
  // Color cyan = Color(0, 255, 255);
  // Color magenta = Color(255, 0, 255);
  // Color gray = Color(128, 128, 128);
  // Color orange = Color(255, 165, 0);

  // vector<Color> colors = {orange, red, yellow, blue, white, black, green};
  // ColorMap colorMap1 = ColorMap(colors);

  // ImageHandler imageHandler = ImageHandler();

  // mapImageAndSave("me.jpg", colorMap1, imageHandler);
  Server server(8080);
  server.start();


  //No frontend made yet, can map with this curl command
  //curl -Method Post -Uri http://localhost:8080/api/color_map -ContentType "application/json" -Body ('{"image": "' + [Convert]::ToBase64String([IO.File]::ReadAllBytes("image.png")) + '", "colors": ["#FF0000", "#000000", "#0000FF"], "threshold": 20}')
  return 0;

}