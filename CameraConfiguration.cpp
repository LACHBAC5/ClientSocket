#include "CameraConfiguration.h"

namespace lb{
    bool CameraConfiguration::create_configuration(const std::string& name){
        if(configurations.find(name) != configurations.end()){
            return false;
        }
        configurations.insert({name, {}});
        return true;
    }

    bool CameraConfiguration::delete_configuration(const std::string& name){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return false;
        }
        configurations.erase(item);
        return true;
    }

    bool CameraConfiguration::add_to_configuration(const std::string& name, const setting& setting){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return false;
        }
        bool found=false;
        for(auto& currentSetting : item->second){
            if(currentSetting.VIRpath == setting.VIRpath && currentSetting.REQpath == setting.REQpath){
                for(const auto& [name, value] : setting.parameters){
                    currentSetting.parameters[name] = value;
                }
                found=true; break;
            }
        }
        if(!found){
            configurations[name].push_back(setting);
        }
        return true;
    }

    bool CameraConfiguration::rm_from_configuration(const std::string& name, const setting& setting){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return false;
        }

        for(auto& currentSetting : item->second){
            if(currentSetting.VIRpath == setting.VIRpath && currentSetting.REQpath == setting.REQpath){
                for(const auto& [name, value] : setting.parameters){
                    if(currentSetting.parameters.find(name) == currentSetting.parameters.end()){
                        return false;
                    }
                    
                    currentSetting.parameters.erase(name);
                }
                return true;
            }
        }
        return false;
    }

    bool CameraConfiguration::save_configuration(const std::string& file, const std::string& name){
        std::ofstream writefile(file);
        Configuration config = get_configuration(name);
        if(config.size() == 0){
            return false;
        }
        writefile << name << '\n';
        for(const auto& item : config){
            writefile << "parameters\n";
            for(const auto& param : item.parameters){
                writefile << param.first << '=' << param.second << '\n';
            }
            writefile << "VIRpath\n";
            for(const auto& path : item.VIRpath){
                writefile << path.first << '=' << path.second << '\n';
            }
            writefile << "REQpath\n" << item.REQpath << '\n';
        }
        return true;
    }

    bool CameraConfiguration::load_configuration(const std::string& file){
        std::ifstream readfile(file);
        std::string line, name;
        setting set;

        std::getline(readfile, line);

        name=line;
        if(!create_configuration(name)){
            return false;
        }

        while(std::getline(readfile, line) && line.size() > 0){
            std::getline(readfile, line); int pos;
            while((pos=line.find('='))!=std::string::npos){
                set.parameters[line.substr(0, pos)] = line.substr(pos+1, line.size()-pos-1);
                std::getline(readfile, line);
            }
            
            std::getline(readfile, line);;
            while((pos=line.find('='))!=std::string::npos){
                set.VIRpath[line.substr(0, pos)] = line.substr(pos+1, line.size()-pos-1);
                std::getline(readfile, line);
            }
            std::getline(readfile, line);
            set.REQpath=line;

            add_to_configuration(name, set);
        }
        return true;
    }

    Configuration CameraConfiguration::get_configuration(const std::string& name){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return {};
        }

        return item->second;
    }
}