#include <string>
#include <vector>
#define CROW_USE_BOOST_ASIO
#include "crow.h"
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

/**
 * @brief A class to represent a server
 * A class to represent a server that handles image processing requests.
 */

class Server {
public:
    explicit Server(int port);
    void start();
    void stop();
    crow::response handleImageProcessingRequest(const std::string &request);
    crow::response handleColorMapRequest(const std::string &request);

private:
    int port;
    bool running;
    crow::SimpleApp colorMapServer;
    std::vector<std::string> getColorsFromMappingRequest(const std::string &request) const;
    bool isValidFileFormat(const std::string &fileFormat) const;



};


   

