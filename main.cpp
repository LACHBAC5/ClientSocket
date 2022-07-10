#include "Camera.h"

#include <iostream>
#include <vector>
#include <map>

std::string userPrompt(const std::string& message, const std::string& hint){
    if(message.size() > 0){
        std::cout << message << '\n';
    }
    std::cout << hint << "> ";
    std::string input; std::getline(std::cin, input);

    return input;
}

std::vector<std::string> split(const std::string& message, const std::string& splitters){
    std::vector<std::string> out; int pos=0;
    for(int i = 0; i < message.size(); i++){
        if(std::find(splitters.begin(), splitters.end(), message[i]) != splitters.end()){
            out.push_back(message.substr(pos, i-pos));
            pos = i+1;
        }
        else if(i == message.size()-1){
            out.push_back(message.substr(pos, i+1-pos));
        }
    }
    return out;
}

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT
    lb::Camera cam(argv[1], argv[2]);
    /*
    bool authenticated = false;
    while(!authenticated){
        cam.change_username(userPrompt("Sign in:", "Username"));
        cam.change_password(userPrompt("", "Password"));

        if(cam.send_request({"stw-cgi", "image.cgi"}, {{"msubmenu", "imageenhancements"}, {"action", "view"}, {"Channel", "0"}}).size() > 0){
            std::cout << "authenticated!\n\n";
            authenticated = true;
        }
        else
        {
            std::cout << "wrong password or username!\n";
        }
    }
    */

    cam.load_from_file("cgis.xml");

    std::string path = "/stw-cgi";
    std::map<std::string, std::string> names;
    std::map<std::string, std::string> parameters;
    
    bool quit=false;
    while(!quit){
        std::vector<std::string> command = split(userPrompt("", "input"), " ");

        if(command[0] == "quit"){
            quit = true;
            break;
        }
        else if(command[0] == "cd"){
            int slashPos=0, oldSlashPos=0;
            if(command[1] == ".."){
                if(cam.previous()){
                    std::string root_name = cam.get_root_name();

                    if(names.find(root_name) != names.end()){
                        names.erase(root_name);
                    }
                    else
                    {
                        while((slashPos=path.find('/', slashPos+1))!=std::string::npos){
                            oldSlashPos = slashPos;
                        }
                        path.erase(path.begin()+oldSlashPos, path.end());
                    }
                }
            }
            else
            {
                while(slashPos!=command[1].size()){
                    slashPos = slashPos=command[1].find('/', slashPos+1);
                    if(slashPos==std::string::npos){
                        slashPos = command[1].size();
                    }
                    std::string name = command[1].substr(oldSlashPos, slashPos-oldSlashPos);
                    std::string root_name = cam.get_root_name();
                    if(cam.get_root_name()!="parameter"&&cam.next({{"name", name}})){
                        if(root_name == "cgi"){
                            path += '/' + name + ".cgi";
                        }
                        else
                        {
                            names[root_name] = name;
                        }
                    }
                    else
                    {
                        std::cout << "No such file or directory: " + name + "\n";
                        break;
                    }
                    oldSlashPos = slashPos+1;
                }

            }
            parameters.clear();
        }
        else if(command[0] == "set"){
            if(cam.get_root_name()=="parameter"){
                if(cam.get_attrib({{"name", command[1]}})){
                    // todo: add value checking
                    parameters[command[1]] = command[2];
                }
                else
                {
                    std::cout << "No such parameter: " << command[1] << '\n';
                }
            }
            else
            {
                std::cout << "No such parameter: " << command[1] << '\n';
            }
        }
        else if(command[0] == "erase"){
            auto item = parameters.find(command[1]);
            if(item != parameters.end()){
                parameters.erase(item);
            }
            else
            {
                std::cout << "No such parameter saved!\n";
            }
        }
        else if(command[0] == "save"){
            if(parameters.size() > 0){
                std::string out = cam.send_request(path, names, parameters);
                if(out.size() > 0){
                    std::cout << "done\n";
                }
                else
                {
                    std::cout << "bad request!\n";
                }
            }
            else
            {
                std::cout << "There aren't any changes!\n";
            }
        }
        else if(command[0] == "ls"){
            std::cout << cam.get_layer(false, false, {"name"}) << '\n';
        }
        else if(command[0] == "print"){
            if(command[1] == "config"){

            }
            else if(command[1] == "unsaved"){

            }
        }
        else if(command[0] == "make"){
            if(command[1] == "config"){

            }
        }
    }
}