#include "Camera.h"

#include <iostream>
#include <vector>
#include <map>

int main(int argc, char *argv[]){ // argv[1]=DNS, argv[2]=PORT, argv[3]=USERNAME, argv[4]=PASSWORD
    // todo:
    // load from web function
    // load commant for interface
    // erase argument commant for interface
    // print argument commant for interface
    // better error checking

    lb::Camera a(argv[1], argv[2], argv[3], argv[4]);
    a.load_from_file("cgis.xml");

    std::map<std::string, std::string> args;
    std::vector<std::pair<std::string, std::string>> path;

    int top = 0;

    bool quit = false;
    while(!quit){
        a.print_layer();

        std::string message; std::cout << "\ninput> ";
        std::cin >> message;

        if(message == "prev"){
            if(args.size() > 0){
                std::cout << "Save changes?\nyes/no>"; std::cin>>message;
                if(message == "yes"){
                    std::cout << a.send_request(path, args) << '\n';
                    args.clear();
                }
            }
            if(a.previous()){
                top--;
            }
        }
        else if(message == "quit"){
            if(args.size() > 0){
                std::cout << "Save changes?\nyes/no>"; std::cin>>message;
                if(message == "yes"){
                    std::cout << a.send_request(path, args) << '\n';
                    args.clear();
                }
            }
            quit = true;
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