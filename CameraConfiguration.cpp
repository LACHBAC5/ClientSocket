#include "CameraConfiguration.h"

namespace lb{
    std::string Configuration::get_config(){
        std::string out;
        for(int i = 0; i < settings.size(); i++){
            out += "Group. " + std::to_string(i) + '\n';
            for(const auto& [name, value] : settings[i].parameters){
                out += name + " = " + value + '\n';
            }
        }
        return out;
    }
}