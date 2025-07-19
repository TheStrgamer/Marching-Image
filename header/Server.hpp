#include <string>
#include <vector>


/**
 * @brief A class to represent a server
 * A class to represent a server that handles image processing requests.
 */

class Server {
public:
    /**
     * @brief Constructor for the Server class
     * Initializes the server with a port number.
     * @param port The port number to listen on.
     */
    explicit Server(int port);

    /**
     * @brief Starts the server
     * Starts the server and listens for incoming requests.
     */
    void start();

    /**
     * @brief Stops the server
     * Stops the server and cleans up resources.
     */
    void stop();

    /**
     * @brief Handles an incoming request
     * Processes the request and returns a response.
     * @param request The request to handle.
     * @return The response to the request.
     */
    std::string handleRequest(const std::string &request);

    /**
     * @brief handles an image processing request
     * Processes an image processing request. The processing will turn the images into 3d models.
     * @param request The request to handle.
     * @return The response to the request.
     */
    std::string handleImageProcessingRequest(const std::string &request);

    /**
     * @brief handles a color mapping request
     * Processes a color map request and returns a response. These will be used to preview how the image will be split.
     * @param request The request to handle.
     * @return The response to the request.
     */
    std::string handleColorMapRequest(const std::string &request);


    /**
     * @brief returns a standard invalid file format response
     * @return A string containing the invalid file format response.
     */
    std::string invalidFileFormatResponse() const;

private:
    int port;
    bool running;

    /**
     * @brief Gets colors from mapping request
     * Extracts colors from a color mapping request.
     * @param request The request containing color information.
     * @return A vector of colors extracted from the request.
     */
    std::vector<std::string> getColorsFromMappingRequest(const std::string &request) const;

    /**
     * @brief Validates the file format of an image
     * Checks if the provided file format is valid for image processing.
     * @param fileFormat The file format to validate.
     * @return True if the file format is valid, false otherwise.
     */
    bool isValidFileFormat(const std::string &fileFormat) const;

    

};


   

