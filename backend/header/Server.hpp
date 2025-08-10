#include <string>
#include <vector>
#define CROW_USE_BOOST_ASIO
#include "crow.h"
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

// CORS middleware
struct CORS {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context&) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");

        // Handle preflight (OPTIONS) request immediately
        if (req.method == crow::HTTPMethod::Options) {
            res.code = 200;
            res.end();
        }
    }

    void after_handle(crow::request&, crow::response& res, context&) {
        // Add headers again (important for non-OPTIONS responses)
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
    }
};
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
    crow::App<CORS> colorMapServer;
    std::vector<std::string> getColorsFromMappingRequest(const std::string &request) const;
    bool isValidFileFormat(const std::string &fileFormat) const;



};


   

