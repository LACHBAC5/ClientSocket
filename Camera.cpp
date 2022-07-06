#include "Camera.h"

lb::Camera::Camera(const std::string& ip, const std::string& port, const std::string& username, const std::string& password) : cs(ip, port, username, password) {}

lb::Camera::~Camera() {}

std::string lb::Camera::send_request(const std::vector<std::pair<std::string, std::string>>& path, const std::map<std::string, std::string>& args){
    

    std::string request = "/stw-cgi/" + path[0].second+".cgi?msubmenu="+path[1].second;

    for(int i = 2; i < path.size(); i++){
        request += '&'+path[i].first+'='+path[i].second;
    }

    for(const auto& [name, value] : args){
        request += "&"+name+"="+value;
    }

    cs.open_connection();

    std::string out;

    int status_code = cs.status_code(cs.send_http_request("GET", request));
    if(status_code == 200){
        out = cs.fetch_response();
    }
    
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

bool lb::Camera::next(const std::string& name){
    bool out = false;
    for(auto i = root; i != 0; i=i->next_sibling()){
        if(i->first_attribute("name") != 0 && i->first_attribute("name")->value() == name){
            root = i->first_node();
            out = true;
            break;
        }
    }
    return out;
}
bool lb::Camera::previous(){
    if(root->parent()==0){
        return false;
    }
    root = root->parent();
    while(root->previous_sibling()!=0){
        root = root->previous_sibling();
    }
    return true;
}

std::string lb::Camera::get_root_name(){
    return root->name();
}