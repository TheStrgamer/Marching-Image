#include <iostream>
#include <string>
#include <vector>
#include "header/ColorMap.hpp"
#include "header/Color.hpp"
#include "header/ImageHandler.hpp"
#include "header/Server.hpp"


using namespace std;

int main() {
  Server server(8080);
  server.start();


  //No frontend made yet, can map with this curl command
  //curl -Method Post -Uri http://localhost:8080/api/color_map -ContentType "application/json" -Body ('{"image": "' + [Convert]::ToBase64String([IO.File]::ReadAllBytes("image.png")) + '", "colors": ["#FF0000", "#000000", "#0000FF"], "threshold": 20}')
  return 0;

}