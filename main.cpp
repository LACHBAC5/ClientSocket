#include "Camera.h"

#include <iostream>
#include <vector>
#include <map>

std::vector<std::string> userPrompt(const std::string& message, const std::string& hint){
    std::string input; input.resize(30);
    if(message.size() != 0){
        std::cout << message << '\n';
    }
    std::cout << hint << "> ";
    std::cin.getline(&input[0], 30);

    int pos = 0;
    std::vector<std::string> out;
    for(int i = 0; i < input.size(); i++){
        if(input[i] == ' '){
            out.push_back(input.substr(pos, i-pos));
            pos = i+1;
        }
        else if(input[i] == '\0' || input[i] == '/'){
            out.push_back(input.substr(pos, i-pos));
            return out;
        }
    }

    return {};
}

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=USERNAME, argv[4]=PASSWORD
    lb::Camera cam(argv[1], argv[2], argv[3], argv[4]);

    std::map<std::string, std::string> args;
    std::vector<std::pair<std::string, std::string>> path;

    int top = 0;

    bool show_node_name=false, show_attrib_name=false;
    std::vector<std::string> attribute_list = {"name"};

    cam.load_from_file("cgis.xml");
    //cam.load_from_web("/stw-cgi/attributes.cgi/cgis");

    
    bool quit = false;
    while(!quit){
        std::cout << cam.get_layer(show_node_name, show_attrib_name, attribute_list) << '\n';

        std::vector<std::string> command = userPrompt("", "input");

        if(command[0] == "save"){
            path.erase(path.begin()+top, path.end());
            std::cout << cam.send_request(path, args) << '\n';
            args.clear();
        }
        else if(command[0] == "quit"){

            if(args.size() > 0 && userPrompt("Save changes?", "yes/no")[0] == "yes"){
                std::cout << cam.send_request(path, args) << '\n';
            }
            quit = true;
        }
        else if(command[0] == "cd"){
            for(const auto& p : command){
                if(p == ".."){
                    // last directory
                    continue;
                }
                // load atributes as input of the search
            }
        }
        else if(command[0] == "previous"){

            if(args.size() > 0 && userPrompt("Save changes?", "yes/no")[0] == "yes"){
                std::cout << cam.send_request(path, args) << '\n';
            }
            args.clear();

            if(cam.previous()){
                path[top-1] = {"", ""};
                top--;
            }
        }
        else
        {
            std::string root_name = cam.get_root_name();

            if(root_name == "parameter"){
                std::vector<std::string> input = userPrompt("", "value");

                args[command[0]] = input[0];
            }
            else if(cam.next(command[0])){
                if(top == path.size()){
                    path.push_back(std::make_pair(root_name, command[0]));
                }
                else
                {
                    path[top] = std::make_pair(root_name, command[0]);
                }
                top++;
            }
        }
    }
}