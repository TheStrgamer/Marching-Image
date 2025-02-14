#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>


using namespace cv;

class ImageHandler {
  public:
    ImageHandler();
    ImageHandler(const std::string &path);
    ImageHandler(const std::string &path, const ColorMap &colorMap);
    void readImage(const std::string &path);
    void saveImage(const std::string &path);
    void mapImage(const ColorMap &colorMap);

  private:
    Mat image;
    std::vector<Mat> channels;
    ColorMap colorMap;

};

