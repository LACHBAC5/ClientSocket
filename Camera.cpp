#include "Camera.h"

namespace lb{
    Camera::Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : socket(ip, port, username, password) {}
    Camera::Camera(const std::string& ip, const std::string& port) : socket(ip, port) {}
    Camera::~Camera() {}

    void Camera::change_password(const std::string& password){
        socket.change_password(password);
    }
    void Camera::change_username(const std::string& username){
        socket.change_username(username);
    }

    bool Camera::apply_setting(const setting& sett){
        socket.open_connection();
        
        std::string request = sett.REQpath+'?';

        for(const auto& [name, value] : sett.VIRpath){
            request += name + '=' + value + '&';
        }
        for(const auto& [name, value] : sett.parameters){
            request += name + '=' + value + '&';
        }

        request.pop_back();
        
        if(socket.status_code(socket.send_http_request("GET", request)) != 200){
            return false;
        }
        
        return true;
    }

    bool Camera::apply_configuration(const Configuration& config){
        socket.open_connection();
        for(const auto& item : config){
            if(!apply_setting(item)){
                return false;
            }
        }
        return true;
    }

    bool Camera::load_settings_web(const std::string& path){
        if(socket.open_connection()!=0){
            return false;
        }
        if(socket.status_code(socket.send_http_request("GET", path)) != 200){
            return false;
        }
        xml_settings_doc.parse<0>(&socket.fetch_response()[0]);
        xml_settings_node=xml_settings_doc.first_node();
        return true;
    }

    void Camera::load_settings_file(const std::string& path){
        std::string text, line;
        std::ifstream file(path);
        while(std::getline(file, line) && line.size() > 0){
            text += line;
        }
        xml_settings_doc.parse<0>(&text[0]);
        xml_settings_node=xml_settings_doc.first_node();
    }

    setting Camera::get_current_setting() const {
        return current_setting;
    }

    void Camera::clear_current_param(){
        current_setting.parameters.clear();
    }

    void Camera::change_current_param(const std::string& name, const std::string& value){
        current_setting.parameters[name]=value;
    }

    void Camera::rm_current_param(const std::string& name){
        auto item = current_setting.parameters.find(name);
        if(item == current_setting.parameters.end()){
            return;
        }
        current_setting.parameters.erase(item);
    }

    void Camera::clear_current_vpath(){
        current_setting.VIRpath.clear();
    }

    void Camera::change_current_vpath(const std::string& name, const std::string& value){
        current_setting.VIRpath[name]=value;
    }

    void Camera::rm_current_vpath(const std::string& name){
        auto item = current_setting.VIRpath.find(name);
        if(item == current_setting.VIRpath.end()){
            return;
        }
        current_setting.VIRpath.erase(item);
    }

    std::string Camera::read_node_name() const {
        return xml_settings_node->name();
    }

    std::string Camera::read_node_child_name(const std::string& name) const {
        for(auto item = xml_settings_node->first_node(); item!=nullptr; item=item->next_sibling()){
            auto attribute_name = item->first_attribute("name");
            if(attribute_name != nullptr && name == attribute_name->value()){
                return item->name();
            }
        }
        return "";
    }

    std::vector<std::string> Camera::read_node_children() const {
        std::vector<std::string> out;

        for(auto item=xml_settings_node->first_node(); item!=nullptr; item=item->next_sibling()){
            auto attribute_name = item->first_attribute("name");
            if(attribute_name != nullptr){
                out.push_back(attribute_name->value());
            }
        }

        return out;
    }
    bool Camera::search_node_child(const std::string& name) const {
        for(auto item = xml_settings_node->first_node(); item!=nullptr; item=item->next_sibling()){
            auto attribute_name = item->first_attribute("name");
            if(attribute_name != nullptr && name == attribute_name->value()){
                return true;
            }
        }
        return false;
    }

    bool Camera::enter_node_path(const std::string& path){
        std::string::const_iterator start=path.cbegin(), end=path.cend(), middle;
        while(middle!=end){
            middle=std::find(start, end, '/');

            if(middle-start>1){
                std::string folder(start, middle);
                if(folder == ".."){
                    if(!last_node()){
                        return false;
                    }
                }
                else
                {
                    if(!next_node(folder)){
                        return false;
                    }
                }
            }

            start=middle+1;
        }
        return true;
    }

    bool Camera::next_node(const std::string& name){
        for(auto item = xml_settings_node->first_node(); item!=nullptr; item=item->next_sibling()){
            if(strcmp(item->name(), "parameter")==0){
                continue;
            }
            auto attribute_name = item->first_attribute("name");
            if(attribute_name != nullptr && name == attribute_name->value()){
                xml_settings_node=item;
                current_setting.VIRpath[item->name()]=attribute_name->value();
                return true;
            }
        }
        return false;
    }

    bool Camera::last_node(){
        auto item = xml_settings_node->parent();
        if(item==nullptr || item->first_attribute("name")==nullptr){
            return false;
        }
        xml_settings_node=item;
        current_setting.VIRpath.erase(item->name());
        return true;
    }
}