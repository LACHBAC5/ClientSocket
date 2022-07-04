#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string>

#include "ClientSocket.h"

namespace lb{
    class Camera{
        public:
        Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password);
        ~Camera();

        std::string send_request(const std::string& cgi, const std::string& msubmenu, const std::string& action, std::vector<std::pair<std::string, std::string>> settings);

        private:
        lb::ClientSocket cs;
        lb::HeaderData info;
    };
}

#endif
