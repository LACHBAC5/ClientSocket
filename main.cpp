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
    std::cout << hint << "> ";
    std::string input; std::getline(std::cin, input);
    return split(input, {" "});
}

bool enterFolder(rapidxml::xml_node<char> ** dir, const std::string name){
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

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=username, argv[4]=password
    // todo:
    // !! configuration commands bug testing
    // print origin command
    // print configuration command
    // readme helper commands
    // undo/redo commands
    // console authorization

    lb::Camera cam(argv[1], argv[2], argv[3], argv[4]);
    rapidxml::xml_document<char> doc;

    std::string settings = cam.load_from_file("cgis.xml");
    //std::string settings = cam.load_from_web("/stw-cgi/attributes.cgi/cgis");

    doc.parse<0>(&settings[0]);
    rapidxml::xml_node<char> * dir = doc.first_node();

    lb::setting origin;  origin.REQpath = "/stw-cgi";

    bool quit=false;
    while(!quit){
        std::vector<std::string> command = userPrompt("", "input");
        /*
        std::cout << "This is the command:\n";
        for(int i = 0; i < command.size(); i++){
            std::cout << i << ") " << command[i] << "|\n";
        }
        */

        if(command[0] == "quit"){
            quit = true;
        }
        else if(command[0] == "cd"){
            if(command[1] == ".."){
                if(!strcmp(dir->name(), "cgi")){
                    origin.REQpath = "/stw-cgi";
                }
                exitFolder(&dir);
            }
            else
            {
                // enters the next node after each '/' or at end of string
                int pos=0, oldPos=0; std::string folder;
                while(pos+1 < command[1].size()){
                    if((pos=command[1].find('/', pos+1))<0){
                        pos = command[1].size();
                    }
                    folder = command[1].substr(oldPos, pos-oldPos);

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

                    oldPos=pos+1;
                }
            }
            origin.parameters.clear();
        }
        else if(command[0] == "ls"){
            for(auto i = dir->first_node(); i != 0; i=i->next_sibling()){
                auto name_attribute = i->first_attribute("name");
                if(name_attribute != 0){
                    std::cout << name_attribute->value() << '\n';
                }
            }
        }
        else if(command[0] == "set"){
            if(!strcmp(dir->name(), "parameter")){
                std::cout << "Only parameters can be set!\n";
            }
            for(int i = 1; i < command.size(); i++){
                int pos = command[i].find('=');
                std::string name=command[i].substr(0, pos), value=command[i].substr(pos+1, command[i].size()-pos-1);
                origin.parameters[name]=value;
            }
        }
        else if(command[0] == "save"){
            if(origin.parameters.size()>0){
                // todo: better feedback
                if(cam.apply_setting(origin)){
                    std::cout << "OK\n";
                }
                else
                {
                    std::cout << "Error!\n";
                }
            }
            else
            {
                std::cout << "No changes to save!\n";
            }
        }
        else if(command[0] == "create"){
            cam.create_configuration(command[1]);
        }
        else if(command[0] == "delete"){
            cam.delete_configuration(command[1]);
        }
        else if(command[0] == "add"){
            cam.add_to_configuration(command[1], origin);
        }
        else if(command[0] == "remove"){
            cam.rm_from_configuration(command[1], origin);
        }
        else if(command[0] == "apply"){
            cam.set_active_configuration(command[1]);
        }
    }
}