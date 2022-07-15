#include "Camera.h"

#include <iostream>
#include <vector>
#include <map>
#include <rapidxml/rapidxml.hpp>

std::vector<std::string> split(const std::string& message, const std::vector<std::string>& strs){
    std::vector<std::string> out; int pos=0, index=0, itemSize=0, messageSize=message.size();
    while(index < messageSize){
        for(const auto& item : strs){
            itemSize = item.size(); 
            if(index+itemSize < messageSize && message.substr(index, itemSize) == item){
                if(pos != index){
                    out.push_back(message.substr(pos, index-pos));
                }
                index += itemSize-1; pos = index+1;
                break;
            }
        }
        index++;
    }
    if(pos != messageSize){
        out.push_back(message.substr(pos, index-pos));
    }
    return out;
}

std::vector<std::string> userPrompt(const std::string& message, const std::string& hint){
    if(message.size() > 0){
        std::cout << message << '\n';
    }
    std::cout << hint << "& ";
    std::string input; std::getline(std::cin, input);
    return split(input, {" "});
}

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=username, argv[4]=password
    std::string dns, port, username, password, xml="cgis.xml";
    for(int i = 1; i < argc; i+=2){
        if(!strcmp(argv[i], "-dns")){
            dns = argv[i+1];
        }
        else if(!strcmp(argv[i], "-port")){
            port = argv[i+1];
        }
        else if(!strcmp(argv[i], "-user")){
            username = argv[i+1];
        }
        else if(!strcmp(argv[i], "-pass")){
            password = argv[i+1];
        }
        else if(!strcmp(argv[i], "-xml")){
            xml = argv[i+1];
        }
    }
    
    lb::Camera cam(dns, port, username, password); cam.load_settings_xml(xml);

    bool quit=false;
    while(!quit){
        std::vector<std::string> command = userPrompt("", "input");
        if(command[0] == "quit"){
            quit = true;
        }
        else if(command[0] == "cd"){
            cam.enter_node_path(command[1]);
            cam.clear_current_param();
        }
        else if(command[0] == "ls"){
            std::vector<std::string> data = cam.read_node_children();
            for(int i = 0; i < data.size(); i++){
                std::cout << data[i] << '\n';
            }
        }
        else if(command[0] == "set"){
            if(cam.search_node_child(command[1]) && cam.read_node_child_name(command[1]) == "parameter"){
                cam.change_current_param(command[1], command[2]);
            }
            else
            {
                std::cout << "No such parameter!\n";
            }
        }
        else if(command[0] == "save"){
            auto current = cam.get_current_setting();

            // camera specific
            // setup cgi
            auto cgi = current.VIRpath.find("cgi");
            current.REQpath = "/stw-cgi/" + cgi->second + ".cgi";
            current.VIRpath.erase(cgi);
            // setup submenu
            auto submenu = current.VIRpath.extract("submenu");
            submenu.key() = "msubmenu";
            current.VIRpath.insert(std::move(submenu));

            if(cam.apply_setting(current)){
                std::cout << "success\n";
            }
        }
    }
}