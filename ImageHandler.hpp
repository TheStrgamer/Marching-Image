#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
inline void displayInvertedImage(const std::string &path) {
    Mat image = imread(path);
    if (image.empty()) {
        std::cerr << "Could not read the image: " << path << std::endl;
        return;
    }
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            image.at<Vec3b>(y, x)[0] = 255 - image.at<Vec3b>(y, x)[0];
        }
    }
    imshow("Original Image", image);
    waitKey(0);

}