#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>


using namespace cv;

void colorToPixel(const Color &color, Vec3b &pixel) {
    pixel[2] = color.getRed();
    pixel[1] = color.getGreen();
    pixel[0] = color.getBlue();
}
Color pixelToColor(const Vec3b &pixel) {
    Color color;
    color.setRed(pixel[2]);
    color.setGreen(pixel[1]);
    color.setBlue(pixel[0]);
    return color;

}


class ImageHandler {
  public:
    ImageHandler();
    ImageHandler(const std::string &path);
    void readImage(const std::string &path);
    void saveImage(const std::string &path);
    void mapImage(ColorMap &colorMap);
    void mapImage(ColorMap &colorMap, const std::string &path);

  private:
    Mat image;
    Mat outputImage;
    std::vector<Mat> channels;
};


inline ImageHandler::ImageHandler() {
    channels = {};
}
inline ImageHandler::ImageHandler(const std::string &path) {
    readImage(path);
    channels = {};
}
inline void ImageHandler::readImage(const std::string &path) {
    image = imread(path);
    outputImage = image.clone();
}
inline void ImageHandler::saveImage(const std::string &path) {
    imwrite(path, outputImage);
}
inline void ImageHandler::mapImage(ColorMap &colorMap) {
    for (int i=0; i<image.rows; i++) {
        for (int j=0; j<image.cols; j++) {
            Vec3b pixel = image.at<Vec3b>(i, j);
            Color color = pixelToColor(pixel);
            colorToPixel(colorMap.getClosestColor(color), pixel);
            outputImage.at<Vec3b>(i, j) = pixel;
        }
    }
}

inline void ImageHandler::mapImage(ColorMap &colorMap, const std::string &path) {
    readImage(path);
    mapImage(colorMap);
}

