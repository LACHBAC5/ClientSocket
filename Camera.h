#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string>

#include "ClientSocket.h"

namespace lb{
    class Camera{
        public:
        Camera(const std::string&, const std::string&, const std::string&, const std::string&);
        ~Camera();

        std::string send_request(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&, const std::string&);

        private:
        lb::ClientSocket cs;
        lb::HeaderData info;
    };
}

#endif
