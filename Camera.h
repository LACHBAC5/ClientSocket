#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <rapidxml/rapidxml.hpp>

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

        bool apply_configuration(const Configuration& config);
        bool apply_setting(const setting& config);

        bool load_settings_web(const std::string& path);
        void load_settings_file(const std::string& path);

        setting get_current_setting() const;
        void clear_current_param();
        void change_current_param(const std::string& name, const std::string& value);
        void rm_current_param(const std::string& name);

        void clear_current_vpath();
        void change_current_vpath(const std::string& name, const std::string& value);
        void rm_current_vpath(const std::string& name);

        void change_current_qpath(const std::string& qpath);

        std::string read_node_name() const;
        std::string read_node_child_name(const std::string& name) const;

        std::vector<std::string> read_node_children() const;
        bool search_node_child(const std::string& name) const;

        bool enter_node_path(const std::string& path);
        bool next_node(const std::string& name);
        bool last_node();

        private:
        HTTPClientSocket socket;
        rapidxml::xml_document<char> xml_settings_doc;
        rapidxml::xml_node<char> * xml_settings_node;

        setting current_setting;
    };
}
