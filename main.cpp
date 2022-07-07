#include "Camera.h"

#include <iostream>
#include <vector>
#include <map>

std::string userPrompt(const std::string& message, const std::string& hint){
    std::string out;
    std::cout << message << '\n' << hint << "> ";
    std::cin >> out;

    return out;
}

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=USERNAME, argv[4]=PASSWORD
    /*
    input> load
    From where to load?
    web/locale> web
    What to load?
    uri> /stw-cgi/attributes.cgi/cgis
    ***
    image
    ***
    input> image
    */

    lb::Camera a(argv[1], argv[2], argv[3], argv[4]);

    std::map<std::string, std::string> args;
    std::vector<std::pair<std::string, std::string>> path;

    int top = 0;

    bool loaded = false;
    bool quit = false;
    while(!quit){
        if(loaded){
            a.print_layer();
        }

        std::string message; std::cout << "\ninput> ";
        std::cin >> message;

        if(message == "prev"){
            if(args.size() > 0 && userPrompt("Save changes?", "yes/no") == "yes"){
                std::cout << a.send_request(path, args) << '\n';
                args.clear();
            }

            if(a.previous()){
                top--;
            }
        }
        else if(message == "quit"){
            if(args.size() > 0 && userPrompt("Save changes?", "yes/no") == "yes"){
                std::cout << a.send_request(path, args) << '\n';
                args.clear();
            }

            quit = true;
        }
        else if(message == "load"){
            std::string input = userPrompt("From where to load?", "web/locale");
            if(input == "web"){
                a.load_from_web(userPrompt("What to load?", "uri"));
            }
            else if(input == "locale"){
                a.load_from_file(userPrompt("What to load?", "uri"));
            }
            loaded = true;
            args.clear(); path.clear();
        }
        else if(message == "save"){
            path.erase(path.begin()+top, path.end());
            std::cout << a.send_request(path, args) << '\n';
            args.clear();
        }
        else
        {
            std::string root_name = a.get_root_name();

            if(a.get_root_name() == "parameter"){
                root_name = message;
                std::cout << "value>"; std::cin>>message;

                args[root_name] = message;
            }
            else if(a.next(message))
            {
                if(top == path.size()){
                    path.push_back(std::make_pair(root_name, message));
                }
                else
                {
                    path[top] = std::make_pair(root_name, message);
                }
                top++;
            }
        }
    }
}