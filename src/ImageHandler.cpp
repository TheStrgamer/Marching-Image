#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include "../header/ImageHandler.hpp"
#include "../header/Color.hpp"
#include "../header/ColorMap.hpp"

/**
 * @brief Convert a color to a pixel
 * @param color The color to convert
 * @param pixel The pixel to set
 */
void colorToPixel(const Color &color, Vec3b &pixel) {
    pixel[2] = color.getRed();
    pixel[1] = color.getGreen();
    pixel[0] = color.getBlue();
}
/**
 * @brief Convert a pixel to a color
 * @param pixel The pixel to convert
 * @return The color
 */
Color pixelToColor(const Vec3b &pixel) {
    Color color;
    color.setRed(pixel[2]);
    color.setGreen(pixel[1]);
    color.setBlue(pixel[0]);
    return color;
}

ImageHandler::ImageHandler() {
}
ImageHandler::ImageHandler(const std::string &path) {
    readImage(path);
}

void ImageHandler::readImage(const std::string &path) {
    image = imread(path);
    if (image.empty()) {
        std::cerr << "Error: Could not read image at " << path << std::endl;
        return;
    }
    outputImage = image.clone();
}

void ImageHandler::saveImage(const std::string &path) {
    imwrite(path, outputImage);
}
/**
 * @brief Map the image by setting the color of each pixel to the closest color in the color map
 * @param colorMap The color map to use
 */
void ImageHandler::mapImage(const ColorMap &colorMap) {
    if (image.empty()) {
        std::cerr << "Error: No image loaded.\n";
        return;
    }
    cv::parallel_for_(cv::Range(0, image.rows), [&](const cv::Range &range) {
        for (int i = range.start; i < range.end; ++i) {
            auto *rowPtr = outputImage.ptr<Vec3b>(i);
            for (int j = 0; j < image.cols; j++) {
                Color color = pixelToColor(rowPtr[j]);
                colorToPixel(colorMap.getClosestColor(color), rowPtr[j]);
            }
        }
    });
}
/**
 * @brief Map the image by setting the color of each pixel to the closest color in the color map
 * @param colorMap The color map to use
 * @param path The path to read the image from
 */
void ImageHandler::mapImage(ColorMap &colorMap, const std::string &path) {
    readImage(path);
    if (!image.empty()) {
        mapImage(colorMap);
    }
}
/**
 * @brief Blur the image
 * Blurs the image using a kernel of the given size. Useful for reducing noise before mapping.
 * @param kernelSize The size of the kernel
 */
void ImageHandler::blurImage(int kernelSize) {
    if (image.empty()) {
        std::cerr << "Error: No image loaded.\n";
        return;
    }
    cv::bilateralFilter(image, outputImage, kernelSize, kernelSize * 2, kernelSize / 2);
}

/**
 * @brief Remove small islands from the image
 * Removes small islands of color from the image. An island is a group of pixels of the same color.
 * @param islandSize The maximum size of an island to remove
*/
void ImageHandler::removeIslands(int islandSize) {
    if (outputImage.empty()) return;

    cv::Mat visited = cv::Mat::zeros(outputImage.size(), CV_8U);

    std::vector<std::pair<int, int>> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}
    };

    auto floodFill = [&](int x, int y, const Vec3b &targetColor) {
        std::vector<cv::Point> stack, islandPixels;
        stack.push_back(cv::Point(x, y));
        visited.at<uchar>(y, x) = 1;

        while (!stack.empty()) {
            cv::Point p = stack.back();
            stack.pop_back();
            islandPixels.push_back(p);

            for (const auto &dir : directions) {
                int nx = p.x + dir.first;
                int ny = p.y + dir.second;

                if (nx >= 0 && nx < outputImage.cols && ny >= 0 && ny < outputImage.rows && !visited.at<uchar>(ny, nx)) {
                    Vec3b currentColor = outputImage.at<Vec3b>(ny, nx);
                    if (currentColor == targetColor) {
                        stack.push_back(cv::Point(nx, ny));
                        visited.at<uchar>(ny, nx) = 1;
                    }
                }
            }
        }

        return islandPixels;
    };

    auto mostFrequentColor = [&](const std::vector<cv::Point> &points) {
        std::map<std::tuple<uchar, uchar, uchar>, int> colorCount;

        for (const auto &p : points) {
            for (const auto &dir : directions) {
                int nx = p.x + dir.first;
                int ny = p.y + dir.second;
                if (nx >= 0 && nx < outputImage.cols && ny >= 0 && ny < outputImage.rows) {
                    Vec3b color = outputImage.at<Vec3b>(ny, nx);
                    colorCount[std::make_tuple(color[0], color[1], color[2])]++;
                }
            }
        }

        auto maxElement = std::max_element(colorCount.begin(), colorCount.end(),
                                           [](const auto &a, const auto &b) { return a.second < b.second; });

        return Vec3b(std::get<0>(maxElement->first), std::get<1>(maxElement->first), std::get<2>(maxElement->first));
    };

    for (int y = 0; y < outputImage.rows; y++) {
        for (int x = 0; x < outputImage.cols; x++) {
            if (visited.at<uchar>(y, x) == 0) {
                Vec3b targetColor = outputImage.at<Vec3b>(y, x);
                std::vector<cv::Point> island = floodFill(x, y, targetColor);

                if (island.size() <= islandSize) {
                    Vec3b newColor = mostFrequentColor(island);
                    for (const auto &p : island) {
                        outputImage.at<Vec3b>(p.y, p.x) = newColor;
                    }
                }
            }
        }
    }
}

