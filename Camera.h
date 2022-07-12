#ifndef CAMERAHEADER
#define CAMERAHEADER

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include "HTTPClientSocket.h"

namespace lb{

    struct setting{
        std::map<std::string, std::string> parameters;
        std::map<std::string, std::string> VIRpath;
        std::string REQpath; 
    };

    typedef std::vector<setting> Configuration;

    class Camera{
        public:
        Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password);
        Camera(const std::string& ip, const std::string& port);
        ~Camera();

        void change_password(const std::string& password);
        void change_username(const std::string& username);

        bool apply_setting(const setting& sett);
        bool apply_configuration(const Configuration& config);

        std::string load_from_file(const std::string& path);
        std::string load_from_web(const std::string& path);

        bool create_configuration(const std::string& name);
        bool delete_configuration(const std::string& name);
        bool add_to_configuration(const std::string& name, const setting& setting);
        bool rm_from_configuration(const std::string& name, const setting& setting);
        bool set_active_configuration(const std::string& name);

        Configuration get_configuration(const std::string& name);
        bool find_configuration(const std::string& name);

        private:
        lb::HTTPClientSocket cs;
        std::map<std::string, Configuration> configurations {{"default", {}}};
    };
}

#endif
