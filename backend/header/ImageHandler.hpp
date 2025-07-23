#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include "Color.hpp"
#include "ColorMap.hpp"

using namespace cv;
/**
 * @brief Convert a color to a pixel
 * @param color The color to convert
 * @param pixel The pixel to set
 */
void colorToPixel(const Color &color, Vec3b &pixel);
/**
 * @brief Convert a pixel to a color
 * @param pixel The pixel to convert
 * @return The color
 */
Color pixelToColor(const Vec3b &pixel);

/**
 * @brief  A class to handle image reading writing and processing
 * A class to handle image reading writing and processing. It has functions for reading, saving, blurring and mapping images
 */
class ImageHandler {
  public:
    ImageHandler();
    explicit ImageHandler(const std::string &path);
    void readImage(const std::string &path);
    void setImage(const Mat &img);
    void saveImage(const std::string &path);
    void mapImage(const ColorMap &colorMap);
    void mapImage(ColorMap &colorMap, const std::string &path);
    void blurImage(int kernelSize);
    void removeIslands(int islandSize);

  private:
    Mat image;
    Mat outputImage;
    ColorMap *colorMapPtr{};
    int currentRow{};
};