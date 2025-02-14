#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>

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
    static void *handleRow(void *args);
    Mat image;
    Mat outputImage;
    ColorMap *colorMapPtr;
    int currentRow;
    pthread_mutex_t mutex;
};

inline ImageHandler::ImageHandler() {
    pthread_mutex_init(&mutex, NULL);
}
inline ImageHandler::ImageHandler(const std::string &path) {
    readImage(path);
    pthread_mutex_init(&mutex, NULL);
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

void *ImageHandler::handleRow(void *args) {
    ImageHandler *self = static_cast<ImageHandler *>(args);

    while (true) {
        pthread_mutex_lock(&self->mutex);
        int row = self->currentRow++;
        pthread_mutex_unlock(&self->mutex);

        if (row >= self->image.rows) {
            return NULL;
        }

        for (int j = 0; j < self->image.cols; j++) {
            Vec3b pixel = self->image.at<Vec3b>(row, j);
            Color color = pixelToColor(pixel);
            colorToPixel(self->colorMapPtr->getClosestColor(color), pixel);
            self->outputImage.at<Vec3b>(row, j) = pixel;
        }
    }
    return NULL;
}

inline void ImageHandler::mapImage(ColorMap &colorMap) {
    colorMapPtr = &colorMap;
    currentRow = 0;
    
    int numThreads = std::min(4, image.rows);
    std::vector<pthread_t> threads(numThreads);
    
    for (int i = 0; i < numThreads; i++) {
        pthread_create(&threads[i], NULL, handleRow, this);
    }
    
    for (auto &thread : threads) {
        pthread_join(thread, NULL);
    }
}

inline void ImageHandler::mapImage(ColorMap &colorMap, const std::string &path) {
    readImage(path);
    if (!image.empty()) {
        mapImage(colorMap);
    }
}
