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


const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

/**
 * @brief Decodes a Base64 encoded string
 * Decodes a Base64 encoded string into a vector of unsigned characters. Used for decoding image data in requests.
 * @param in The Base64 encoded string to decode.
 * @return A vector of unsigned characters representing the decoded data.
 */
static inline std::vector<uchar> base64_decode(const std::string &in) {
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
 * @brief encodes bytes to a base64 string
 * @param bytes_to_encode the bytes that are encoded
 * @param in_len the length
 * @return the base64 string
 */
std::string base64_encode(const unsigned char* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; i < 4; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; j < i + 1; j++)
            ret += base64_chars[char_array_4[j]];

        while (i++ < 3)
            ret += '=';
    }

    return ret;
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

cv::Mat mapColors(const std::vector<std::string> &colors, const cv::Mat &image) {
    ImageHandler imageHandler = ImageHandler();
    ColorMap colorMap = ColorMap(colors);
    std::string outputFolder = "/app/output/";

    std::cout << "colors: ";
    for (const Color &color : colorMap.getColors()) {
        std::cout << color.getHex() << " ";
    }

    imageHandler.setImage(image);
    imageHandler.mapImage(colorMap);
    imageHandler.saveImage(outputFolder + "mapped_image.jpg");
    // for (int i = 4; i < 5; i++) {
    //     imageHandler.blurImage(i * 12 + 1);
    //     imageHandler.mapImage(colorMap);

    //     std::string outputFileName = outputFolder + "b_" + std::to_string(i) + "mapped_image.png";
    //     imageHandler.saveImage(outputFileName);
    //     std::cout << "Saved " << outputFileName << std::endl;
    // }
    return imageHandler.getImage();
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

        cv::Mat processed = mapColors(colors, mat);

        // Encode processed image to PNG in-memory
        std::vector<uchar> buf;
        cv::imencode(".png", processed, buf);
        

        std::string encoded_img = base64_encode(buf.data(), buf.size());

        json response_json;
        response_json["image"] = encoded_img;

        return crow::response(200, response_json.dump());

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
