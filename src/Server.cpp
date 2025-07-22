#include <string>
#include <vector>
#include "../header/Server.hpp"
#include <iostream>


#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include "../header/Color.hpp"
#include "../header/ColorMap.hpp"
#include "../header/ImageHandler.hpp"

Server::Server(int port) : port(port), running(false) {
    std::cout << "Server initialized on port " << port << std::endl;
}


/**
 * @brief Decodes a Base64 encoded string
 * Decodes a Base64 encoded string into a vector of unsigned characters. Used for decoding image data in requests.
 * @param in The Base64 encoded string to decode.
 * @return A vector of unsigned characters representing the decoded data.
 */
static inline std::vector<uchar> base64_decode(const std::string &in) {
    std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[base64_chars[i]] = i;

    std::vector<uchar> out;
    int val = 0, valb = -8;
    for (uchar c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

/**
  * @brief Starts the server
 * Starts the server and listens for incoming requests.
 */
void Server::start() {
    running = true;

    CROW_ROUTE(colorMapServer, "/api/test")
    ([]() {
    
        return crow::response(200, "Success");
    });

    CROW_ROUTE(colorMapServer, "/api/image_processing")
    .methods("POST"_method)
    ([this](const crow::request &req) {
        std::string requestBody = req.body;
        std::cout << "Received image processing request: " << requestBody << std::endl;
        return handleImageProcessingRequest(requestBody);
    });

    CROW_ROUTE(colorMapServer, "/api/color_map")
    .methods("POST"_method)
    ([this](const crow::request &req) {
        std::string requestBody = req.body;
        std::cout << "Received color map request: " << requestBody << std::endl;
        return handleColorMapRequest(requestBody);
    });

    colorMapServer.port(port).multithreaded().run();
    std::cout << "Server started on port " << port << std::endl;
}

/**
 * @brief Stops the server
 * Stops the server and cleans up resources.
 */
void Server::stop() {
    running = false;
    colorMapServer.stop();
    std::cout << "Server stopped." << std::endl;
}

/**
 * @brief handles an image processing request
 * Processes an image processing request. The processing will turn the images into 3d models.
 * @param request The request to handle.
 * @return The response to the request.
 */
crow::response Server::handleImageProcessingRequest(const std::string &request) {
    return crow::response(200, "Success"); //TODO actually make this work
}

void mapColors(const std::vector<std::string> &colors, const cv::Mat &image) {
    ImageHandler imageHandler = ImageHandler();
    ColorMap colorMap = ColorMap(colors);
    std::string outputFolder = "/app/output/";

    imageHandler.setImage(image);
    imageHandler.mapImage(colorMap);
    imageHandler.saveImage(outputFolder + "mapped_image.jpg");
    for (int i = 2; i < 5; i++) {
        imageHandler.blurImage(i * 12 + 1);
        imageHandler.mapImage(colorMap);
        imageHandler.removeIslands(100);

        std::string outputFileName = outputFolder + "b_" + std::to_string(i) + "mapped_image.jpg";
        imageHandler.saveImage(outputFileName);
        std::cout << "Saved " << outputFileName << std::endl;
    }
}
/**
 * @brief handles a color mapping request
 * Processes a color map request and returns a response. These will be used to preview how the image will be split.
 * @param request The request to handle.
 * @return The response to the request.
 */
crow::response Server::handleColorMapRequest(const std::string& body) {
    using json = nlohmann::json;

    try {
        auto parsed = json::parse(body);

        std::string base64_img = parsed["image"];
        std::vector<uchar> raw_data = base64_decode(base64_img);

        cv::Mat mat = cv::imdecode(raw_data, cv::IMREAD_COLOR);
        if (mat.empty()) {
            return crow::response(400, "Could not decode image");
        }
        std::vector<std::string> colors = parsed.value("colors", std::vector<std::string>{});
        int threshold = parsed.value("threshold", 0);
        mapColors(colors, mat);

        

        return crow::response(200, "Image received and processed successfully");

    } catch (const std::exception& e) {
        return crow::response(400, std::string("Bad request: ") + e.what());
    }
}



/**
 * @brief Gets colors from mapping request
 * Extracts colors from a color mapping request.
 * @param request The request containing color information.
 * @return A vector of colors extracted from the request.
 */
std::vector<std::string> Server::getColorsFromMappingRequest(const std::string &request) const {
    std::cout << "Extracting colors from request: " << request << std::endl;
    return {};
     // TODO: Implement actual extraction logic
}

/**
 * @brief Validates the file format of an image
 * Checks if the provided file format is valid for image processing.
 * @param fileFormat The file format to validate.
 * @return True if the file format is valid, false otherwise.
 */
bool isValidFileFormat(const std::string &fileFormat) {
    std::vector<std::string> validFormats = {".jpg", ".jpeg", ".png", ".bmp", ".tiff"};
    for (const auto &format : validFormats) {
        if (fileFormat.find(format) != std::string::npos) {
            return true;
        }
    }
    return false;
}
