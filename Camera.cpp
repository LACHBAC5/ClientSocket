#include "Camera.h"

lb::Camera::Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : cs(ip, port, username, password) {}

lb::Camera::~Camera() {}

std::string lb::Camera::send_request(const std::array<std::string, 3>& path, const std::map<std::string, std::string>& args){
    

    std::string request = "/stw-cgi/" + path[0]+".cgi?msubmenu="+path[1]+"&action="+path[2];
    for(const auto& [name, value] : args){
        request += "&"+name+"="+value;
    }

    cs.open_connection();

    std::string out = cs.send_http_request("GET", request);

    return out;
}

void lb::Camera::load_from_file(const std::string& path){
    std::string text, line;
    std::ifstream file(path);
    while(std::getline(file, line) && line.size() != 0){
        text += line;
    }
    doc.parse<0>(&text[0]);

    root = doc.first_node()->first_node();
}

void lb::Camera::print_layer(){
    for(auto i = root; i != 0; i=i->next_sibling()){
        if(i->first_attribute("name") != 0){
            std::cout << i->first_attribute("name")->value() << '\n';
        }
    }
}

void lb::Camera::next(const std::string& name){
    for(auto i = root; i != 0; i=i->next_sibling()){
        if(i->first_attribute("name") != 0 && i->first_attribute("name")->value() == name){
            root = i->first_node();
            break;
        }
    }
}
void lb::Camera::previous(){
    root = root->parent();
    while(root->previous_sibling()!=0){
        root = root->previous_sibling();
    }
}

std::string lb::Camera::get_root_name(){
    return root->name();
}