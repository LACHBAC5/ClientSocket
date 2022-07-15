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
    // user authentication
    //lb::Camera cam(argv[1], argv[2], argv[3], argv[4]);
    
    lb::Camera cam(argv[1], argv[2], argv[3], argv[4]); cam.load_settings_file("cgis.xml");
    /*
    bool authenticated = false;
    while(!authenticated){
        cam.change_username(userPrompt("Sign in:", "Username")[0]);
        cam.change_password(userPrompt("", "Password")[0]);
        if(cam.apply_setting({{{"Channel", "0"}}, {{"msubmenu", "imageenhancements"}, {"action", "view"}}, "/stw-cgi/image.cgi"})){
            std::cout << "authenticated!\n";
            authenticated = true;
        }
        else
        {
            std::cout << "wrong password or username!\n";
        }
    }
    */

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
            cam.change_current_param(command[1], command[2]);
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