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
    void blurImage(int kernelSize);

  private:
    Mat image;
    Mat outputImage;
    ColorMap *colorMapPtr;
    int currentRow;
};

inline ImageHandler::ImageHandler() {
}
inline ImageHandler::ImageHandler(const std::string &path) {
    readImage(path);
}

inline void ImageHandler::readImage(const std::string &path) {
    image = imread(path);
    if (image.empty()) {
        std::cerr << "Error: Could not read image at " << path << std::endl;
        return;
    }
    outputImage = image.clone();
}

inline void ImageHandler::saveImage(const std::string &path) {
    imwrite(path, outputImage);
}

inline void ImageHandler::mapImage(ColorMap &colorMap) {
    if (image.empty()) {
        std::cerr << "Error: No image loaded.\n";
        return;
    }
    cv::parallel_for_(cv::Range(0, image.rows), [&](const cv::Range &range) {
        for (int i = range.start; i < range.end; ++i) {
            Vec3b *rowPtr = outputImage.ptr<Vec3b>(i);
            for (int j = 0; j < image.cols; j++) {
                Color color = pixelToColor(rowPtr[j]);
                colorToPixel(colorMap.getClosestColor(color), rowPtr[j]);
            }
        }
    });
}

inline void ImageHandler::mapImage(ColorMap &colorMap, const std::string &path) {
    readImage(path);
    if (!image.empty()) {
        mapImage(colorMap);
    }
}

inline void ImageHandler::blurImage(int kernelSize) {
    if (image.empty()) {
        std::cerr << "Error: No image loaded.\n";
        return;
    }
    cv::blur(image, outputImage, cv::Size(kernelSize, kernelSize));
}


