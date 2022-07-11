#include "Camera.h"

namespace lb{
    Camera::Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : cs(ip, port, username, password) {}
    Camera::Camera(const std::string& ip, const std::string& port) : cs(ip, port) {}
    Camera::~Camera() {}

    void Camera::change_password(const std::string& password){
        cs.change_password(password);
    }
    void Camera::change_username(const std::string& username){
        cs.change_username(username);
    }

    bool Camera::apply_setting(const setting& sett){
        cs.open_connection();
        
        std::string request = sett.REQpath+'?';

        for(const auto& [name, value] : sett.VIRpath){
            request += name + '=' + value + '&';
        }
        for(const auto& [name, value] : sett.parameters){
            request += name + '=' + value + '&';
        }

        request.pop_back();

        std::cout << request << '\n';

        if(cs.status_code(cs.send_http_request("GET", request)) != 200){
            return false;
        }
        
        return true;
    }

    bool Camera::apply_configuration(const Configuration& config){
        cs.open_connection();
        for(const auto& item : config){
            if(!apply_setting(item)){
                return false;
            }
        }
        return true;
    }

    std::string Camera::load_from_file(const std::string& path){
        std::string text, line;
        std::ifstream file(path);
        while(std::getline(file, line) && line.size() != 0){
            text += line;
        }
        return text;
    }
    std::string Camera::load_from_web(const std::string& path){
        if(cs.open_connection()!=0){
            return "";
        }
        if(cs.status_code(cs.send_http_request("GET", path)) != 200){
            return "";
        }
        return cs.fetch_response();
    }

    bool Camera::create_configuration(const std::string& name){
        if(configurations.find(name) != configurations.end()){
            return false;
        }
        configurations.insert({name, {}});
        return true;
    }

    bool Camera::delete_configuration(const std::string& name){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return false;
        }
        configurations.erase(item);
        return true;
    }

    bool Camera::add_to_configuration(const std::string& name, const setting& setting){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return false;
        }
        bool found=false;
        for(auto& currentSetting : item->second){
            if(currentSetting.VIRpath == setting.VIRpath && currentSetting.REQpath == setting.REQpath){
                for(const auto& param : setting.parameters){
                    currentSetting.parameters.insert(param);
                }
                found=true; break;
            }
        }
        if(!found){
            configurations[name].push_back(setting);
        }
        return true;
    }

    bool Camera::rm_from_configuration(const std::string& name, const setting& setting){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            // when config name doesn't match
            return false;
        }

        for(auto& currentSetting : item->second){
            if(currentSetting.VIRpath == setting.VIRpath && currentSetting.REQpath == setting.REQpath){
                for(const auto& [name, value] : setting.parameters){
                    if(currentSetting.parameters.find(name) == currentSetting.parameters.end()){
                        // when settings do not match
                        return false;
                    }
                    
                    currentSetting.parameters.erase(name);
                }

                // when settings and dir match
                return true;
            }
        }

        // when config name matches but dir doesn't
        return false;
    }

    bool Camera::set_active_configuration(const std::string& name){
        auto item = configurations.find(name);
        if(item == configurations.end()){
            return false;
        }

        apply_configuration(item->second);

        return true;
    }
}