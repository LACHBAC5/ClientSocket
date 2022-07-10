#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string>
#include <fstream>
#include <iostream>
#include <rapidxml/rapidxml.hpp>
#include <algorithm>

#include "HTTPClientSocket.h"
#include "CameraConfiguration.h"

namespace lb{
    class Camera{
        public:
        Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password);
        Camera(const std::string& ip, const std::string& port);
        ~Camera();

        void change_password(const std::string& password);
        void change_username(const std::string& username);

        std::string send_request(const std::string& path, const std::map<std::string, std::string>& names, const std::map<std::string, std::string>& parameters);

        void load_from_file(const std::string& path);
        void load_from_web(const std::string& path);

        std::string get_layer(bool show_node_name=true, bool show_attrib_name=true, const std::vector<std::string>& attribute_names={});
        
        bool next(const std::vector<std::pair<std::string, std::string>>& names);
        bool previous();

        std::string get_root_name();
        bool get_attrib(const std::vector<std::pair<std::string, std::string>>& names);


        private:
        lb::HTTPClientSocket cs;

        rapidxml::xml_document<char> doc;
        rapidxml::xml_node<char> * root;

        std::map<std::string, Configuration> config;
    };
}

#endif
