#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string.h>
#include "ClientSocket.h"


namespace lb{
    class Camera{
        public:
        Camera(const std::string&, const std::string&);
        ~Camera();

        

        private:
        ClientSocket cs;
    };
}

#endif