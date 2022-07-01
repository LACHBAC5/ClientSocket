#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string>

#include "ClientSocket.h"

namespace lb{
    class Camera{
        public:
        Camera(const std::string&, const std::string&, const std::string&, const std::string&);
        ~Camera();

        std::string send_request(const std::string&, const std::string&);
        private:
        lb::ClientSocket socket;

        std::string username_;
        std::string password_;
        std::string cnonce_ = "83ba2d99eed620dd";
        std::string realm_;
        std::string nonce_;
        std::string qop_;
        int nc_ = 0;
    };
}

#endif