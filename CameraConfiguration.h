#pragma once

#include <string>
#include <map>
#include <vector>
#include <fstream>

namespace lb{
    struct setting{
        std::map<std::string, std::string> parameters;
        std::map<std::string, std::string> VIRpath;
        std::string REQpath;
    };

    typedef std::vector<setting> Configuration;

    class CameraConfiguration{
        public:
        bool create_configuration(const std::string& name);
        bool delete_configuration(const std::string& name);

        bool add_to_configuration(const std::string& name, const setting& setting);
        bool rm_from_configuration(const std::string& name, const setting& setting);

        bool save_configuration(const std::string& file, const std::string& name);
        bool load_configuration(const std::string& file);

        Configuration get_configuration(const std::string& name);

        private:
        std::map<std::string, Configuration> configurations {{"default", {}}};
    };
}
