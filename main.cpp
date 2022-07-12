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

bool enterFolder(rapidxml::xml_node<char> ** dir, const std::string& name){
    if(!strcmp((*dir)->first_node()->name(), "parameter")){
        return false;
    }
    for(auto i = (*dir)->first_node(); i != 0; i=i->next_sibling()){
        auto name_attribute = i->first_attribute("name");
        if(name_attribute != 0 && name_attribute->value() == name){
            *dir = i;
            return true;
        }
    }
    return false;
}

bool exitFolder(rapidxml::xml_node<char> ** dir){
    auto folder = (*dir)->parent();
    if(folder == 0){
        return false;
    }
    *dir=folder;
    return true;
}

bool checkName(rapidxml::xml_node<char> ** dir, const std::string& name){
    if((*dir)->first_node() == 0){
        return false;
    }
    for(auto i = (*dir)->first_node(); i != 0; i=i->next_sibling()){
        auto name_attribute = i->first_attribute("name");
        if(name_attribute != 0 && name_attribute->value() == name){
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=username, argv[4]=password
    // todo:
    // readme helper commands
    // undo/redo commands

    // user authentication
    lb::Camera cam(argv[1], argv[2], argv[3], argv[4]);
    /*
    lb::Camera cam(argv[1], argv[2]);
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

    // load xml settings menu
    rapidxml::xml_document<char> doc;
    std::string settings = cam.load_from_file("cgis.xml"); //std::string settings = cam.load_from_web("/stw-cgi/attributes.cgi/cgis");
    doc.parse<0>(&settings[0]);
    rapidxml::xml_node<char> * dir = doc.first_node();

    // origin as in current path and parameters
    lb::setting origin;  origin.REQpath = "/stw-cgi";
    
    // directory to be displayed to the user
    std::string userdir;

    bool quit=false;
    while(!quit){
        std::vector<std::string> command = userPrompt("", userdir);
        if(command[0] == "quit"){
            quit = true;
        }
        else if(command[0] == "cd"){ // navigation through name as an attribute of the "dir" node children
            int pos=0, oldPos=0; std::string folder;
            while(pos+1 < command[1].size()){
                if((pos=command[1].find('/', pos+1))<0){
                    pos = command[1].size();
                }
                folder = command[1].substr(oldPos, pos-oldPos);

                if(folder == ".."){
                    if(!strcmp(dir->name(), "cgi")){
                        origin.REQpath = "/stw-cgi";
                    }
                    else
                    {
                        origin.VIRpath.erase(dir->name());
                    }
                    if(!strcmp(dir->name(), "cgis") || !exitFolder(&dir)){
                        std::cout << "Invalid directory\n";
                        break;
                    }

                    // update dir visible to the user
                    int last_folder_pos = 0;
                    while(userdir.find('/', last_folder_pos)!=std::string::npos){
                        last_folder_pos=userdir.find('/', last_folder_pos)+1;
                    }
                    if(last_folder_pos > 0){
                        userdir.erase(userdir.begin()+last_folder_pos-1, userdir.end());
                    }
                }
                else
                {
                    if(!enterFolder(&dir, folder)){
                        std::cout << "Invalid directory: " << folder << '\n';
                        break;
                    }

                    // update origin
                    if(!strcmp(dir->name(), "cgi")){
                        origin.REQpath += "/" + folder + ".cgi";
                    }
                    else if(!strcmp(dir->name(), "submenu"))
                    {
                        origin.VIRpath["msubmenu"] = folder;
                    }
                    else
                    {
                        origin.VIRpath[dir->name()] = folder;
                    }

                    // update dir visible to the user
                    userdir += "/"; userdir += folder;
                }

                oldPos=pos+1;
            }
            origin.parameters.clear();
        }
        else if(command[0] == "ls"){ // print the children of "dir" by attribute name
            for(auto i = dir->first_node(); i != 0; i=i->next_sibling()){
                auto name_attribute = i->first_attribute("name");
                if(name_attribute != 0){
                    std::cout << name_attribute->value() << '\n';
                }
            }
        }
        else if(command[0] == "create"){ // create configuration
            if(!cam.create_configuration(command[1])){
                std::cout << "Configuration already exists!\n";
            }
        }
        else if(command[0] == "delete"){ // delete configuration
            if(!cam.delete_configuration(command[1])){
                std::cout << "No such configuration!\n";
            }
        }
        else if(command[0] == "add"){ // add setting to an existing configuration
            if(!strcmp(dir->name(), "parameter")){
                std::cout << "Only parameters can be set!\n";
            }
            for(int i = 2; i < command.size(); i++){
                int pos = command[i].find('=');
                std::string name=command[i].substr(0, pos), value=command[i].substr(pos+1, command[i].size()-pos-1);
                // checks if name exists in the current dir
                if(!checkName(&dir, name)){
                    std::cout << "No such param in the current directory!\n";
                    break;
                }
                origin.parameters[name]=value;
            }

            if(!cam.add_to_configuration(command[1], origin)){
                std::cout << "No such configuration!\n";
            }
            else
            {
                origin.parameters.clear();
            }
        }
        else if(command[0] == "remove"){ // remove setting from an existing configuration
            if(!strcmp(dir->name(), "parameter")){
                std::cout << "Only parameters can be removed!\n";
            }
            for(int i = 2; i < command.size(); i++){
                origin.parameters[command[i]]="";
            }

            if(!cam.rm_from_configuration(command[1], origin)){
                std::cout << "No such configuration or path!\n";
            }
            else
            {
                origin.parameters.clear();
            }
        }
        else if(command[0] == "apply"){ // change camera configuration to an existing configuration
            if(!cam.set_active_configuration(command[1])){
                std::cout << "No such configuration!\n";
            }
        }
        else if(command[0] == "print"){ // print the parameters of an existing configuration
            auto item = cam.get_configuration(command[1]);
            for(int i = 0; i < item.size(); i++){
                std::cout << "---setting num: " << i << '\n';
                std::cout << "parameters:\n";
                for(const auto& [name, value] : item[i].parameters){
                    std::cout << name << " = " << value << '\n';
                }
                std::cout << "vipath:\n";
                for(const auto& [name, value] : item[i].VIRpath){
                    std::cout << name << " = " << value << '\n';
                }
                std::cout << "reqpath:\n" << item[i].REQpath << '\n';
            }
        }
    }
}