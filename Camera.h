#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string>
#include <fstream>
#include <iostream>
#include <rapidxml/rapidxml.hpp>

#include "HTTPClientSocket.h"

namespace lb{
    class Camera{
        public:
        Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password);
        ~Camera();

        std::string send_request(const std::string& cgi, const std::string& msubmenu, const std::string& action, const std::vector<std::pair<std::string, std::string>>& settings = {});

        void load_from_file(const std::string& path);
        void load_from_web(const std::string& path);

        void print_layer();
        void next(const std::string& name);
        void previous();

        std::string get_root_name();

        private:
        lb::HTTPClientSocket cs;

        rapidxml::xml_document<char> doc;
        rapidxml::xml_node<char> * root;
    };
}

#endif
