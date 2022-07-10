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

    std::string Camera::send_request(const std::string& path, const std::map<std::string, std::string>& names, const std::map<std::string, std::string>& parameters){
        std::string request = path;

        if(names.size() > 0 || parameters.size() > 0){
            request += '?';
        }

        for(auto i = names.begin(); i != names.end(); i++){
            request += i->first+'='+i->second+'&';
        }
        for(auto i = parameters.begin(); i != parameters.end(); i++){
            request += i->first+'='+i->second+'&';
        }
        request.pop_back();

        std::cout << request << '\n';

        std::string out;
        /*
        cs.open_connection();
        int status_code = cs.status_code(cs.send_http_request("GET", request));
        if(status_code == 200){
            out = cs.fetch_response();
        }
        */
        return out;
    }

    void Camera::load_from_file(const std::string& path){
        std::string text, line;
        std::ifstream file(path);
        while(std::getline(file, line) && line.size() != 0){
            text += line;
        }

        doc.parse<0>(&text[0]);
        root = doc.first_node()->first_node();
    }
    void Camera::load_from_web(const std::string& path){
        if(cs.open_connection()!=0){
            return;
        }
        if(cs.status_code(cs.send_http_request("GET", path)) != 200){
            return;
        }
        
        std::string text = cs.fetch_response();

        doc.parse<0>(&text[0]);
        root = doc.first_node()->first_node();
    }

    std::string Camera::get_layer(bool show_node_name, bool show_attrib_name, const std::vector<std::string>& attribute_names){
        std::string out;
        
        for(auto i = root; i != 0; i=i->next_sibling()){
            if(show_node_name){
                out += i->name(); out += " | ";
            }
            bool first = true;
            for(auto o = i->first_attribute(); o != 0; o = o->next_attribute()){
                if(attribute_names.size()==0 || std::find(attribute_names.begin(), attribute_names.end(), std::string(o->name()))!=attribute_names.end()){
                    if(!first){
                        out += ", ";
                    }
                    else
                    {
                        first = false;
                    }
                    if(show_attrib_name){
                        out += o->name(); out += " -> ";
                    }
                    out += o->value();
                }
            }
            out += '\n';
        }
        
        return out;
    }

    bool Camera::next(const std::vector<std::pair<std::string, std::string>>& names){
        for(auto i = root; i != 0; i=i->next_sibling()){
            bool passed = true;
            for(const auto& [name, value] : names){
                if(i->first_attribute(name.c_str())->value() != value){
                    passed = false;
                    break;
                }
            }
            if(passed){
                root = i->first_node();
                return true;
            }
        }
        return false;
    }
    bool Camera::previous(){
        if(root->parent()==0){
            return false;
        }
        root = root->parent();
        while(root->previous_sibling()!=0){
            root = root->previous_sibling();
        }
        return true;
    }

    std::string Camera::get_root_name(){
        std::string out = root->name();
        if(out == "submenu"){
            return "msubmenu";
        }
        return out;
    }

    bool Camera::get_attrib(const std::vector<std::pair<std::string, std::string>>& names){
        for(auto i = root; i != 0; i=i->next_sibling()){
            bool passed = true;
            for(const auto& [name, value] : names){
                if(i->first_attribute(name.c_str())->value() != value){
                    passed = false;
                    break;
                }
            }
            if(passed){
                return true;
            }
        }
        return false;
    }
}