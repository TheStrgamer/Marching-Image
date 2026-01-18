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

void ImageHandler::setImage(const cv::Mat &img) {
    image = img.clone();
    outputImage = img.clone();
    std::cout << "Image set successfully.\n";
    saveImage("/app/output/output_image.png");
    std::cout << "Image saved as output_image.png\n";
}

void ImageHandler::saveImage(const std::string &path) {
    std::cout << "Saving image to " << path << std::endl;
    imwrite(path, outputImage);
}
/**
 * @brief Map the image by setting the color of each pixel to the closest color in the color map
 * @param colorMap The color map to use
 * @param hsl if the method should use hsl distance
 */
void ImageHandler::mapImage(const ColorMap &colorMap, bool hsl) {
    std::cout << "Mapping image with " << colorMap.getColors().size() << " colors.\n";
    for (const auto &color : colorMap.getColors()) {
        std::cout << "Color: " << color.getHex() << "\n";
    }
    if (image.empty()) {
        std::cerr << "Error: No image loaded.\n";
        return;
    }
    cv::parallel_for_(cv::Range(0, image.rows), [&](const cv::Range &range) {
        for (int i = range.start; i < range.end; ++i) {
            auto *rowPtr = outputImage.ptr<cv::Vec3b>(i);
            for (int j = 0; j < image.cols; j++) {
                Color c = pixelToColor(rowPtr[j]);
                colorToPixel(colorMap.getClosestColor(c, hsl), rowPtr[j]);
            }
        }
    });
}

/**
 * @brief Map the image by setting the color of each pixel to the closest color in the color map
 * @param colorMap The color map to use
 */
void ImageHandler::mapImage(const ColorMap &colorMap) {
    mapImage(colorMap, false);
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


// TODO make this work better, kinda ass rn.
/**
 * @brief Remove small islands from the image
 * Removes small islands of color from the image. An island is a group of pixels of the same color.
 * @param islandSize The maximum size of an island to remove
*/
void ImageHandler::removeIslands(int islandSize) {
    if (outputImage.empty() || islandSize <= 0) return;

    cv::Mat visited = cv::Mat::zeros(outputImage.size(), CV_8U);
    std::vector<std::pair<int, int>> directions = {{0,1},{1,0},{0,-1},{-1,0}};

    auto colorsEqual = [](const Vec3b &a, const Vec3b &b) {
        // Tolerant equality check (threshold 0 → exact match)
        return a == b;
    };

    auto floodFill = [&](int x, int y, const Vec3b &targetColor) {
        std::vector<cv::Point> stack, islandPixels;
        stack.push_back({x, y});
        visited.at<uchar>(y, x) = 1;

        while (!stack.empty()) {
            cv::Point p = stack.back();
            stack.pop_back();
            islandPixels.push_back(p);

            for (const auto &dir : directions) {
                int nx = p.x + dir.first;
                int ny = p.y + dir.second;
                if (nx >= 0 && nx < outputImage.cols && ny >= 0 && ny < outputImage.rows &&
                    !visited.at<uchar>(ny, nx)) {
                    Vec3b currentColor = outputImage.at<Vec3b>(ny, nx);
                    if (colorsEqual(currentColor, targetColor)) {
                        stack.push_back({nx, ny});
                        visited.at<uchar>(ny, nx) = 1;
                    }
                }
            }
        }
        return islandPixels;
    };

    auto mostFrequentColor = [&](const std::vector<cv::Point> &points) -> Vec3b {
        std::map<std::tuple<uchar, uchar, uchar>, int> colorCount;
        for (const auto &p : points) {
            for (const auto &dir : directions) {
                int nx = p.x + dir.first;
                int ny = p.y + dir.second;
                if (nx >= 0 && nx < outputImage.cols && ny >= 0 && ny < outputImage.rows) {
                    Vec3b color = outputImage.at<Vec3b>(ny, nx);
                    colorCount[{color[0], color[1], color[2]}]++;
                }
            }
        }
        if (colorCount.empty()) return outputImage.at<Vec3b>(points.front().y, points.front().x);

        auto maxElement = std::max_element(
            colorCount.begin(), colorCount.end(),
            [](const auto &a, const auto &b) { return a.second < b.second; }
        );
        return Vec3b(std::get<0>(maxElement->first), std::get<1>(maxElement->first), std::get<2>(maxElement->first));
    };

    for (int y = 0; y < outputImage.rows; y++) {
        for (int x = 0; x < outputImage.cols; x++) {
            if (!visited.at<uchar>(y, x)) {
                Vec3b targetColor = outputImage.at<Vec3b>(y, x);
                auto island = floodFill(x, y, targetColor);
                if (static_cast<int>(island.size()) <= islandSize) {
                    Vec3b newColor = mostFrequentColor(island);
                    for (const auto &p : island) {
                        outputImage.at<Vec3b>(p.y, p.x) = newColor;
                    }
                }
            }
        }
    }
}

/**
 * @brief returns a matrix representation of the image given a color
 * The values are 0 if the color of a pixel does not match with the given color
 * and 1 if the color matches.
 * 
 * @param color the color to match with
 * @return the matrix
 */
Matrix ImageHandler::getImageAsMatrix(const Color &color) {
    Matrix m(image.rows+2, std::vector<int>(image.cols+2, 0));

    cv::parallel_for_(cv::Range(0, image.rows),
        [&](const cv::Range& range) {
            for (int i = range.start; i < range.end; ++i) {
                const cv::Vec3b* rowPtr = image.ptr<cv::Vec3b>(i);
                for (int j = 0; j < image.cols; ++j) {
                    Color c = pixelToColor(rowPtr[j]);
                    m[i+1][j+1] = (c == color) ? 1 : 0;
                }
            }
        }
    );

    return m;
}

/**
 * @brief downscales image to maxSize
 * @param maxSize the maximum width/height the image can have
 */
void ImageHandler::downScaleImage(int maxSize) {

    if (image.empty() || maxSize <= 0) {
        return;
    }
    int width  = image.cols;
    int height = image.rows;

    int largest = std::max(width, height);

    if (largest <= maxSize) {
        return;
    }

    double scale = static_cast<double>(maxSize) / largest;
    int newWidth  = static_cast<int>(width  * scale);
    int newHeight = static_cast<int>(height * scale);

    cv::resize(
        image,
        image,
        cv::Size(newWidth, newHeight),
        0,
        0,
        cv::INTER_AREA
    );    
    cv::resize(
        outputImage,
        outputImage,
        cv::Size(newWidth, newHeight),
        0,
        0,
        cv::INTER_AREA
    );
}

