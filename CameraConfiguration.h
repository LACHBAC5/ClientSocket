#ifndef CAMERACONFIGURATIONHEADER
#define CAMERACONFIGURATIONHEADER

#include <map>
#include <string>
#include <vector>

namespace lb{
    struct setting{
        std::string path;
        std::map<std::string, std::string> names;
        std::map<std::string, std::string> parameters;
    };

    class Configuration{
        public:
        std::string get_config();
        private:
        std::vector<setting> settings;
    };
}

#endif